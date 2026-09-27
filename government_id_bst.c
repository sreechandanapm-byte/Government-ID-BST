/* ============================================================
   GOVERNMENT IDENTIFICATION DATABASE - BST IMPLEMENTATION
   ------------------------------------------------------------
   Question 11:
   a) Build BST + inorder traversal; analyse the tree structure
   b) Compare BST Search vs Linear Search (comparison counts)
   c) Analyse effect of insertion order / key length on height
      and search performance
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_LEN 20

struct Node
{
    char id[ID_LEN];
    struct Node *left;
    struct Node *right;
};

/* ---------- Global comparison counters ----------
   Every comparison in this program goes through myCompare(), so both
   node-level comparisons (how many nodes were visited) and character-
   level comparisons (how many characters were actually examined) get
   tracked together. depthOf() below uses plain strcmp() on purpose --
   it is only a display helper and must not disturb these counts. */
long nodeCompareCount = 0;
long charCompareCount = 0;

int myCompare(const char *a, const char *b)
{
    nodeCompareCount++;
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0')
    {
        charCompareCount++;
        if (a[i] != b[i]) return (unsigned char)a[i] - (unsigned char)b[i];
        i++;
    }
    charCompareCount++;
    return (unsigned char)a[i] - (unsigned char)b[i];
}

/* ---------- Node creation ---------- */
struct Node* createNode(char id[])
{
    struct Node *newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newNode->id, id);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/* ---------- Insert into BST (trace = 1 prints each decision) ---------- */
struct Node* insert(struct Node *root, char id[], int trace)
{
    if (root == NULL)
    {
        if (trace) printf("    -> \"%s\" inserted here (new node)\n", id);
        return createNode(id);
    }

    int cmp = myCompare(id, root->id);
    if (cmp < 0)
    {
        if (trace) printf("    \"%s\" < \"%s\" -> go LEFT\n", id, root->id);
        root->left = insert(root->left, id, trace);
    }
    else if (cmp > 0)
    {
        if (trace) printf("    \"%s\" > \"%s\" -> go RIGHT\n", id, root->id);
        root->right = insert(root->right, id, trace);
    }
    else if (trace)
    {
        printf("    \"%s\" already exists, duplicate skipped\n", id);
    }
    return root;
}

/* ---------- Inorder traversal (gives sorted order) ---------- */
void inorder(struct Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%s ", root->id);
        inorder(root->right);
    }
}

/* ---------- Height of BST (root counted as level 1) ---------- */
int height(struct Node *root)
{
    if (root == NULL) return 0;
    int lh = height(root->left);
    int rh = height(root->right);
    return (lh > rh ? lh : rh) + 1;
}

