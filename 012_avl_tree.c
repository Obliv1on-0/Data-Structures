/*
 * AVL Tree
 *
 * ADT:
 *   height(n)        - height stored in node (0 for NULL)   O(1)
 *   updateHeight(n)  - recompute from children              O(1)
 *   getBalance(n)    - balance factor of node               O(1)
 *   rotateRight(y)   - fixes left-heavy                     O(1)
 *   rotateLeft(x)    - fixes right-heavy                    O(1)
 *   insert(r, k)     - BST insert + rebalance               O(log n)
 *   delete(r, k)     - BST delete + rebalance               O(log n)
 *   search(r, k)     - as BST                               O(log n)
 *   findMin(r)       - leftmost node                        O(log n)
 *   inorder(r)       - sorted output                        O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct AVLNode {
  int key;
  int height;
  struct AVLNode *left;
  struct AVLNode *right;
} AVLNode;

AVLNode *root = NULL;

void displayMenu();
AVLNode* createNode(int key);
int heightOf(AVLNode *n);
void updateHeight(AVLNode *n);
int getBalance(AVLNode *n);
int maxInt(int a, int b);
AVLNode* rotateRight(AVLNode *y);
AVLNode* rotateLeft(AVLNode *x);
AVLNode* insert(AVLNode *n, int k);
AVLNode* findMin(AVLNode *n);
AVLNode* deleteKey(AVLNode *n, int k);
AVLNode* search(AVLNode *n, int k);
void inorder(AVLNode *n);
void preorder(AVLNode *n);
void insertMenu();
void deleteMenu();
void searchMenu();
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
      case 1: insertMenu(); break;
      case 2: deleteMenu(); break;
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

AVLNode* createNode(int key) {
  AVLNode *n = (AVLNode*) malloc(sizeof(AVLNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->key = key;
  n->height = 1;
  n->left = NULL;
  n->right = NULL;
  return n;
}

int heightOf(AVLNode *n) {
  if (n == NULL) return 0;
  return n->height;
}

void updateHeight(AVLNode *n) {
  n->height = 1 + maxInt(heightOf(n->left), heightOf(n->right));
}

int getBalance(AVLNode *n) {
  if (n == NULL) return 0;
  return heightOf(n->left) - heightOf(n->right);
}

int maxInt(int a, int b) {
  return (a > b) ? a : b;
}

AVLNode* rotateRight(AVLNode *y) {
  AVLNode *x = y->left;
  AVLNode *t2 = x->right;
  x->right = y;
  y->left = t2;
  updateHeight(y);
  updateHeight(x);
  return x;
}

AVLNode* rotateLeft(AVLNode *x) {
  AVLNode *y = x->right;
  AVLNode *t2 = y->left;
  y->left = x;
  x->right = t2;
  updateHeight(x);
  updateHeight(y);
  return y;
}

AVLNode* insert(AVLNode *n, int k) {
  if (n == NULL) return createNode(k);

  if (k < n->key) n->left = insert(n->left, k);
  else if (k > n->key) n->right = insert(n->right, k);
  else return n; /* duplicate */

  updateHeight(n);
  int bf = getBalance(n);

  if (bf > 1 && k < n->left->key) return rotateRight(n);
  if (bf < -1 && k > n->right->key) return rotateLeft(n);
  if (bf > 1 && k > n->left->key) {
    n->left = rotateLeft(n->left);
    return rotateRight(n);
  }
  if (bf < -1 && k < n->right->key) {
    n->right = rotateRight(n->right);
    return rotateLeft(n);
  }
  return n;
}

AVLNode* findMin(AVLNode *n) {
  while (n->left != NULL) n = n->left;
  return n;
}

AVLNode* deleteKey(AVLNode *n, int k) {
  if (n == NULL) return NULL;

  if (k < n->key) n->left = deleteKey(n->left, k);
  else if (k > n->key) n->right = deleteKey(n->right, k);
  else {
    if (n->left == NULL || n->right == NULL) {
      AVLNode *child = (n->left != NULL) ? n->left : n->right;
      free(n);
      return child;
    } else {
      AVLNode *succ = findMin(n->right);
      n->key = succ->key;
      n->right = deleteKey(n->right, succ->key);
    }
  }

  if (n == NULL) return NULL;

  updateHeight(n);
  int bf = getBalance(n);

  if (bf > 1 && getBalance(n->left) >= 0) return rotateRight(n);
  if (bf > 1 && getBalance(n->left) < 0) {
    n->left = rotateLeft(n->left);
    return rotateRight(n);
  }
  if (bf < -1 && getBalance(n->right) <= 0) return rotateLeft(n);
  if (bf < -1 && getBalance(n->right) > 0) {
    n->right = rotateRight(n->right);
    return rotateLeft(n);
  }
  return n;
}

AVLNode* search(AVLNode *n, int k) {
  if (n == NULL || n->key == k) return n;
  if (k < n->key) return search(n->left, k);
  return search(n->right, k);
}

void inorder(AVLNode *n) {
  if (n == NULL) return;
  inorder(n->left);
  printf("%d ", n->key);
  inorder(n->right);
}

void insertMenu() {
  printf("\n----- insert() -----");
  int k;
  printf("\nEnter key: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  root = insert(root, k);
  printf("\n[SUCCESS] Inserted key %d\n", k);
}

void deleteMenu() {
  printf("\n----- delete() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  int k;
  printf("\nEnter key to delete: ");
  scanf("%d", &k);
  flushStdin();
  if (search(root, k) == NULL) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }
  root = deleteKey(root, k);
  printf("\n[SUCCESS] Deleted key %d\n", k);
}

void searchMenu() {
  printf("\n----- search() -----");
  int k;
  printf("\nEnter key to search: ");
  scanf("%d", &k);
  flushStdin();
  if (search(root, k) != NULL) printf("\n[SUCCESS] Key %d found\n", k);
  else printf("\n[ERROR] Key %d not found\n", k);
}

void display() {
  printf("\n----- display() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  printf("\nInorder (sorted): ");
  inorder(root);
  printf("\n[SUCCESS] Displayed all keys\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
