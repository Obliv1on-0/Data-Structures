/*
 * Binomial Heap
 *
 * ADT:
 *   makeHeap(H)               - create empty heap               O(1)
 *   link(y, z)                - make root y a child of root z   O(1)
 *   mergeRoots(H1, H2)        - merge root lists by degree      O(log n)
 *   unionHeaps(H1, H2)        - merge two heaps                 O(log n)
 *   insert(H, x)              - add key                         O(log n)
 *   minimum(H)                - smallest key                    O(log n)
 *   extractMin(H)             - remove and return smallest      O(log n)
 *   decreaseKey(H, node, k)   - lower a key                     O(log n)
 *   deleteNode(H, node)       - remove a node                   O(log n)
 *   display(H)                - print every tree                O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define ERR INT_MIN

typedef struct BinNode {
  int key;
  int degree;
  struct BinNode *parent;
  struct BinNode *child;
  struct BinNode *sibling;
} BinNode;

typedef struct BinHeap {
  BinNode *head;
} BinHeap;

BinHeap heap;

void displayMenu();
void makeHeap();
BinNode* createNode(int key);
BinNode* link(BinNode *y, BinNode *z);
BinNode* mergeRoots(BinNode *h1, BinNode *h2);
BinNode* unionHeaps(BinNode *h1, BinNode *h2);
void insertKey();
int minimum();
int extractMin();
void deleteKey();
void display();
void displayTree(BinNode *node, int level);
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
        "\n2. minimum()"
        "\n3. extractMin()"
        "\n4. deleteKey()"
        "\n5. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void makeHeap() {
  printf("\n----- makeHeap() -----");
  heap.head = NULL;
  printf("\n[SUCCESS] Initialized Binomial Heap\n");
}

BinNode* createNode(int key) {
  BinNode *n = (BinNode*) malloc(sizeof(BinNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->key = key;
  n->degree = 0;
  n->parent = NULL;
  n->child = NULL;
  n->sibling = NULL;
  return n;
}

/* y and z are roots of equal degree; y->key >= z->key, y becomes child of z */
BinNode* link(BinNode *y, BinNode *z) {
  y->parent = z;
  y->sibling = z->child;
  z->child = y;
  z->degree++;
  return z;
}

BinNode* mergeRoots(BinNode *h1, BinNode *h2) {
  if (h1 == NULL) return h2;
  if (h2 == NULL) return h1;

  BinNode *head = NULL, *tail = NULL;
  BinNode *a = h1, *b = h2;

  while (a != NULL && b != NULL) {
    BinNode *chosen;
    if (a->degree <= b->degree) { chosen = a; a = a->sibling; }
    else { chosen = b; b = b->sibling; }

    if (head == NULL) { head = chosen; tail = chosen; }
    else { tail->sibling = chosen; tail = chosen; }
  }
  tail->sibling = (a != NULL) ? a : b;
  return head;
}

BinNode* unionHeaps(BinNode *h1, BinNode *h2) {
  BinNode *newHead = mergeRoots(h1, h2);
  if (newHead == NULL) return NULL;

  BinNode *prev = NULL, *x = newHead, *next = x->sibling;

  while (next != NULL) {
    if (x->degree != next->degree ||
        (next->sibling != NULL && next->sibling->degree == x->degree)) {
      prev = x;
      x = next;
    } else if (x->key <= next->key) {
      x->sibling = next->sibling;
      x = link(next, x);
    } else {
      if (prev == NULL) newHead = next;
      else prev->sibling = next;
      x = link(x, next);
    }
    next = x->sibling;
  }
  return newHead;
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
  BinNode *n = createNode(k);
  heap.head = unionHeaps(heap.head, n);
  printf("\n[SUCCESS] Inserted key: %d\n", k);
}

int minimum() {
  printf("\n----- minimum() -----");
  if (heap.head == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return ERR;
  }
  BinNode *best = heap.head;
  BinNode *cur = heap.head->sibling;
  while (cur != NULL) {
    if (cur->key < best->key) best = cur;
    cur = cur->sibling;
  }
  printf("\n[SUCCESS] Minimum key: %d\n", best->key);
  return best->key;
}

int extractMin() {
  printf("\n----- extractMin() -----");
  if (heap.head == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return ERR;
  }

  /* find minimum root and its previous node */
  BinNode *prevMin = NULL, *minNode = heap.head;
  BinNode *prev = NULL, *cur = heap.head;
  while (cur != NULL) {
    if (cur->key < minNode->key) { minNode = cur; prevMin = prev; }
    prev = cur;
    cur = cur->sibling;
  }

  /* unlink minNode from root list */
  if (prevMin == NULL) heap.head = minNode->sibling;
  else prevMin->sibling = minNode->sibling;

  /* reverse minNode's child list to form a new root list */
  BinNode *childList = NULL;
  BinNode *child = minNode->child;
  while (child != NULL) {
    BinNode *next = child->sibling;
    child->parent = NULL;
    child->sibling = childList;
    childList = child;
    child = next;
  }

  heap.head = unionHeaps(heap.head, childList);

  int minKey = minNode->key;
  free(minNode);
  printf("\n[SUCCESS] Extracted Minimum: %d\n", minKey);
  return minKey;
}

void deleteKey() {
  printf("\n----- deleteKey() -----");
  if (heap.head == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return;
  }
  int k;
  printf("\nEnter key to delete: ");
  scanf("%d", &k);
  flushStdin();

  /* find the node with key k (simple linear search over all trees) */
  BinNode *target = NULL;
  BinNode *stackArr[1000];
  int top = -1;
  BinNode *cur = heap.head;
  while (cur != NULL) { stackArr[++top] = cur; cur = cur->sibling; }
  while (top >= 0 && target == NULL) {
    BinNode *node = stackArr[top--];
    if (node->key == k) { target = node; break; }
    BinNode *c = node->child;
    while (c != NULL) { stackArr[++top] = c; c = c->sibling; }
  }

  if (target == NULL) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }

  /* decreaseKey(target, -infinity), bubbling it to a root, then extractMin */
  target->key = INT_MIN;
  BinNode *y = target;
  BinNode *z = y->parent;
  while (z != NULL && y->key < z->key) {
    int tmp = y->key; y->key = z->key; z->key = tmp;
    y = z;
    z = y->parent;
  }
  extractMin();
  printf("\n[SUCCESS] Deleted key %d\n", k);
}

void displayTree(BinNode *node, int level) {
  if (node == NULL) return;
  for (int i = 0; i < level; i++) printf("    ");
  printf("%d\n", node->key);
  displayTree(node->child, level + 1);
  displayTree(node->sibling, level);
}

void display() {
  printf("\n----- display() -----");
  if (heap.head == NULL) {
    printf("\n[ERROR] Heap is Empty\n");
    return;
  }
  printf("\nBinomial Trees (root list):\n");
  BinNode *cur = heap.head;
  while (cur != NULL) {
    printf("-- Tree of degree %d --\n", cur->degree);
    displayTree(cur, 0);
    cur = cur->sibling;
  }
  printf("[SUCCESS] Displayed all trees\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
