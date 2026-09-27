# Government Identification Database Using BST

## 1. Aim

To implement a Binary Search Tree (BST) for organizing government identification numbers and compare the performance of BST Search with Linear Search.

## 2. Problem Statement

A government database stores the following identification numbers:

A102, A25, A7, B100, B12, A120, B3, A45

The objectives are:

- Implement a BST to organize the identification numbers.
- Display the inorder traversal.
- Analyse the resulting BST structure.
- Compare BST Search and Linear Search.
- Record the number of comparisons.
- Analyse the effect of key length and insertion order on BST height and search performance.
- Compare the observed results with theoretical complexities.
- Suggest a suitable approach for maintaining efficient searches as the database grows.

## 3. Input Data

The identification numbers are inserted into the BST in the following order:

A102, A25, A7, B100, B12, A120, B3, A45

(IDs are alphanumeric, so all ordering uses lexicographic — `strcmp`-style — comparison, not numeric value. That's why `A120` sorts before `A25`: at the second character, `'1' < '2'`.)

## 4. BST Structure

```text
ROOT: A102
    \--R: A25
        /--L: A120
        \--R: A7
            /--L: A45
            \--R: B100
                \--R: B12
                    \--R: B3
```

The height of the BST is **6 levels** (8 nodes, 3 leaves). See `trace_table.md` for the step-by-step insertion trace that produces this shape.

## 5. Inorder Traversal

```text
A102 A120 A25 A45 A7 B100 B12 B3
```

## 6. BST Search vs Linear Search

| ID | BST Comparisons | Linear Comparisons | Result |
|:---:|:---:|:---:|:---:|
| A102 | 1 | 1 | Found |
| A25 | 2 | 2 | Found |
| A7 | 3 | 3 | Found |
| B100 | 4 | 4 | Found |
| B12 | 5 | 5 | Found |
| A120 | 3 | 6 | Found |
| B3 | 6 | 7 | Found |
| A45 | 4 | 8 | Found |
| C99 | 6 | 8 | Not Found |

Full breakdown (including character-level comparisons) in `comparison_table.md`.

## 7. Complexity Analysis

| Operation | Best | Average | Worst |
|:---|:---:|:---:|:---:|
| BST Search | O(1) | O(log n) | O(n) |
| BST Insert | O(1) | O(log n) | O(n) |
| Linear Search | O(1) | O(n) | O(n) |
| Inorder Traversal | O(n) | O(n) | O(n) |

Space: O(n) to store n keys either way; BST insert/search carry an additional O(h) recursion-stack cost. Full explanations in `complexity_analysis.md`.

## 8. Effect of Insertion Order

The insertion order affects the height of a normal BST — this was measured directly rather than just argued: the same 8 keys were rebuilt in sorted order and in a balanced order, giving heights of 8 (worst case) and 4 (best case) respectively, against 6 for the order given in the question. Details and the reasoning in `complexity_analysis.md`.

## 9. Effect of Key Length

Because IDs are compared character-by-character (`strcmp`), longer keys and keys sharing long common prefixes cost more *character* comparisons per node visited, even when the *node-level* comparison count is low. This affects the constant factor, not the O(log n)/O(n) complexity. Measured character-comparison counts are in `comparison_table.md`.

## 10. Suitable Approach for a Growing Database

For a growing database, a balanced search tree is more suitable than Linear Search. A self-balancing BST such as an AVL Tree or Red-Black Tree can maintain a balanced structure and guarantee O(log n) search performance regardless of the order IDs arrive in — which matters here, since sequential ID assignment (a very plausible real-world pattern) is exactly the worst case for a plain BST.

## 11. Conclusion

The experiment shows that BST Search requires fewer comparisons than Linear Search for most of the identification numbers, and never more. The performance of a normal BST depends on its structure and insertion order — if the tree becomes skewed, its search performance can degrade to O(n), matching Linear Search with none of the benefit.

Therefore, for a growing government identification database, maintaining a **balanced BST** is the suitable approach for efficient searching.

## 12. Files Included

| File | Purpose |
|---|---|
| `government_id_bst.c` | C source code — BST + linear search, insertion trace, and the insertion-order experiment |
| `input.txt` | Input identification numbers |
| `output.txt` | Full captured program execution output |
| `trace_table.md` | Insertion trace table and search trace table |
| `complexity_analysis.md` | Theoretical complexity + measured insertion-order and key-length effects |
| `comparison_table.md` | BST vs Linear Search comparison (node- and character-level) |
| `README.md` | This file — project documentation |

### How to build and run

```bash
gcc -Wall -o government_id_bst government_id_bst.c
./government_id_bst
```
