/*
 * Doubly Linked List (DLL)
 *
 * ADT:
 *   init(L)                 - create empty list               O(1)
 *   insertFront(L, x)       - insert at beginning             O(1)
 *   insertEnd(L, x)         - insert at end                   O(1)
 *   insertAtPos(L, pos, x)  - insert so x becomes node #pos   O(n)
 *   deleteFront(L)          - remove first node               O(1)
 *   deleteEnd(L)            - remove last node                O(1)
 *   deleteAtPos(L, pos)     - remove node #pos                O(n)
 *   deleteByKey(L, key)     - remove first node with key      O(n)
 *   search(L, key)          - return position or 0            O(n)
 *   displayForward(L)       - print head to tail              O(n)
 *   displayBackward(L)      - print tail to head              O(n)
 *   reverse(L)              - reverse the list                O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct DNode {
  int data;
  struct DNode *prev;
  struct DNode *next;
} DNode;

typedef struct DList {
  DNode *head;
  DNode *tail;
  int count;
} DList;

DList list;

void displayMenu();
void init();
DNode* createNode(int x);
void insertFront();
void insertEnd();
void insertAtPos();
void deleteNode(DNode *p);
void deleteFront();
void deleteEnd();
void deleteAtPos();
void deleteByKey();
int search();
void displayForward();
void displayBackward();
void reverse();
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
      case 3: insertAtPos(); break;
      case 4: deleteFront(); break;
      case 5: deleteEnd(); break;
      case 6: deleteAtPos(); break;
      case 7: deleteByKey(); break;
      case 8: search(); break;
      case 9: displayForward(); break;
      case 10: displayBackward(); break;
      case 11: reverse(); break;
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
        "\n3. insertAtPos()"
        "\n4. deleteFront()"
        "\n5. deleteEnd()"
        "\n6. deleteAtPos()"
        "\n7. deleteByKey()"
        "\n8. search()"
        "\n9. displayForward()"
        "\n10. displayBackward()"
        "\n11. reverse()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  list.head = NULL;
  list.tail = NULL;
  list.count = 0;
  printf("\n[SUCCESS] Initialized Doubly Linked List\n");
}

DNode* createNode(int x) {
  DNode *n = (DNode*) malloc(sizeof(DNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->data = x;
  n->prev = NULL;
  n->next = NULL;
  return n;
}

void insertFront() {
  printf("\n----- insertFront() -----");
  int x;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  DNode *n = createNode(x);
  n->next = list.head;
  if (list.head != NULL) list.head->prev = n;
  else list.tail = n;
  list.head = n;
  list.count++;
  printf("\n[SUCCESS] Inserted %d at front\n", x);
}

void insertEnd() {
  printf("\n----- insertEnd() -----");
  int x;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  DNode *n = createNode(x);
  n->prev = list.tail;
  if (list.tail != NULL) list.tail->next = n;
  else list.head = n;
  list.tail = n;
  list.count++;
  printf("\n[SUCCESS] Inserted %d at end\n", x);
}

void insertAtPos() {
  printf("\n----- insertAtPos() -----");
  int pos, x;
  printf("\nEnter position: ");
  scanf("%d", &pos);
  flushStdin();
  if (pos < 1 || pos > list.count + 1) {
    printf("\n[ERROR] Invalid position\n");
    return;
  }
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();

  if (pos == 1) { insertFront(); return; }
  if (pos == list.count + 1) { insertEnd(); return; }

  DNode *cur = list.head;
  for (int i = 0; i < pos - 1; i++) cur = cur->next;
  DNode *n = createNode(x);
  n->prev = cur->prev;
  n->next = cur;
  cur->prev->next = n;
  cur->prev = n;
  list.count++;
  printf("\n[SUCCESS] Inserted %d at position %d\n", x, pos);
}

void deleteNode(DNode *p) {
  if (p->prev != NULL) p->prev->next = p->next;
  else list.head = p->next;
  if (p->next != NULL) p->next->prev = p->prev;
  else list.tail = p->prev;
  free(p);
  list.count--;
}

void deleteFront() {
  printf("\n----- deleteFront() -----");
  if (list.head == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  printf("\n[SUCCESS] Deleted %d from front\n", list.head->data);
  deleteNode(list.head);
}

void deleteEnd() {
  printf("\n----- deleteEnd() -----");
  if (list.tail == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  printf("\n[SUCCESS] Deleted %d from end\n", list.tail->data);
  deleteNode(list.tail);
}

void deleteAtPos() {
  printf("\n----- deleteAtPos() -----");
  int pos;
  printf("\nEnter position: ");
  scanf("%d", &pos);
  flushStdin();
  if (pos < 1 || pos > list.count) {
    printf("\n[ERROR] Invalid position\n");
    return;
  }
  DNode *cur = list.head;
  for (int i = 0; i < pos - 1; i++) cur = cur->next;
  printf("\n[SUCCESS] Deleted %d from position %d\n", cur->data, pos);
  deleteNode(cur);
}

void deleteByKey() {
  printf("\n----- deleteByKey() -----");
  int key;
  printf("\nEnter key to delete: ");
  scanf("%d", &key);
  flushStdin();
  DNode *cur = list.head;
  while (cur != NULL && cur->data != key) cur = cur->next;
  if (cur == NULL) {
    printf("\n[ERROR] Key %d not found\n", key);
    return;
  }
  printf("\n[SUCCESS] Deleted key %d\n", key);
  deleteNode(cur);
}

int search() {
  printf("\n----- search() -----");
  int key;
  printf("\nEnter key to search: ");
  scanf("%d", &key);
  flushStdin();
  int pos = 1;
  DNode *cur = list.head;
  while (cur != NULL) {
    if (cur->data == key) {
      printf("\n[SUCCESS] Key %d found at position %d\n", key, pos);
      return pos;
    }
    cur = cur->next;
    pos++;
  }
  printf("\n[ERROR] Key %d not found\n", key);
  return 0;
}

void displayForward() {
  printf("\n----- displayForward() -----");
  if (list.head == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  printf("\nForward: NULL <- ");
  DNode *cur = list.head;
  while (cur != NULL) {
    printf("%d <-> ", cur->data);
    cur = cur->next;
  }
  printf("NULL");
  printf("\n[SUCCESS] Displayed all nodes forward\n");
}

void displayBackward() {
  printf("\n----- displayBackward() -----");
  if (list.tail == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  printf("\nBackward: NULL <- ");
  DNode *cur = list.tail;
  while (cur != NULL) {
    printf("%d <-> ", cur->data);
    cur = cur->prev;
  }
  printf("NULL");
  printf("\n[SUCCESS] Displayed all nodes backward\n");
}

void reverse() {
  printf("\n----- reverse() -----");
  DNode *cur = list.head;
  DNode *tmp = NULL;
  while (cur != NULL) {
    tmp = cur->prev;
    cur->prev = cur->next;
    cur->next = tmp;
    cur = cur->prev;
  }
  tmp = list.head;
  list.head = list.tail;
  list.tail = tmp;
  printf("\n[SUCCESS] Reversed the list\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
