/*
 * Fibonacci Heap
 *
 * ADT:
 *   makeHeap(H)                - create empty heap              O(1)
 *   insert(H, x)               - add key                        O(1)
 *   minimum(H)                 - smallest key                   O(1)
 *   unionHeaps(H1, H2)         - merge two heaps                O(1)
 *   extractMin(H)              - remove and return smallest     O(log n) amortized
 *   decreaseKey(H, x, k)       - lower a key                    O(1) amortized
 *   deleteNode(H, x)           - remove a node                  O(log n) amortized
 *   display(H)                 - print root list and children   O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define ERR INT_MIN
#define MAXDEG 64

typedef struct FibNode {
  int key;
  int degree;
  int mark;
  struct FibNode *parent;
  struct FibNode *child;
  struct FibNode *left;
  struct FibNode *right;
} FibNode;

typedef struct FibHeap {
  FibNode *min;
  int n;
} FibHeap;

FibHeap heap;

void displayMenu();
void makeHeap();
FibNode* createNode(int key);
void addToRootList(FibNode *x);
void removeFromList(FibNode *x);
void insertKey();
int minimum();
void heapLink(FibNode *y, FibNode *x);
void consolidate();
int extractMin();
void cut(FibNode *x, FibNode *y);
void cascadingCut(FibNode *y);
void decreaseKeyOp(FibNode *x, int k);
void decreaseKeyMenu();
FibNode* findNode(FibNode *start, int k);
void deleteKey();
void display();
void displayTree(FibNode *start, int level);
static void flushStdin();

int main() {
  printf("\n----- main() -----");
  makeHeap();
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
      case 2: minimum(); break;
      case 3: extractMin(); break;
      case 4: decreaseKeyMenu(); break;
      case 5: deleteKey(); break;
      case 6: display(); break;
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
        "\n2. minimum()"
        "\n3. extractMin()"
        "\n4. decreaseKey()"
        "\n5. deleteKey()"
        "\n6. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void makeHeap() {
  printf("\n----- makeHeap() -----");
  heap.min = NULL;
  heap.n = 0;
  printf("\n[SUCCESS] Initialized Fibonacci Heap\n");
}

FibNode* createNode(int key) {
  FibNode *n = (FibNode*) malloc(sizeof(FibNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->key = key;
  n->degree = 0;
  n->mark = 0;
  n->parent = NULL;
  n->child = NULL;
  n->left = n;
  n->right = n;
  return n;
}

void addToRootList(FibNode *x) {
  x->parent = NULL;
  if (heap.min == NULL) {
    x->left = x;
    x->right = x;
    heap.min = x;
  } else {
    x->right = heap.min->right;
    x->left = heap.min;
    heap.min->right->left = x;
    heap.min->right = x;
    if (x->key < heap.min->key) heap.min = x;
  }
}

void removeFromList(FibNode *x) {
  x->left->right = x->right;
  x->right->left = x->left;
}

void insertKey() {
  printf("\n----- insert() -----");
  int k;
  printf("\nEnter int data: ");
  if (scanf("%d", &k) != 1) {
    printf("\n[ERROR] Invalid integer input.\n");
    flushStdin();
    return;
  }
  flushStdin();
  FibNode *x = createNode(k);
  addToRootList(x);
  heap.n++;
  printf("\n[SUCCESS] Inserted key: %d\n", k);
}

int minimum() {
  printf("\n----- minimum() -----");
  if (heap.min == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return ERR;
  }
  printf("\n[SUCCESS] Minimum key: %d\n", heap.min->key);
  return heap.min->key;
}

/* make y a child of x (x->key <= y->key) */
void heapLink(FibNode *y, FibNode *x) {
  removeFromList(y);
  y->parent = x;
  if (x->child == NULL) {
    x->child = y;
    y->left = y;
    y->right = y;
  } else {
    y->right = x->child->right;
    y->left = x->child;
    x->child->right->left = y;
    x->child->right = y;
  }
  x->degree++;
  y->mark = 0;
}

