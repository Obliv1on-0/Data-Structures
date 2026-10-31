/*
 * Binary Tree (unordered, level-order insertion)
 *
 * ADT:
 *   insertLevelOrder(r, k)  - insert at first free position (level-wise)   O(n)
 *   preorder(r)             - Root, Left, Right                            O(n)
 *   inorder(r)              - Left, Root, Right                            O(n)
 *   postorder(r)            - Left, Right, Root                            O(n)
 *   levelOrder(r)           - level by level using a queue                 O(n)
 *   height(r)               - height of the tree                           O(n)
 *   countNodes(r)           - total nodes                                  O(n)
 *   countLeaves(r)          - nodes with no children                       O(n)
 *   search(r, k)            - find a key (unordered tree)                  O(n)
 *   mirror(r)               - swap left and right everywhere               O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

typedef struct TNode {
  int key;
  struct TNode *left;
  struct TNode *right;
} TNode;

TNode *root = NULL;

void displayMenu();
TNode* createNode(int k);
TNode* insertLevelOrder(TNode *rt, int k);
void preorder(TNode *t);
void inorder(TNode *t);
void postorder(TNode *t);
void levelOrder(TNode *rt);
int height(TNode *t);
int countNodes(TNode *t);
int countLeaves(TNode *t);
TNode* search(TNode *t, int k);
void mirror(TNode *t);
void insertMenu();
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
      case 2: searchMenu(); break;
      case 3: printf("\n[SUCCESS] Height: %d\n", height(root)); break;
      case 4: printf("\n[SUCCESS] Total nodes: %d\n", countNodes(root)); break;
      case 5: printf("\n[SUCCESS] Leaf nodes: %d\n", countLeaves(root)); break;
      case 6: mirror(root); printf("\n[SUCCESS] Mirrored the tree\n"); break;
      case 7: display(); break;
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
        "\n1. insertLevelOrder()"
        "\n2. search()"
        "\n3. height()"
        "\n4. countNodes()"
        "\n5. countLeaves()"
        "\n6. mirror()"
        "\n7. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

TNode* createNode(int k) {
  TNode *n = (TNode*) malloc(sizeof(TNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->key = k;
  n->left = NULL;
  n->right = NULL;
  return n;
}

TNode* insertLevelOrder(TNode *rt, int k) {
  if (rt == NULL) return createNode(k);

  TNode *queue[MAX];
  int front = 0, rear = 0;
  queue[rear++] = rt;

  while (front < rear) {
    TNode *t = queue[front++];
    if (t->left == NULL) {
      t->left = createNode(k);
      return rt;
    } else {
      queue[rear++] = t->left;
    }
    if (t->right == NULL) {
      t->right = createNode(k);
      return rt;
    } else {
      queue[rear++] = t->right;
    }
  }
  return rt;
}

void preorder(TNode *t) {
  if (t == NULL) return;
  printf("%d ", t->key);
  preorder(t->left);
  preorder(t->right);
}

void inorder(TNode *t) {
  if (t == NULL) return;
  inorder(t->left);
  printf("%d ", t->key);
  inorder(t->right);
}

void postorder(TNode *t) {
  if (t == NULL) return;
  postorder(t->left);
  postorder(t->right);
  printf("%d ", t->key);
}

void levelOrder(TNode *rt) {
  if (rt == NULL) return;
  TNode *queue[MAX];
  int front = 0, rear = 0;
  queue[rear++] = rt;
  while (front < rear) {
    TNode *t = queue[front++];
    printf("%d ", t->key);
    if (t->left != NULL) queue[rear++] = t->left;
    if (t->right != NULL) queue[rear++] = t->right;
  }
}

int height(TNode *t) {
  if (t == NULL) return -1;
  int lh = height(t->left);
  int rh = height(t->right);
  return 1 + ((lh > rh) ? lh : rh);
}

int countNodes(TNode *t) {
  if (t == NULL) return 0;
  return 1 + countNodes(t->left) + countNodes(t->right);
}

int countLeaves(TNode *t) {
  if (t == NULL) return 0;
  if (t->left == NULL && t->right == NULL) return 1;
  return countLeaves(t->left) + countLeaves(t->right);
}

TNode* search(TNode *t, int k) {
  if (t == NULL) return NULL;
  if (t->key == k) return t;
  TNode *res = search(t->left, k);
  if (res != NULL) return res;
  return search(t->right, k);
}

void mirror(TNode *t) {
  if (t == NULL) return;
  TNode *tmp = t->left;
  t->left = t->right;
  t->right = tmp;
  mirror(t->left);
  mirror(t->right);
}

void insertMenu() {
  printf("\n----- insertLevelOrder() -----");
  int k;
  printf("\nEnter key: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  root = insertLevelOrder(root, k);
  printf("\n[SUCCESS] Inserted key %d\n", k);
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
  printf("\nPreorder:   "); preorder(root);
  printf("\nInorder:    "); inorder(root);
  printf("\nPostorder:  "); postorder(root);
  printf("\nLevelOrder: "); levelOrder(root);
  printf("\n[SUCCESS] Displayed all traversals\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