int countNodes(struct Node *root)
{
    if (root == NULL) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(struct Node *root)
{
    if (root == NULL) return 0;
    if (root->left == NULL && root->right == NULL) return 1;
    return countLeaves(root->left) + countLeaves(root->right);
}

/* ---------- Depth (level) at which a key is found (1-indexed) ----------
   Uses plain strcmp on purpose -- a display helper only. */
int depthOf(struct Node *root, char key[], int level)
{
    if (root == NULL) return -1;
    if (strcmp(key, root->id) == 0) return level;
    if (strcmp(key, root->id) < 0) return depthOf(root->left, key, level + 1);
    return depthOf(root->right, key, level + 1);
}

/* ---------- BST Search ---------- */
int bstSearch(struct Node *root, char key[])
{
    while (root != NULL)
    {
        int cmp = myCompare(key, root->id);
        if (cmp == 0) return 1;
        root = (cmp < 0) ? root->left : root->right;
    }
    return 0;
}

/* ---------- Linear Search ---------- */
int linearSearch(char ids[][ID_LEN], int n, char key[])
{
    for (int i = 0; i < n; i++)
    {
        if (myCompare(ids[i], key) == 0) return 1;
    }
    return 0;
}

void freeTree(struct Node *root)
{
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

/* ============================================================
   VISUAL TREE PRINTER #1 -- simple indented view (safe on any terminal)
   ============================================================ */
void printTree(struct Node *node, char *prefix, int isRoot, int isLeftChild, const char *branchLabel)
{
    if (node == NULL) return;

    if (isRoot)
        printf("ROOT: %s\n", node->id);
    else
        printf("%s%s--%s: %s\n", prefix, isLeftChild ? "/" : "\\", branchLabel, node->id);

    char newPrefix[256];
    if (isRoot)
        strcpy(newPrefix, "    ");
    else
        snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLeftChild ? "|   " : "    ");

    if (node->left)  printTree(node->left,  newPrefix, 0, 1, "L");
    if (node->right) printTree(node->right, newPrefix, 0, 0, "R");
}

/* ============================================================
   VISUAL TREE PRINTER #2 -- centered top-down "/  \" diagram
   (classic recursive box-layout algorithm)
   ============================================================ */
#define MAX_LINES 30
#define MAX_WIDTH 200

typedef struct
{
    char lines[MAX_LINES][MAX_WIDTH];
    int  numLines;
    int  width;
    int  rootPos;
} Box;

static void fillSpaces(char *buf, int *pos, int count) { for (int i = 0; i < count; i++) buf[(*pos)++] = ' '; }
static void fillChar(char *buf, int *pos, char c, int count) { for (int i = 0; i < count; i++) buf[(*pos)++] = c; }
static void fillStr(char *buf, int *pos, const char *s) { while (*s) buf[(*pos)++] = *s++; }

Box* buildBox(struct Node *node)
{
    Box *box = (Box*)malloc(sizeof(Box));
    memset(box, 0, sizeof(Box));
    if (node == NULL) return box;

    int u = (int)strlen(node->id);

    if (node->left == NULL && node->right == NULL)
    {
        strcpy(box->lines[0], node->id);
        box->numLines = 1; box->width = u; box->rootPos = u / 2;
        return box;
    }

    if (node->right == NULL)
    {
        Box *L = buildBox(node->left);
        int n = L->width, x = L->rootPos, p = L->numLines;
        int pos = 0;
        char first[MAX_WIDTH] = {0}, second[MAX_WIDTH] = {0};

        fillSpaces(first, &pos, x + 1);
        fillChar(first, &pos, '_', n - x - 1);
        fillStr(first, &pos, node->id);
        first[pos] = '\0';

        pos = 0;
        fillSpaces(second, &pos, x);
        second[pos++] = '/';
        fillSpaces(second, &pos, n - x - 1 + u);
        second[pos] = '\0';

        strcpy(box->lines[0], first);
        strcpy(box->lines[1], second);
        for (int i = 0; i < p; i++)
        {
            strcpy(box->lines[2 + i], L->lines[i]);
            int len = (int)strlen(box->lines[2 + i]);
            fillSpaces(box->lines[2 + i], &len, u);
            box->lines[2 + i][len] = '\0';
        }
        box->numLines = p + 2; box->width = n + u; box->rootPos = n + u / 2;
        free(L);
        return box;
    }

    if (node->left == NULL)
    {
        Box *R = buildBox(node->right);
        int m = R->width, y = R->rootPos, q = R->numLines;
        int pos = 0;
        char first[MAX_WIDTH] = {0}, second[MAX_WIDTH] = {0};

        fillStr(first, &pos, node->id);
        fillChar(first, &pos, '_', y);
        fillSpaces(first, &pos, m - y);
        first[pos] = '\0';

        pos = 0;
        fillSpaces(second, &pos, u + y);
        second[pos++] = '\\';
        fillSpaces(second, &pos, m - y - 1);
        second[pos] = '\0';

        strcpy(box->lines[0], first);
        strcpy(box->lines[1], second);
        for (int i = 0; i < q; i++)
        {
            char tmp[MAX_WIDTH] = {0};
            int p2 = 0;
            fillSpaces(tmp, &p2, u);
            fillStr(tmp, &p2, R->lines[i]);
            tmp[p2] = '\0';
            strcpy(box->lines[2 + i], tmp);
        }
        box->numLines = q + 2; box->width = m + u; box->rootPos = u / 2;
        free(R);
        return box;
    }

    {
        Box *L = buildBox(node->left);
        Box *R = buildBox(node->right);
        int n = L->width, x = L->rootPos, p = L->numLines;
        int m = R->width, y = R->rootPos, q = R->numLines;
        int pos = 0;
        char first[MAX_WIDTH] = {0}, second[MAX_WIDTH] = {0};

        fillSpaces(first, &pos, x + 1);
        fillChar(first, &pos, '_', n - x - 1);
        fillStr(first, &pos, node->id);
        fillChar(first, &pos, '_', y);
        fillSpaces(first, &pos, m - y);
        first[pos] = '\0';

        pos = 0;
        fillSpaces(second, &pos, x);
        second[pos++] = '/';
        fillSpaces(second, &pos, n - x - 1 + u + y);
        second[pos++] = '\\';
        fillSpaces(second, &pos, m - y - 1);
        second[pos] = '\0';

        strcpy(box->lines[0], first);
        strcpy(box->lines[1], second);

        int maxLines = (p > q) ? p : q;
        for (int i = 0; i < maxLines; i++)
        {
            char tmp[MAX_WIDTH] = {0};
            int p2 = 0;
            const char *lline = (i < p) ? L->lines[i] : "";
            const char *rline = (i < q) ? R->lines[i] : "";
            fillStr(tmp, &p2, lline);
            fillSpaces(tmp, &p2, n - (int)strlen(lline));
            fillSpaces(tmp, &p2, u);
            fillStr(tmp, &p2, rline);
            tmp[p2] = '\0';
            strcpy(box->lines[2 + i], tmp);
        }
        box->numLines = maxLines + 2; box->width = n + m + u; box->rootPos = n + u / 2;
        free(L); free(R);
        return box;
    }
}

void printTree2D(struct Node *root)
{
    Box *box = buildBox(root);
    for (int i = 0; i < box->numLines; i++)
    {
        char *line = box->lines[i];
        int len = (int)strlen(line);
        while (len > 0 && line[len - 1] == ' ') len--;
        line[len] = '\0';
        printf("%s\n", line);
    }
    free(box);
}

/* ============================================================
   MAIN PROGRAM
   ============================================================ */
int main(void)
{
    char ids[][ID_LEN] = {"A102", "A25", "A7", "B100", "B12", "A120", "B3", "A45"};
    int n = 8;

    printf("============================================\n");
    printf("   GOVERNMENT IDENTIFICATION DATABASE (BST)\n");
    printf("============================================\n\n");

    printf("Insertion Order:\n");
    for (int i = 0; i < n; i++) printf("%s ", ids[i]);
    printf("\n");

    printf("\n--------------------------------------------\n");
    printf("a) BUILDING THE BST (insertion trace)\n");
    printf("--------------------------------------------\n");

    struct Node *root = NULL;
    for (int i = 0; i < n; i++)
    {
        long before = nodeCompareCount;
        printf("\nInsert \"%s\":\n", ids[i]);
        root = insert(root, ids[i], 1);
        printf("  [%ld comparison%s to place \"%s\"]\n", nodeCompareCount - before,
               (nodeCompareCount - before) == 1 ? "" : "s", ids[i]);
    }

    printf("\nInorder Traversal of BST (sorted order):\n");
    inorder(root);
    printf("\n");

    int h = height(root);
    printf("\nBST Height (levels): %d\n", h);
    printf("Total nodes: %d   Leaf nodes: %d\n", countNodes(root), countLeaves(root));
    printf("Theoretical best height for n=%d : 4 (ceil(log2(n+1)))\n", n);
    printf("Theoretical worst height for n=%d: %d\n", n, n);

    printf("\nTree Structure (indented view):\n\n");
    printTree(root, "", 1, 0, "");

    printf("\nTree Structure (top-down \"/  \\\" view):\n\n");
    printTree2D(root);

    /* ---------------- Part (b) ---------------- */
    printf("\n============================================\n");
    printf("b) BST SEARCH vs LINEAR SEARCH COMPARISON\n");
    printf("============================================\n\n");
    printf("%-8s %-7s %-10s %-10s %-10s %-10s\n",
           "ID", "Depth", "BST-Node", "BST-Char", "Lin-Node", "Lin-Char");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        nodeCompareCount = 0; charCompareCount = 0;
        bstSearch(root, ids[i]);
        long bstNode = nodeCompareCount, bstChar = charCompareCount;

        nodeCompareCount = 0; charCompareCount = 0;
        linearSearch(ids, n, ids[i]);
        long linNode = nodeCompareCount, linChar = charCompareCount;

        printf("%-8s %-7d %-10ld %-10ld %-10ld %-10ld\n",
               ids[i], depthOf(root, ids[i], 1), bstNode, bstChar, linNode, linChar);
    }

    char missing[ID_LEN] = "C99";
    nodeCompareCount = 0; charCompareCount = 0;
    bstSearch(root, missing);
    long bstNodeMiss = nodeCompareCount, bstCharMiss = charCompareCount;

    nodeCompareCount = 0; charCompareCount = 0;
    linearSearch(ids, n, missing);
    long linNodeMiss = nodeCompareCount, linCharMiss = charCompareCount;

    printf("%-8s %-7s %-10ld %-10ld %-10ld %-10ld   (not found)\n",
           missing, "-", bstNodeMiss, bstCharMiss, linNodeMiss, linCharMiss);

    /* ---------------- Part (c) ---------------- */
    printf("\n============================================\n");
    printf("c) EFFECT OF INSERTION ORDER ON BST HEIGHT\n");
    printf("============================================\n\n");

    char sortedIds[][ID_LEN] = {"A102", "A120", "A25", "A45", "A7", "B100", "B12", "B3"};
    struct Node *sortedRoot = NULL;
    for (int i = 0; i < n; i++) sortedRoot = insert(sortedRoot, sortedIds[i], 0);

    char balancedOrder[][ID_LEN] = {"A45", "A120", "A102", "A25", "B100", "A7", "B12", "B3"};
    struct Node *balancedRoot = NULL;
    for (int i = 0; i < n; i++) balancedRoot = insert(balancedRoot, balancedOrder[i], 0);

    printf("Given order (as in the question)        -> height = %d\n", h);
    printf("Sorted order (ascending)                -> height = %d  (worst case, skewed)\n", height(sortedRoot));
    printf("Balanced insertion order (median-first)  -> height = %d  (best case, balanced)\n", height(balancedRoot));

    /* ---------------- Optional interactive search ---------------- */
    printf("\n--------------------------------------------\n");
    printf("Enter an ID to search (or press Ctrl+D to skip): ");
    char key[ID_LEN];
    if (scanf("%19s", key) == 1)
    {
        nodeCompareCount = 0; charCompareCount = 0;
        int foundBst = bstSearch(root, key);
        long bstN = nodeCompareCount;

        nodeCompareCount = 0; charCompareCount = 0;
        int foundLin = linearSearch(ids, n, key);
        long linN = nodeCompareCount;

        printf("\nBST Search Result   : %s %s (Comparisons: %ld)\n", key, foundBst ? "Found" : "Not Found", bstN);
        printf("Linear Search Result: %s %s (Comparisons: %ld)\n", key, foundLin ? "Found" : "Not Found", linN);

        printf("\n--- Performance Verdict ---\n");
        if (bstN < linN) printf("BST Search performed better for this key.\n");
        else if (linN < bstN) printf("Linear Search performed better for this key.\n");
        else printf("Both methods used the same number of comparisons.\n");
    }

    freeTree(root);
    freeTree(sortedRoot);
    freeTree(balancedRoot);

    return 0;
}
