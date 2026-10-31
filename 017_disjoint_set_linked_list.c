/*
 * Disjoint Set (linked-list representation, weighted union)
 *
 * ADT:
 *   makeSet(x)       - new set containing only x; returns its node   O(1)
 *   findSet(node)    - return representative value                   O(1)
 *   unionSets(a, b)  - merge the sets of nodes a and b               O(size of smaller)
 *   display(set)     - print all members                             O(n)
 *
 * (Simplified for a practical exam: keys are 0 .. MAX-1; nodeArr[x]
 *  holds the LNode created for key x so menu operations can refer
 *  to elements by their integer value.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

typedef struct LSet {
  struct LNode *head;
  struct LNode *tail;
  int size;
} LSet;

typedef struct LNode {
  int value;
  struct LNode *next;
  LSet *set;
} LNode;

LNode *nodeArr[MAX];
int total;

void displayMenu();
void init(int n);
LNode* makeSet(int x);
int findSet(LNode *nd);
void unionSets(LNode *a, LNode *b);
void displaySet(LSet *s);
void unionMenu();
void findMenu();
void displayAll();
static void flushStdin();

int main() {
  printf("\n----- main() -----");
  printf("\nEnter number of elements (0 .. n-1): ");
  scanf("%d", &total);
  flushStdin();
  init(total);

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
      case 1: unionMenu(); break;
      case 2: findMenu(); break;
      case 3: displayAll(); break;
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
        "\n1. unionSets()"
        "\n2. findSet()"
        "\n3. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init(int n) {
  printf("\n----- init() -----");
  for (int i = 0; i < n; i++) nodeArr[i] = makeSet(i);
  printf("\n[SUCCESS] Initialized %d singleton sets\n", n);
}

LNode* makeSet(int x) {
  LNode *nd = (LNode*) malloc(sizeof(LNode));
  LSet *s = (LSet*) malloc(sizeof(LSet));
  if (nd == NULL || s == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  nd->value = x;
  nd->next = NULL;
  nd->set = s;
  s->head = nd;
  s->tail = nd;
  s->size = 1;
  return nd;
}

int findSet(LNode *nd) {
  return nd->set->head->value;
}

void unionSets(LNode *a, LNode *b) {
  LSet *s1 = a->set;
  LSet *s2 = b->set;
  if (s1 == s2) {
    printf("\n[INFO] Elements already in the same set\n");
    return;
  }
  if (s1->size < s2->size) {
    LSet *tmp = s1;
    s1 = s2;
    s2 = tmp;
  }
  s1->tail->next = s2->head;
  s1->tail = s2->tail;
  for (LNode *p = s2->head; p != NULL; p = p->next) p->set = s1;
  s1->size += s2->size;
  free(s2);
  printf("\n[SUCCESS] Union complete\n");
}

void displaySet(LSet *s) {
  printf("{ ");
  for (LNode *p = s->head; p != NULL; p = p->next) printf("%d ", p->value);
  printf("}");
}

void unionMenu() {
  printf("\n----- unionSets() -----");
  int x, y;
  printf("\nEnter x and y to union: ");
  scanf("%d %d", &x, &y);
  flushStdin();
  if (x < 0 || x >= total || y < 0 || y >= total) {
    printf("\n[ERROR] Invalid element\n");
    return;
  }
  unionSets(nodeArr[x], nodeArr[y]);
}

void findMenu() {
  printf("\n----- findSet() -----");
  int x;
  printf("\nEnter element: ");
  scanf("%d", &x);
  flushStdin();
  if (x < 0 || x >= total) {
    printf("\n[ERROR] Invalid element\n");
    return;
  }
  printf("\n[SUCCESS] Representative of %d is %d\n", x, findSet(nodeArr[x]));
}

void displayAll() {
  printf("\n----- display() -----");
  bool shown[MAX] = {false};
  printf("\nSets: ");
  for (int i = 0; i < total; i++) {
    int rep = findSet(nodeArr[i]);
    if (!shown[rep]) {
      printf("\n    ");
      displaySet(nodeArr[i]->set);
      shown[rep] = true;
    }
  }
  printf("\n[SUCCESS] Displayed all sets\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
