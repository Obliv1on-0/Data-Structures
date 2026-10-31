/*
 * Red-Black Tree
 *
 * ADT:
 *   init(T)              - create empty tree                  O(1)
 *   rotateLeft(T, x)     - left rotation about x              O(1)
 *   rotateRight(T, x)    - right rotation about x             O(1)
 *   insert(T, k)         - insert key                         O(log n)
 *   delete(T, z)         - remove node                        O(log n)
 *   search(T, k)         - find key                           O(log n)
 *   inorder(T, x)        - sorted output with colours         O(n)
 *
 * Invariants:
 *   1. Every node is RED or BLACK
 *   2. Root is BLACK
 *   3. Leaves (nil) are BLACK
 *   4. A RED node has only BLACK children
 *   5. Every path from a node to its nil leaves has the same number
 *      of BLACK nodes
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
  int key;
  Color color;
  struct RBNode *left, *right, *parent;
} RBNode;

RBNode *root;
RBNode *nilNode;

void displayMenu();
void init();
RBNode* createNode(int key);
void rotateLeft(RBNode *x);
void rotateRight(RBNode *x);
void insertFixup(RBNode *z);
void insertKey();
void transplant(RBNode *u, RBNode *v);
RBNode* treeMinimum(RBNode *x);
void deleteFixup(RBNode *x);
RBNode* searchNode(RBNode *x, int k);
void deleteKey();
void searchMenu();
void inorder(RBNode *x);
void display();
static void flushStdin();

int main() {
  printf("\n----- main() -----");
  init();
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
      case 2: deleteKey(); break;
      case 3: searchMenu(); break;
      case 4: display(); break;
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
        "\n2. delete()"
        "\n3. search()"
        "\n4. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  nilNode = (RBNode*) malloc(sizeof(RBNode));
  nilNode->color = BLACK;
  nilNode->left = nilNode->right = nilNode->parent = nilNode;
  root = nilNode;
  printf("\n[SUCCESS] Initialized Red-Black Tree\n");
}

RBNode* createNode(int key) {
  RBNode *n = (RBNode*) malloc(sizeof(RBNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->key = key;
  n->color = RED;
  n->left = n->right = n->parent = nilNode;
  return n;
}

void rotateLeft(RBNode *x) {
  RBNode *y = x->right;
  x->right = y->left;
  if (y->left != nilNode) y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == nilNode) root = y;
  else if (x == x->parent->left) x->parent->left = y;
  else x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void rotateRight(RBNode *x) {
  RBNode *y = x->left;
  x->left = y->right;
  if (y->right != nilNode) y->right->parent = x;
  y->parent = x->parent;
  if (x->parent == nilNode) root = y;
  else if (x == x->parent->right) x->parent->right = y;
  else x->parent->left = y;
  y->right = x;
  x->parent = y;
}

void insertFixup(RBNode *z) {
  while (z->parent->color == RED) {
    if (z->parent == z->parent->parent->left) {
      RBNode *u = z->parent->parent->right;
      if (u->color == RED) {
        z->parent->color = BLACK;
        u->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          rotateLeft(z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rotateRight(z->parent->parent);
      }
    } else {
      RBNode *u = z->parent->parent->left;
      if (u->color == RED) {
        z->parent->color = BLACK;
        u->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          rotateRight(z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        rotateLeft(z->parent->parent);
      }
    }
  }
  root->color = BLACK;
}

void insertKey() {
  printf("\n----- insert() -----");
  int k;
  printf("\nEnter key: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();

  RBNode *z = createNode(k);
  RBNode *y = nilNode;
  RBNode *x = root;
  while (x != nilNode) {
    y = x;
    if (z->key < x->key) x = x->left;
    else if (z->key > x->key) x = x->right;
    else { printf("\n[ERROR] Duplicate key\n"); free(z); return; }
  }
  z->parent = y;
  if (y == nilNode) root = z;
  else if (z->key < y->key) y->left = z;
  else y->right = z;

  insertFixup(z);
  printf("\n[SUCCESS] Inserted key %d\n", k);
}

void transplant(RBNode *u, RBNode *v) {
  if (u->parent == nilNode) root = v;
  else if (u == u->parent->left) u->parent->left = v;
  else u->parent->right = v;
  v->parent = u->parent;
}

RBNode* treeMinimum(RBNode *x) {
  while (x->left != nilNode) x = x->left;
  return x;
}

void deleteFixup(RBNode *x) {
  while (x != root && x->color == BLACK) {
    if (x == x->parent->left) {
      RBNode *w = x->parent->right;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotateLeft(x->parent);
        w = x->parent->right;
      }
      if (w->left->color == BLACK && w->right->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->right->color == BLACK) {
          w->left->color = BLACK;
          w->color = RED;
          rotateRight(w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->right->color = BLACK;
        rotateLeft(x->parent);
        x = root;
      }
    } else {
      RBNode *w = x->parent->left;
      if (w->color == RED) {
        w->color = BLACK;
        x->parent->color = RED;
        rotateRight(x->parent);
        w = x->parent->left;
      }
      if (w->right->color == BLACK && w->left->color == BLACK) {
        w->color = RED;
        x = x->parent;
      } else {
        if (w->left->color == BLACK) {
          w->right->color = BLACK;
          w->color = RED;
          rotateLeft(w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = BLACK;
        w->left->color = BLACK;
        rotateRight(x->parent);
        x = root;
      }
    }
  }
  x->color = BLACK;
}

RBNode* searchNode(RBNode *x, int k) {
  while (x != nilNode && x->key != k) {
    if (k < x->key) x = x->left;
    else x = x->right;
  }
  return x;
}

void deleteKey() {
  printf("\n----- delete() -----");
  int k;
  printf("\nEnter key to delete: ");
  scanf("%d", &k);
  flushStdin();

  RBNode *z = searchNode(root, k);
  if (z == nilNode) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }

  RBNode *y = z;
  RBNode *x;
  Color yOrigColor = y->color;

  if (z->left == nilNode) {
    x = z->right;
    transplant(z, z->right);
  } else if (z->right == nilNode) {
    x = z->left;
    transplant(z, z->left);
  } else {
    y = treeMinimum(z->right);
    yOrigColor = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {
      transplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  free(z);
  if (yOrigColor == BLACK) deleteFixup(x);
  printf("\n[SUCCESS] Deleted key %d\n", k);
}

void searchMenu() {
  printf("\n----- search() -----");
  int k;
  printf("\nEnter key to search: ");
  scanf("%d", &k);
  flushStdin();
  if (searchNode(root, k) != nilNode) printf("\n[SUCCESS] Key %d found\n", k);
  else printf("\n[ERROR] Key %d not found\n", k);
}

void inorder(RBNode *x) {
  if (x == nilNode) return;
  inorder(x->left);
  printf("%d(%s) ", x->key, x->color == RED ? "R" : "B");
  inorder(x->right);
}

void display() {
  printf("\n----- display() -----");
  if (root == nilNode) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  printf("\nInorder (key, color): ");
  inorder(root);
  printf("\n[SUCCESS] Displayed all keys\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
