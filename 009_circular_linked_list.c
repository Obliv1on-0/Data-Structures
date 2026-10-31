/*
 * Circular Linked List (singly, tail pointer)
 *
 * ADT:
 *   init(L)              - create empty list        O(1)
 *   insertFront(L, x)    - insert at beginning      O(1)
 *   insertEnd(L, x)      - insert at end            O(1)
 *   deleteFront(L)       - remove first node        O(1)
 *   deleteEnd(L)         - remove last node         O(n)
 *   search(L, key)       - true/false               O(n)
 *   display(L)           - print one full round     O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct CNode {
  int data;
  struct CNode *next;
} CNode;

typedef struct CList {
  CNode *tail;
  int count;
} CList;

CList list;

void displayMenu();
void init();
CNode* createNode(int x);
void insertFront();
void insertEnd();
void deleteFront();
void deleteEnd();
void search();
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
      case 1: insertFront(); break;
      case 2: insertEnd(); break;
      case 3: deleteFront(); break;
      case 4: deleteEnd(); break;
      case 5: search(); break;
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
        "\n1. insertFront()"
        "\n2. insertEnd()"
        "\n3. deleteFront()"
        "\n4. deleteEnd()"
        "\n5. search()"
        "\n6. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  list.tail = NULL;
  list.count = 0;
  printf("\n[SUCCESS] Initialized Circular Linked List\n");
}

CNode* createNode(int x) {
  CNode *n = (CNode*) malloc(sizeof(CNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->data = x;
  n->next = NULL;
  return n;
}

void insertFront() {
  printf("\n----- insertFront() -----");
  int x;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  CNode *n = createNode(x);
  if (list.tail == NULL) {
    n->next = n;
    list.tail = n;
  } else {
    n->next = list.tail->next;
    list.tail->next = n;
  }
  list.count++;
  printf("\n[SUCCESS] Inserted %d at front\n", x);
}

void insertEnd() {
  printf("\n----- insertEnd() -----");
  insertFront();
  list.tail = list.tail->next;
}

void deleteFront() {
  printf("\n----- deleteFront() -----");
  if (list.tail == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  if (list.count == 1) {
    printf("\n[SUCCESS] Deleted %d from front\n", list.tail->data);
    free(list.tail);
    list.tail = NULL;
  } else {
    CNode *h = list.tail->next;
    list.tail->next = h->next;
    printf("\n[SUCCESS] Deleted %d from front\n", h->data);
    free(h);
  }
  list.count--;
}

void deleteEnd() {
  printf("\n----- deleteEnd() -----");
  if (list.tail == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  if (list.count == 1) {
    printf("\n[SUCCESS] Deleted %d from end\n", list.tail->data);
    free(list.tail);
    list.tail = NULL;
  } else {
    CNode *prev = list.tail->next;
    while (prev->next != list.tail) prev = prev->next;
    prev->next = list.tail->next;
    printf("\n[SUCCESS] Deleted %d from end\n", list.tail->data);
    free(list.tail);
    list.tail = prev;
  }
  list.count--;
}

void search() {
  printf("\n----- search() -----");
  if (list.tail == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  int key;
  printf("\nEnter key to search: ");
  scanf("%d", &key);
  flushStdin();
  CNode *cur = list.tail->next;
  for (int i = 0; i < list.count; i++) {
    if (cur->data == key) {
      printf("\n[SUCCESS] Key %d found\n", key);
      return;
    }
    cur = cur->next;
  }
  printf("\n[ERROR] Key %d not found\n", key);
}

void display() {
  printf("\n----- display() -----");
  if (list.tail == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  printf("\nList (one round): ");
  CNode *cur = list.tail->next;
  for (int i = 0; i < list.count; i++) {
    printf("%d -> ", cur->data);
    cur = cur->next;
  }
  printf("(back to head)");
  printf("\n[SUCCESS] Displayed all nodes\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
