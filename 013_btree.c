/*
 * B-Tree (minimum degree T)
 *
 * ADT:
 *   createNode(leaf)      - allocate empty node          O(1)
 *   traverse(x)           - print keys in sorted order   O(n)
 *   search(x, k)          - find node containing k       O(log n)
 *   splitChild(x, i)      - split full child i of x      O(T)
 *   insertNonFull(x, k)   - insert into non-full node    O(log n)
 *   insert(tree, k)       - insert key                   O(log n)
 *   delete(tree, k)       - delete key                   O(log n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 3
#define MAXKEYS (2 * T - 1)
#define MAXCHILD (2 * T)

typedef struct BTNode {
  int n;
  int keys[MAXKEYS];
  struct BTNode *child[MAXCHILD];
  int leaf;
} BTNode;

BTNode *root = NULL;

void displayMenu();
BTNode* createNode(int leaf);
void traverse(BTNode *x);
BTNode* search(BTNode *x, int k);
void splitChild(BTNode *x, int i);
void insertNonFull(BTNode *x, int k);
void insertKey(int k);
int findKey(BTNode *x, int k);
void removeFromLeaf(BTNode *x, int idx);
void removeFromNonLeaf(BTNode *x, int idx);
int getPred(BTNode *x, int idx);
int getSucc(BTNode *x, int idx);
void fill(BTNode *x, int idx);
void borrowFromPrev(BTNode *x, int idx);
void borrowFromNext(BTNode *x, int idx);
void merge(BTNode *x, int idx);
void deleteKey(BTNode *x, int k);
void deleteRoot(int k);
void display();
static void flushStdin();

int main() {
  printf("\n----- main() -----");
  int choice, k;
  do {
    displayMenu();
    if (scanf("%d", &choice) != 1) {
      printf("\n[ERROR] Invalid input choice.\n");
      flushStdin();
      continue;
    }
    flushStdin();
    switch (choice) {
      case 1:
        printf("\nEnter key to insert: ");
        scanf("%d", &k);
        flushStdin();
        insertKey(k);
        break;
      case 2:
        printf("\nEnter key to search: ");
        scanf("%d", &k);
        flushStdin();
        if (root == NULL || search(root, k) == NULL)
          printf("\n[ERROR] Key %d not found\n", k);
        else
          printf("\n[SUCCESS] Key %d found\n", k);
        break;
      case 3:
        printf("\nEnter key to delete: ");
        scanf("%d", &k);
        flushStdin();
        deleteRoot(k);
        break;
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
        "\n2. search()"
        "\n3. delete()"
        "\n4. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

BTNode* createNode(int leaf) {
  BTNode *node = (BTNode*) malloc(sizeof(BTNode));
  if (node == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  node->n = 0;
  node->leaf = leaf;
  for (int i = 0; i < MAXCHILD; i++) node->child[i] = NULL;
  return node;
}

void traverse(BTNode *x) {
  if (x == NULL) return;
  int i;
  for (i = 0; i < x->n; i++) {
    if (!x->leaf) traverse(x->child[i]);
    printf("%d ", x->keys[i]);
  }
  if (!x->leaf) traverse(x->child[i]);
}

BTNode* search(BTNode *x, int k) {
  if (x == NULL) return NULL;
  int i = 0;
  while (i < x->n && k > x->keys[i]) i++;
  if (i < x->n && x->keys[i] == k) return x;
  if (x->leaf) return NULL;
  return search(x->child[i], k);
}

void splitChild(BTNode *x, int i) {
  BTNode *y = x->child[i];
  BTNode *z = createNode(y->leaf);
  z->n = T - 1;

  for (int j = 0; j < T - 1; j++) z->keys[j] = y->keys[j + T];
  if (!y->leaf) {
    for (int j = 0; j < T; j++) z->child[j] = y->child[j + T];
  }
  y->n = T - 1;

  for (int j = x->n; j >= i + 1; j--) x->child[j + 1] = x->child[j];
  x->child[i + 1] = z;

  for (int j = x->n - 1; j >= i; j--) x->keys[j + 1] = x->keys[j];
  x->keys[i] = y->keys[T - 1];
  x->n++;
}

void insertNonFull(BTNode *x, int k) {
  int i = x->n - 1;
  if (x->leaf) {
    while (i >= 0 && x->keys[i] > k) {
      x->keys[i + 1] = x->keys[i];
      i--;
    }
    x->keys[i + 1] = k;
    x->n++;
  } else {
    while (i >= 0 && x->keys[i] > k) i--;
    i++;
    if (x->child[i]->n == MAXKEYS) {
      splitChild(x, i);
      if (x->keys[i] < k) i++;
    }
    insertNonFull(x->child[i], k);
  }
}

void insertKey(int k) {
  if (root == NULL) {
    root = createNode(1);
    root->keys[0] = k;
    root->n = 1;
    printf("\n[SUCCESS] Inserted key %d\n", k);
    return;
  }
  if (root->n == MAXKEYS) {
    BTNode *s = createNode(0);
    s->child[0] = root;
    splitChild(s, 0);
    root = s;
  }
  insertNonFull(root, k);
  printf("\n[SUCCESS] Inserted key %d\n", k);
}

int findKey(BTNode *x, int k) {
  int idx = 0;
  while (idx < x->n && x->keys[idx] < k) idx++;
  return idx;
}

void removeFromLeaf(BTNode *x, int idx) {
  for (int i = idx + 1; i < x->n; i++) x->keys[i - 1] = x->keys[i];
  x->n--;
}

int getPred(BTNode *x, int idx) {
  BTNode *cur = x->child[idx];
  while (!cur->leaf) cur = cur->child[cur->n];
  return cur->keys[cur->n - 1];
}

int getSucc(BTNode *x, int idx) {
  BTNode *cur = x->child[idx + 1];
  while (!cur->leaf) cur = cur->child[0];
  return cur->keys[0];
}

void removeFromNonLeaf(BTNode *x, int idx) {
  int k = x->keys[idx];
  if (x->child[idx]->n >= T) {
    int pred = getPred(x, idx);
    x->keys[idx] = pred;
    deleteKey(x->child[idx], pred);
  } else if (x->child[idx + 1]->n >= T) {
    int succ = getSucc(x, idx);
    x->keys[idx] = succ;
    deleteKey(x->child[idx + 1], succ);
  } else {
    merge(x, idx);
    deleteKey(x->child[idx], k);
  }
}

void borrowFromPrev(BTNode *x, int idx) {
  BTNode *c = x->child[idx];
  BTNode *s = x->child[idx - 1];
  for (int i = c->n - 1; i >= 0; i--) c->keys[i + 1] = c->keys[i];
  if (!c->leaf) {
    for (int i = c->n; i >= 0; i--) c->child[i + 1] = c->child[i];
  }
  c->keys[0] = x->keys[idx - 1];
  if (!c->leaf) c->child[0] = s->child[s->n];
  x->keys[idx - 1] = s->keys[s->n - 1];
  c->n++;
  s->n--;
}

void borrowFromNext(BTNode *x, int idx) {
  BTNode *c = x->child[idx];
  BTNode *s = x->child[idx + 1];
  c->keys[c->n] = x->keys[idx];
  if (!c->leaf) c->child[c->n + 1] = s->child[0];
  x->keys[idx] = s->keys[0];
  for (int i = 1; i < s->n; i++) s->keys[i - 1] = s->keys[i];
  if (!s->leaf) {
    for (int i = 1; i <= s->n; i++) s->child[i - 1] = s->child[i];
  }
  c->n++;
  s->n--;
}

void merge(BTNode *x, int idx) {
  BTNode *c = x->child[idx];
  BTNode *s = x->child[idx + 1];

  c->keys[T - 1] = x->keys[idx];
  for (int i = 0; i < s->n; i++) c->keys[i + T] = s->keys[i];
  if (!c->leaf) {
    for (int i = 0; i <= s->n; i++) c->child[i + T] = s->child[i];
  }
  for (int i = idx + 1; i < x->n; i++) x->keys[i - 1] = x->keys[i];
  for (int i = idx + 2; i <= x->n; i++) x->child[i - 1] = x->child[i];
  c->n += s->n + 1;
  x->n--;
  free(s);
}

void fill(BTNode *x, int idx) {
  if (idx != 0 && x->child[idx - 1]->n >= T) {
    borrowFromPrev(x, idx);
  } else if (idx != x->n && x->child[idx + 1]->n >= T) {
    borrowFromNext(x, idx);
  } else {
    if (idx != x->n) merge(x, idx);
    else merge(x, idx - 1);
  }
}

void deleteKey(BTNode *x, int k) {
  int idx = findKey(x, k);
  if (idx < x->n && x->keys[idx] == k) {
    if (x->leaf) removeFromLeaf(x, idx);
    else removeFromNonLeaf(x, idx);
  } else {
    if (x->leaf) {
      printf("\n[ERROR] Key %d not found\n", k);
      return;
    }
    bool flag = (idx == x->n);
    if (x->child[idx]->n < T) fill(x, idx);
    if (flag && idx > x->n) deleteKey(x->child[idx - 1], k);
    else deleteKey(x->child[idx], k);
  }
}

void deleteRoot(int k) {
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  deleteKey(root, k);
  if (root->n == 0) {
    BTNode *tmp = root;
    if (root->leaf) root = NULL;
    else root = root->child[0];
    free(tmp);
  }
  printf("\n[SUCCESS] Deleted key %d (if it existed)\n", k);
}

void display() {
  printf("\n----- display() -----");
  if (root == NULL) {
    printf("\n[ERROR] Tree is Empty\n");
    return;
  }
  printf("\nKeys (sorted): ");
  traverse(root);
  printf("\n[SUCCESS] Displayed all keys\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
