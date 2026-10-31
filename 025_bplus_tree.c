/*
 * B+ Tree (order M)
 *
 * ADT:
 *   createNode(isLeaf)           - allocate empty node               O(1)
 *   findLeaf(T, k)               - leaf where key k belongs          O(log n)
 *   search(T, k)                 - return value or ERR               O(log n)
 *   rangeSearch(T, lo, hi)       - all keys between lo and hi        O(log n + r)
 *   insert(T, k, v)              - insert record                     O(log n)
 *   splitLeaf(T, leaf)           - split overflowing leaf            O(M)
 *   splitInternal(T, node)       - split overflowing internal node   O(M)
 *   insertIntoParent(T, L, k, R) - add separator to parent           O(log n)
 *   delete(T, k)                 - delete record                     O(log n)
 *   display(T)                   - print leaves in order             O(n)
 *
 * NOTE: delete() here removes the key from its leaf and fixes the
 * parent separator if needed, following the exam algorithm. Full
 * borrow/merge rebalancing of underflowing nodes is intentionally
 * left out to keep the program exam-simple; the tree remains correct
 * for search, range search and display after any sequence of deletes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define M 4
#define ERR INT_MIN

typedef struct BPNode {
  int isLeaf;
  int n;
  int keys[M];
  struct BPNode *child[M + 1];
  int value[M];
  struct BPNode *next;
  struct BPNode *parent;
} BPNode;

BPNode *root = NULL;

void displayMenu();
BPNode* createNode(int isLeaf);
BPNode* findLeaf(int k);
int searchKey(int k);
void rangeSearch();
void insertIntoParent(BPNode *L, int k, BPNode *R);
void splitLeaf(BPNode *L);
void splitInternal(BPNode *P);
void insertKey();
void deleteKey();
void display();
static void flushStdin();

int main() {
  printf("\n----- main() -----");
  int choice;
  do {
    displayMenu();
    if (scanf("%d", &choice) != 1) {
      printf("\n[ERROR] Invalid input choice.\n");
      flushStdin();
      continue;
    }
    flushStdin();
    switch (choice) {
      case 1: insertKey(); break;
      case 2: {
        int k;
        printf("\nEnter key to search: ");
        scanf("%d", &k);
        flushStdin();
        int v = searchKey(k);
        if (v != ERR) printf("\n[SUCCESS] Key %d found with value %d\n", k, v);
        else printf("\n[ERROR] Key %d not found\n", k);
        break;
      }
      case 3: rangeSearch(); break;
      case 4: deleteKey(); break;
      case 5: display(); break;
      case 0:
        printf("\n[INFO] Exiting program...\n");
        break;
      default:
        printf("\n[ERROR] Invalid menu choice.\n");
    }
  } while (choice != 0);
  return 0;
}

void displayMenu() {
  printf("\n-----  displayMenu() -----"
        "\n1. insert()"
        "\n2. search()"
        "\n3. rangeSearch()"
        "\n4. delete()"
        "\n5. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

BPNode* createNode(int isLeaf) {
  BPNode *n = (BPNode*) malloc(sizeof(BPNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->isLeaf = isLeaf;
  n->n = 0;
  n->next = NULL;
  n->parent = NULL;
  for (int i = 0; i <= M; i++) n->child[i] = NULL;
  return n;
}

BPNode* findLeaf(int k) {
  if (root == NULL) return NULL;
  BPNode *cur = root;
  while (!cur->isLeaf) {
    int i = 0;
    while (i < cur->n && k >= cur->keys[i]) i++;
    cur = cur->child[i];
  }
  return cur;
}

int searchKey(int k) {
  BPNode *leaf = findLeaf(k);
  if (leaf == NULL) return ERR;
  for (int i = 0; i < leaf->n; i++) {
    if (leaf->keys[i] == k) return leaf->value[i];
  }
  return ERR;
}

void rangeSearch() {
  printf("\n----- rangeSearch() -----");
  int lo, hi;
  printf("\nEnter lo and hi: ");
  scanf("%d %d", &lo, &hi);
  flushStdin();
  BPNode *leaf = findLeaf(lo);
  if (leaf == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  printf("\nKeys in [%d, %d]: ", lo, hi);
  bool found = false;
  while (leaf != NULL) {
    for (int i = 0; i < leaf->n; i++) {
      if (leaf->keys[i] >= lo && leaf->keys[i] <= hi) {
        printf("%d ", leaf->keys[i]);
        found = true;
      } else if (leaf->keys[i] > hi) {
        if (!found) printf("(none)");
        printf("\n[SUCCESS] Range search complete\n");
        return;
      }
    }
    leaf = leaf->next;
  }
  if (!found) printf("(none)");
  printf("\n[SUCCESS] Range search complete\n");
}

void insertIntoParent(BPNode *L, int k, BPNode *R) {
  if (L->parent == NULL) {
    BPNode *newRoot = createNode(0);
    newRoot->keys[0] = k;
    newRoot->child[0] = L;
    newRoot->child[1] = R;
    newRoot->n = 1;
    L->parent = newRoot;
    R->parent = newRoot;
    root = newRoot;
    return;
  }

  BPNode *P = L->parent;
  int i = 0;
  while (P->child[i] != L) i++;

  for (int j = P->n; j > i; j--) {
    P->keys[j] = P->keys[j - 1];
    P->child[j + 1] = P->child[j];
  }
  P->keys[i] = k;
  P->child[i + 1] = R;
  R->parent = P;
  P->n++;

  if (P->n == M) splitInternal(P);
}

void splitLeaf(BPNode *L) {
  BPNode *R = createNode(1);
  int mid = M / 2;

  for (int i = mid; i < L->n; i++) {
    R->keys[i - mid] = L->keys[i];
    R->value[i - mid] = L->value[i];
  }
  R->n = L->n - mid;
  L->n = mid;

  R->next = L->next;
  L->next = R;
  R->parent = L->parent;

  int sep = R->keys[0];
  insertIntoParent(L, sep, R);
}

void splitInternal(BPNode *P) {
  BPNode *Q = createNode(0);
  int mid = M / 2;
  int midKey = P->keys[mid];

  for (int i = mid + 1; i < P->n; i++) Q->keys[i - mid - 1] = P->keys[i];
  for (int i = mid + 1; i <= P->n; i++) {
    Q->child[i - mid - 1] = P->child[i];
    if (Q->child[i - mid - 1] != NULL) Q->child[i - mid - 1]->parent = Q;
  }
  Q->n = P->n - mid - 1;
  P->n = mid;
  Q->parent = P->parent;

  insertIntoParent(P, midKey, Q);
}

void insertKey() {
  printf("\n----- insert() -----");
  int k, v;
  printf("\nEnter key: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  printf("\nEnter value: ");
  if (scanf("%d", &v) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();

  if (root == NULL) {
    root = createNode(1);
    root->keys[0] = k;
    root->value[0] = v;
    root->n = 1;
    printf("\n[SUCCESS] Inserted key %d\n", k);
    return;
  }

  BPNode *leaf = findLeaf(k);
  for (int i = 0; i < leaf->n; i++) {
    if (leaf->keys[i] == k) {
      leaf->value[i] = v;
      printf("\n[SUCCESS] Updated key %d with value %d\n", k, v);
      return;
    }
  }

  int i = leaf->n - 1;
  while (i >= 0 && leaf->keys[i] > k) {
    leaf->keys[i + 1] = leaf->keys[i];
    leaf->value[i + 1] = leaf->value[i];
    i--;
  }
  leaf->keys[i + 1] = k;
  leaf->value[i + 1] = v;
  leaf->n++;

  if (leaf->n == M) splitLeaf(leaf);
  printf("\n[SUCCESS] Inserted key %d\n", k);
}

void fixParentSeparator(BPNode *leaf) {
  if (leaf->parent == NULL || leaf->n == 0) return;
  BPNode *P = leaf->parent;
  int i = 0;
  while (P->child[i] != leaf) i++;
  if (i > 0) P->keys[i - 1] = leaf->keys[0];
}

void deleteKey() {
  printf("\n----- delete() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  int k;
  printf("\nEnter key to delete: ");
  scanf("%d", &k);
  flushStdin();

  BPNode *leaf = findLeaf(k);
  int pos = -1;
  for (int i = 0; i < leaf->n; i++) if (leaf->keys[i] == k) { pos = i; break; }
  if (pos == -1) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }

  for (int i = pos + 1; i < leaf->n; i++) {
    leaf->keys[i - 1] = leaf->keys[i];
    leaf->value[i - 1] = leaf->value[i];
  }
  leaf->n--;

  fixParentSeparator(leaf);

  if (leaf == root && leaf->n == 0) {
    free(root);
    root = NULL;
  }
  printf("\n[SUCCESS] Deleted key %d\n", k);
}

void display() {
  printf("\n----- display() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  BPNode *leaf = root;
  while (!leaf->isLeaf) leaf = leaf->child[0];

  printf("\nLeaves (in order): ");
  while (leaf != NULL) {
    printf("[ ");
    for (int i = 0; i < leaf->n; i++) printf("%d:%d ", leaf->keys[i], leaf->value[i]);
    printf("] -> ");
    leaf = leaf->next;
  }
  printf("NULL");
  printf("\n[SUCCESS] Displayed all leaves\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
