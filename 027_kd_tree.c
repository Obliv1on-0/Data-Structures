/*
 * k-d Tree (K = 2 dimensions)
 *
 * ADT:
 *   createNode(p)                 - allocate node with point p     O(1)
 *   insert(root, p, depth)        - insert a point                 O(log n) avg
 *   search(root, p, depth)        - is the point present           O(log n) avg
 *   delete(root, p, depth)        - remove a point                 O(log n) avg
 *   nearest(root, q, depth, best) - closest point to query q       O(log n) avg
 *   display(root)                 - print points                   O(n)
 *
 * splitting axis at depth d is d % K (x, y, x, y, ...)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define K 2

typedef struct KDNode {
  int point[K];
  struct KDNode *left, *right;
} KDNode;

KDNode *root = NULL;

void displayMenu();
KDNode* createNode(int p[K]);
KDNode* insert(KDNode *node, int p[K], int depth);
bool search(KDNode *node, int p[K], int depth);
int findMinDim(KDNode *node, int d, int depth);
KDNode* deletePoint(KDNode *node, int p[K], int depth);
void nearest(KDNode *node, int q[K], int depth, KDNode **best, long *bestDist);
long sqDist(int a[K], int b[K]);
void display(KDNode *node, int level);
void insertMenu();
void searchMenu();
void deleteMenu();
void nearestMenu();
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
      case 3: deleteMenu(); break;
      case 4: nearestMenu(); break;
      case 5:
        printf("\n----- display() -----");
        if (root == NULL) printf("\n[ERROR] Tree is Empty\n");
        else { printf("\nPoints:\n"); display(root, 0); printf("[SUCCESS] Displayed all points\n"); }
        break;
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
        "\n3. delete()"
        "\n4. nearest()"
        "\n5. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

KDNode* createNode(int p[K]) {
  KDNode *n = (KDNode*) malloc(sizeof(KDNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  for (int i = 0; i < K; i++) n->point[i] = p[i];
  n->left = n->right = NULL;
  return n;
}

KDNode* insert(KDNode *node, int p[K], int depth) {
  if (node == NULL) return createNode(p);
  int axis = depth % K;
  if (p[axis] < node->point[axis]) node->left = insert(node->left, p, depth + 1);
  else node->right = insert(node->right, p, depth + 1);
  return node;
}

bool search(KDNode *node, int p[K], int depth) {
  if (node == NULL) return false;
  bool match = true;
  for (int i = 0; i < K; i++) if (node->point[i] != p[i]) { match = false; break; }
  if (match) return true;
  int axis = depth % K;
  if (p[axis] < node->point[axis]) return search(node->left, p, depth + 1);
  return search(node->right, p, depth + 1);
}

int findMinDim(KDNode *node, int d, int depth) {
  if (node == NULL) return INT_MAX;
  int axis = depth % K;
  if (axis == d) {
    if (node->left == NULL) return node->point[d];
    return findMinDim(node->left, d, depth + 1);
  }
  int lm = findMinDim(node->left, d, depth + 1);
  int rm = findMinDim(node->right, d, depth + 1);
  int m = node->point[d];
  if (lm < m) m = lm;
  if (rm < m) m = rm;
  return m;
}

KDNode* deletePoint(KDNode *node, int p[K], int depth) {
  if (node == NULL) return NULL;
  int axis = depth % K;

  bool match = true;
  for (int i = 0; i < K; i++) if (node->point[i] != p[i]) { match = false; break; }

  if (match) {
    if (node->right != NULL) {
      int minVal = findMinDim(node->right, axis, depth + 1);
      int minPoint[K];
      /* find the actual point with this minimum value on axis, in right subtree */
      KDNode *cur = node->right;
      /* simple search to locate a node with point[axis] == minVal */
      KDNode *stack[1000]; int top = 0; stack[top++] = cur;
      KDNode *found = NULL;
      while (top > 0) {
        KDNode *t = stack[--top];
        if (t->point[axis] == minVal) { found = t; break; }
        if (t->left) stack[top++] = t->left;
        if (t->right) stack[top++] = t->right;
      }
      for (int i = 0; i < K; i++) minPoint[i] = found->point[i];
      for (int i = 0; i < K; i++) node->point[i] = minPoint[i];
      node->right = deletePoint(node->right, minPoint, depth + 1);
    } else if (node->left != NULL) {
      int minVal = findMinDim(node->left, axis, depth + 1);
      int minPoint[K];
      KDNode *stack[1000]; int top = 0; stack[top++] = node->left;
      KDNode *found = NULL;
      while (top > 0) {
        KDNode *t = stack[--top];
        if (t->point[axis] == minVal) { found = t; break; }
        if (t->left) stack[top++] = t->left;
        if (t->right) stack[top++] = t->right;
      }
      for (int i = 0; i < K; i++) minPoint[i] = found->point[i];
      for (int i = 0; i < K; i++) node->point[i] = minPoint[i];
      node->right = deletePoint(node->left, minPoint, depth + 1);
      node->left = NULL;
    } else {
      free(node);
      return NULL;
    }
    return node;
  }

  if (p[axis] < node->point[axis]) node->left = deletePoint(node->left, p, depth + 1);
  else node->right = deletePoint(node->right, p, depth + 1);
  return node;
}