void consolidate() {
  FibNode *A[MAXDEG];
  for (int i = 0; i < MAXDEG; i++) A[i] = NULL;

  /* collect current roots into a temporary array first */
  int cnt = 0;
  FibNode *roots[MAXDEG * 4];
  FibNode *cur = heap.min;
  if (cur != NULL) {
    FibNode *start = cur;
    do {
      roots[cnt++] = cur;
      cur = cur->right;
    } while (cur != start);
  }

  for (int i = 0; i < cnt; i++) {
    FibNode *x = roots[i];
    int d = x->degree;
    while (A[d] != NULL) {
      FibNode *y = A[d];
      if (x->key > y->key) { FibNode *tmp = x; x = y; y = tmp; }
      heapLink(y, x);
      A[d] = NULL;
      d++;
    }
    A[d] = x;
  }

  heap.min = NULL;
  for (int i = 0; i < MAXDEG; i++) {
    if (A[i] != NULL) {
      A[i]->left = A[i];
      A[i]->right = A[i];
      addToRootList(A[i]);
    }
  }
}

int extractMin() {
  printf("\n----- extractMin() -----");
  FibNode *z = heap.min;
  if (z == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return ERR;
  }

  if (z->child != NULL) {
    FibNode *c = z->child;
    FibNode *start = c;
    do {
      FibNode *next = c->right;
      addToRootList(c);
      c = next;
    } while (c != start);
  }

  removeFromList(z);
  if (z->right == z) heap.min = NULL;
  else {
    heap.min = z->right;
    consolidate();
  }

  heap.n--;
  int key = z->key;
  free(z);
  printf("\n[SUCCESS] Extracted Minimum: %d\n", key);
  return key;
}

void cut(FibNode *x, FibNode *y) {
  if (x->right == x) y->child = NULL;
  else {
    if (y->child == x) y->child = x->right;
    removeFromList(x);
  }
  y->degree--;
  addToRootList(x);
  x->mark = 0;
}

void cascadingCut(FibNode *y) {
  FibNode *z = y->parent;
  if (z != NULL) {
    if (y->mark == 0) y->mark = 1;
    else {
      cut(y, z);
      cascadingCut(z);
    }
  }
}

void decreaseKeyOp(FibNode *x, int k) {
  if (k > x->key) {
    printf("\n[ERROR] New key is greater than current key\n");
    return;
  }
  x->key = k;
  FibNode *y = x->parent;
  if (y != NULL && x->key < y->key) {
    cut(x, y);
    cascadingCut(y);
  }
  if (x->key < heap.min->key) heap.min = x;
}

FibNode* findNode(FibNode *start, int k) {
  if (start == NULL) return NULL;
  FibNode *cur = start;
  do {
    if (cur->key == k) return cur;
    FibNode *found = findNode(cur->child, k);
    if (found != NULL) return found;
    cur = cur->right;
  } while (cur != start);
  return NULL;
}

void decreaseKeyMenu() {
  printf("\n----- decreaseKey() -----");
  if (heap.min == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return;
  }
  int oldKey, newKey;
  printf("\nEnter current key value: ");
  scanf("%d", &oldKey);
  flushStdin();
  printf("\nEnter new (smaller) key value: ");
  scanf("%d", &newKey);
  flushStdin();

  FibNode *x = findNode(heap.min, oldKey);
  if (x == NULL) {
    printf("\n[ERROR] Key %d not found\n", oldKey);
    return;
  }
  decreaseKeyOp(x, newKey);
  printf("\n[SUCCESS] Decreased key %d to %d\n", oldKey, newKey);
}

void deleteKey() {
  printf("\n----- deleteKey() -----");
  if (heap.min == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return;
  }
  int k;
  printf("\nEnter key to delete: ");
  scanf("%d", &k);
  flushStdin();

  FibNode *x = findNode(heap.min, k);
  if (x == NULL) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }
  decreaseKeyOp(x, INT_MIN);
  extractMin();
  printf("\n[SUCCESS] Deleted key %d\n", k);
}

void displayTree(FibNode *start, int level) {
  if (start == NULL) return;
  FibNode *cur = start;
  do {
    for (int i = 0; i < level; i++) printf("    ");
    printf("%d\n", cur->key);
    displayTree(cur->child, level + 1);
    cur = cur->right;
  } while (cur != start);
}

void display() {
  printf("\n----- display() -----");
  if (heap.min == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return;
  }
  printf("\nFibonacci Heap trees (root list):\n");
  displayTree(heap.min, 0);
  printf("[SUCCESS] Displayed all trees\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
