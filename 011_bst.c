/*
 * Binary Search Tree (BST)
 *
 * ADT:
 *   insert(r, k)       - insert key (ignore duplicates)   O(h)
 *   search(r, k)       - find node with key k             O(h)
 *   findMin(r), findMax(r) - smallest / largest key       O(h)
 *   delete(r, k)       - remove key, keep BST property    O(h)
 *   successor(r, k)    - next larger key                  O(h)
 *   predecessor(r, k)  - next smaller key                 O(h)
 *   height(r)          - height of tree                   O(n)
 *   inorder/preorder/postorder(r)                         O(n)
 *   (h = height: O(log n) if balanced, O(n) if skewed)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define ERR INT_MIN

typedef struct TNode {
  int key;
  struct TNode *left;
  struct TNode *right;
} TNode;

TNode *root = NULL;

void displayMenu();
TNode* createNode(int key);
TNode* insert(TNode *t, int k);
TNode* search(TNode *t, int k);
TNode* findMin(TNode *t);
TNode* findMax(TNode *t);
TNode* deleteKey(TNode *t, int k);
TNode* successor(TNode *rt, int k);
TNode* predecessor(TNode *rt, int k);
int height(TNode *t);
void inorder(TNode *t);
void preorder(TNode *t);
void postorder(TNode *t);
void insertMenu();
void deleteMenu();
void searchMenu();
void minMaxMenu();
void successorMenu();
void predecessorMenu();
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
      case 4: minMaxMenu(); break;
      case 5: successorMenu(); break;
      case 6: predecessorMenu(); break;
      case 7: printf("\n[SUCCESS] Height: %d\n", height(root)); break;
      case 8: display(); break;
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
        "\n4. findMin()/findMax()"
        "\n5. successor()"
        "\n6. predecessor()"
        "\n7. height()"
        "\n8. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

TNode* createNode(int key) {
  TNode *n = (TNode*) malloc(sizeof(TNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->key = key;
  n->left = NULL;
  n->right = NULL;
  return n;
}

TNode* insert(TNode *t, int k) {
  if (t == NULL) return createNode(k);
  if (k < t->key) t->left = insert(t->left, k);
  else if (k > t->key) t->right = insert(t->right, k);
  return t;
}

TNode* search(TNode *t, int k) {
  if (t == NULL || t->key == k) return t;
  if (k < t->key) return search(t->left, k);
  return search(t->right, k);
}

TNode* findMin(TNode *t) {
  while (t != NULL && t->left != NULL) t = t->left;
  return t;
}

TNode* findMax(TNode *t) {
  while (t != NULL && t->right != NULL) t = t->right;
  return t;
}

TNode* deleteKey(TNode *t, int k) {
  if (t == NULL) return NULL;
  if (k < t->key) {
    t->left = deleteKey(t->left, k);
  } else if (k > t->key) {
    t->right = deleteKey(t->right, k);
  } else {
    if (t->left == NULL) {
      TNode *tmp = t->right;
      free(t);
      return tmp;
    } else if (t->right == NULL) {
      TNode *tmp = t->left;
      free(t);
      return tmp;
    } else {
      TNode *s = findMin(t->right);
      t->key = s->key;
      t->right = deleteKey(t->right, s->key);
    }
  }
  return t;
}

TNode* successor(TNode *rt, int k) {
  TNode *succ = NULL;
  TNode *cur = rt;
  while (cur != NULL) {
    if (k < cur->key) { succ = cur; cur = cur->left; }
    else cur = cur->right;
  }
  return succ;
}

TNode* predecessor(TNode *rt, int k) {
  TNode *pred = NULL;
  TNode *cur = rt;
  while (cur != NULL) {
    if (k > cur->key) { pred = cur; cur = cur->right; }
    else cur = cur->left;
  }
  return pred;
}

int height(TNode *t) {
  if (t == NULL) return -1;
  int lh = height(t->left);
  int rh = height(t->right);
  return 1 + ((lh > rh) ? lh : rh);
}

void inorder(TNode *t) {
  if (t == NULL) return;
  inorder(t->left);
  printf("%d ", t->key);
  inorder(t->right);
}

void preorder(TNode *t) {
  if (t == NULL) return;
  printf("%d ", t->key);
  preorder(t->left);
  preorder(t->right);
}

void postorder(TNode *t) {
  if (t == NULL) return;
  postorder(t->left);
  postorder(t->right);
  printf("%d ", t->key);
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

void minMaxMenu() {
  printf("\n----- findMin()/findMax() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  printf("\n[SUCCESS] Min: %d, Max: %d\n", findMin(root)->key, findMax(root)->key);
}

void successorMenu() {
  printf("\n----- successor() -----");
  int k;
  printf("\nEnter key: ");
  scanf("%d", &k);
  flushStdin();
  TNode *s = successor(root, k);
  if (s != NULL) printf("\n[SUCCESS] Successor of %d is %d\n", k, s->key);
  else printf("\n[ERROR] No successor for %d\n", k);
}

void predecessorMenu() {
  printf("\n----- predecessor() -----");
  int k;
  printf("\nEnter key: ");
  scanf("%d", &k);
  flushStdin();
  TNode *p = predecessor(root, k);
  if (p != NULL) printf("\n[SUCCESS] Predecessor of %d is %d\n", k, p->key);
  else printf("\n[ERROR] No predecessor for %d\n", k);
}

void display() {
  printf("\n----- display() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  printf("\nInorder:   "); inorder(root);
  printf("\nPreorder:  "); preorder(root);
  printf("\nPostorder: "); postorder(root);
  printf("\n[SUCCESS] Displayed all traversals\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