long sqDist(int a[K], int b[K]) {
  long sum = 0;
  for (int i = 0; i < K; i++) {
    long diff = a[i] - b[i];
    sum += diff * diff;
  }
  return sum;
}

void nearest(KDNode *node, int q[K], int depth, KDNode **best, long *bestDist) {
  if (node == NULL) return;
  long d = sqDist(node->point, q);
  if (*best == NULL || d < *bestDist) {
    *best = node;
    *bestDist = d;
  }
  int axis = depth % K;
  long diff = q[axis] - node->point[axis];
  KDNode *nearSide = (diff < 0) ? node->left : node->right;
  KDNode *farSide = (diff < 0) ? node->right : node->left;

  nearest(nearSide, q, depth + 1, best, bestDist);
  if (diff * diff < *bestDist) nearest(farSide, q, depth + 1, best, bestDist);
}

void display(KDNode *node, int level) {
  if (node == NULL) return;
  display(node->left, level + 1);
  for (int i = 0; i < level; i++) printf("    ");
  printf("(");
  for (int i = 0; i < K; i++) printf("%d%s", node->point[i], (i < K - 1) ? "," : "");
  printf(")\n");
  display(node->right, level + 1);
}

void insertMenu() {
  printf("\n----- insert() -----");
  int p[K];
  printf("\nEnter %d coordinates: ", K);
  for (int i = 0; i < K; i++) scanf("%d", &p[i]);
  flushStdin();
  root = insert(root, p, 0);
  printf("\n[SUCCESS] Inserted point (%d,%d)\n", p[0], p[1]);
}

void searchMenu() {
  printf("\n----- search() -----");
  int p[K];
  printf("\nEnter %d coordinates: ", K);
  for (int i = 0; i < K; i++) scanf("%d", &p[i]);
  flushStdin();
  if (search(root, p, 0)) printf("\n[SUCCESS] Point (%d,%d) found\n", p[0], p[1]);
  else printf("\n[ERROR] Point (%d,%d) not found\n", p[0], p[1]);
}

void deleteMenu() {
  printf("\n----- delete() -----");
  int p[K];
  printf("\nEnter %d coordinates: ", K);
  for (int i = 0; i < K; i++) scanf("%d", &p[i]);
  flushStdin();
  if (!search(root, p, 0)) {
    printf("\n[ERROR] Point (%d,%d) not found\n", p[0], p[1]);
    return;
  }
  root = deletePoint(root, p, 0);
  printf("\n[SUCCESS] Deleted point (%d,%d)\n", p[0], p[1]);
}

void nearestMenu() {
  printf("\n----- nearest() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  int q[K];
  printf("\nEnter query point (%d coordinates): ", K);
  for (int i = 0; i < K; i++) scanf("%d", &q[i]);
  flushStdin();
  KDNode *best = NULL;
  long bestDist = 0;
  nearest(root, q, 0, &best, &bestDist);
  printf("\n[SUCCESS] Nearest point: (%d,%d)\n", best->point[0], best->point[1]);
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
