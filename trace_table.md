# Trace Table - Government Identification BST

## 1. Insertion Trace Table

| Step | ID Inserted | Comparison Path | Position | Comparisons |
| ---: | ----------- | ---------------------------- | ------------- | ---: |
| 1 | A102 | — | Root | 0 |
| 2 | A25 | A102 | Right of A102 | 1 |
| 3 | A7 | A102 → A25 | Right of A25 | 2 |
| 4 | B100 | A102 → A25 → A7 | Right of A7 | 3 |
| 5 | B12 | A102 → A25 → A7 → B100 | Right of B100 | 4 |
| 6 | A120 | A102 → A25 | Left of A25 | 2 |
| 7 | B3 | A102 → A25 → A7 → B100 → B12 | Right of B12 | 5 |
| 8 | A45 | A102 → A25 → A7 | Left of A7 | 3 |

**Total comparisons to build the tree: 20**

## 2. BST Search vs Linear Search Trace Table

| No. | Search ID | BST Search Path | BST Comparisons | Linear Comparisons | Result |
| --: | :-------: | --------------------------------- | ---------------: | ------------------: | --------- |
| 1 | A102 | A102 | 1 | 1 | Found |
| 2 | A25 | A102 → A25 | 2 | 2 | Found |
| 3 | A7 | A102 → A25 → A7 | 3 | 3 | Found |
| 4 | B100 | A102 → A25 → A7 → B100 | 4 | 4 | Found |
| 5 | B12 | A102 → A25 → A7 → B100 → B12 | 5 | 5 | Found |
| 6 | A120 | A102 → A25 → A120 | 3 | 6 | Found |
| 7 | B3 | A102 → A25 → A7 → B100 → B12 → B3 | 6 | 7 | Found |
| 8 | A45 | A102 → A25 → A7 → A45 | 4 | 8 | Found |
| 9 | C99 | A102 → A25 → A7 → B100 → B12 → B3 | 6 | 8 | Not Found |

*(Every ID in the database was searched, plus one key — C99 — that does not exist, to trace the "not found" path.)*

Full step-by-step console trace (each individual `<` / `>` decision) is in [`output.txt`](./output.txt), produced by running [`government_id_bst.c`](./government_id_bst.c).
