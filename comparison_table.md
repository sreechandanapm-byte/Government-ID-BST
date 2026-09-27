# BST Search vs Linear Search Comparison

## 1. Search Comparison Results

| No. | Identification ID | BST Comparisons | Linear Comparisons | More Efficient |
|:---:|:------------------:|:---------------:|:------------------:|:--------------:|
| 1 | A102 | 1 | 1 | Same |
| 2 | A25 | 2 | 2 | Same |
| 3 | A7 | 3 | 3 | Same |
| 4 | B100 | 4 | 4 | Same |
| 5 | B12 | 5 | 5 | Same |
| 6 | A120 | 3 | 6 | BST |
| 7 | B3 | 6 | 7 | BST |
| 8 | A45 | 4 | 8 | BST |
| 9 | C99 (not found) | 6 | 8 | BST |

## 2. Character-Level Comparisons (effect of key length)

Because the IDs are alphanumeric strings, each "comparison" above is itself a character-by-character scan. Counting individual character comparisons gives a finer-grained view of the actual work done:

| No. | Identification ID | BST char comparisons | Linear char comparisons |
|:---:|:------------------:|:---------------------:|:------------------------:|
| 1 | A102 | 5 | 5 |
| 2 | A25 | 6 | 6 |
| 3 | A7 | 7 | 7 |
| 4 | B100 | 8 | 8 |
| 5 | B12 | 10 | 10 |
| 6 | A120 | 10 | 14 |
| 7 | B3 | 10 | 11 |
| 8 | A45 | 10 | 15 |
| 9 | C99 (not found) | 6 | 8 |

## 3. Performance Analysis

| Search Method | Best Case | Average Case | Worst Case |
|:-------------|:---------:|:------------:|:----------:|
| BST Search | O(1) | O(log n) | O(n) |
| Linear Search | O(1) | O(n) | O(n) |

## 4. Observed Comparison (node-level)

| Identification ID | BST | Linear Search | Difference |
|:-----------------:|----:|--------------:|----------:|
| A102 | 1 | 1 | 0 |
| A25 | 2 | 2 | 0 |
| A7 | 3 | 3 | 0 |
| B100 | 4 | 4 | 0 |
| B12 | 5 | 5 | 0 |
| A120 | 3 | 6 | 3 |
| B3 | 6 | 7 | 1 |
| A45 | 4 | 8 | 4 |
| C99 | 6 | 8 | 2 |

## 5. Analysis

The results show that BST search requires fewer comparisons than linear search for A120, B3, A45, and C99 — every ID that isn't in the first half of the original list. For A102, A25, A7, B100, and B12, both methods happen to need the same number of comparisons, because those five keys sit at the front of the array *and* along the same downward path in the tree.

The BST performs efficiently because search follows only one path from the root instead of checking every element sequentially. The gap grows with dataset position: for A45 (near the end of the original list), linear search needs *twice* as many comparisons as BST search.

However, the performance of a normal BST depends on its structure, not just its size. If the tree becomes skewed because of an unfavourable insertion order, its search performance can degrade toward O(n) — see `complexity_analysis.md` for a measured example (the same 8 keys produce a tree anywhere from height 4 to height 8 depending purely on insertion order).

## 6. Conclusion

For the given identification numbers, the BST provides better or equal search performance for every selected ID compared with linear search. For a growing database, a **balanced BST (AVL or Red-Black Tree)** is the more suitable approach, since it guarantees O(log n) search time regardless of the order IDs are added in.
