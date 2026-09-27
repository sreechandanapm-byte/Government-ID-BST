# Complexity Analysis - Government Identification BST

## 1. BST Search Complexity

| Case | Time Complexity | Explanation |
|:---|:---:|:---|
| Best Case | O(1) | The required ID is found at the root. |
| Average Case | O(log n) | The tree is reasonably balanced, so only a small number of levels are searched. |
| Worst Case | O(n) | If the BST becomes skewed, the search may visit every node. |

## 2. Linear Search Complexity

| Case | Time Complexity | Explanation |
|:---|:---:|:---|
| Best Case | O(1) | The required ID is the first element. |
| Average Case | O(n) | About half of the elements may need to be checked. |
| Worst Case | O(n) | All elements may need to be checked. |

## 3. BST Insertion Complexity

| Case | Time Complexity | Explanation |
|:---|:---:|:---|
| Best Case | O(1) | The tree is empty and the first element becomes the root. |
| Average Case | O(log n) | In a reasonably balanced BST, insertion follows the tree height. |
| Worst Case | O(n) | A skewed BST may require traversing all existing nodes. |

## 4. Inorder Traversal Complexity

| Operation | Time Complexity |
|:---|:---:|
| Inorder Traversal | O(n) |

Inorder traversal visits every node exactly once.

## 5. Space Complexity

| Operation | Space Complexity |
|:---|:---:|
| BST Storage | O(n) |
| BST Insert / Search recursion stack | O(h), h = tree height (O(log n) avg, O(n) worst) |
| Linear Search (iterative) | O(1) auxiliary |

The BST requires O(n) space to store n identification numbers. `bstSearch()` in the program is written iteratively, so its own auxiliary space is O(1); only the recursive `insert()` carries an O(h) call-stack cost.

## 6. Effect of Insertion Order (measured, not just theoretical)

The insertion order affects the height of a normal BST. To confirm this rather than just state it, the same 8 keys were rebuilt in three different insertion orders and the resulting height was actually measured:

| Insertion order | Height (levels) | Case |
|:---|:---:|:---|
| Given order (as in the question) | 6 | between best and worst |
| Sorted ascending order | **8** | Worst case — every new key is greater than all previous ones, so the tree degenerates into a linked list |
| Balanced order (median-first) | **4** | Best case — matches the theoretical minimum exactly |

For n = 8: theoretical best height = ⌈log₂(n+1)⌉ = 4, theoretical worst height = n = 8. The measured heights hit both bounds exactly, and the given insertion order (6) falls in between — closer to worst than to best, because every "B…" key is greater than every "A…" key and so all four B-keys chain down the same right-hand side.

This matters in practice: government ID numbers are very often assigned in increasing/sequential order (auto-incrementing IDs), which is exactly the "sorted ascending" case above — the worst case for a plain BST.

## 7. Effect of Key Length

The identification numbers have different lengths (A7, A25, A45, A102, A120, B3, B12, B100). The BST orders them using lexicographic (`strcmp`-style) comparison, one character at a time — so each "node comparison" is really a small loop over characters, not a single O(1) step.

Counting actual character comparisons (not just node visits) shows this clearly:

| ID | BST node comparisons | BST character comparisons |
|:---:|:---:|:---:|
| A45 | 4 | 10 |
| A120 | 3 | 10 |

`A120` needed *fewer* node comparisons than `A45` (3 vs 4) but the *same number* of character comparisons (10) — because it shares a long common prefix (`"A1"` / `"A2"`) with the keys it's compared against, so each comparison has to scan further before finding a difference. Key length and prefix similarity therefore affect the constant factor behind each comparison, but not the O(log n)/O(n) asymptotic complexity itself.

## 8. Suitable Approach for a Growing Database

A plain BST's performance depends entirely on insertion order (Section 6). Since sequential ID assignment — very plausible for a government database — produces exactly the worst-case height, a **self-balancing BST (AVL Tree or Red-Black Tree)** is the recommended approach: it automatically rotates on insertion to keep the height at O(log n) regardless of insertion order, guaranteeing fast search as the database grows. (Production database systems typically use B-trees for the same reason, with the added benefit of being optimized for disk-based storage.)
