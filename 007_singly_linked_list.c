/*
 * Singly Linked List (SLL)
 *
 * ADT:
 *   init(L)                    - create empty list                  O(1)
 *   insertFront(L, x)          - insert at beginning                O(1)
 *   insertEnd(L, x)            - insert at end                      O(n)
 *   insertAtPos(L, pos, x)     - insert so x becomes node #pos      O(n)
 *   deleteFront(L)             - remove first node                  O(1)
 *   deleteEnd(L)               - remove last node                   O(n)
 *   deleteAtPos(L, pos)        - remove node #pos                   O(n)
 *   deleteByKey(L, key)        - remove first node with key         O(n)
 *   search(L, key)             - return position (1-based) or 0     O(n)
 *   length(L)                  - number of nodes                    O(1)
 *   display(L)                 - print all nodes                    O(n)
 *   reverse(L)                 - reverse the links                  O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
  int data;
  struct Node *next;
} Node;

typedef struct SList {
  Node *head;
  int count;
} SList;

SList list;

void displayMenu();
void init();
Node* createNode(int x);
void insertFront();
void insertEnd();
void insertAtPos();
void deleteFront();
void deleteEnd();
void deleteAtPos();
void deleteByKey();
int search();
int length();
void display();
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
      case 9: printf("\n[SUCCESS] Length: %d\n", length()); break;
      case 10: display(); break;
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
        "\n9. length()"
        "\n10. display()"
        "\n11. reverse()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  list.head = NULL;
  list.count = 0;
  printf("\n[SUCCESS] Initialized Singly Linked List\n");
}

Node* createNode(int x) {
  Node *n = (Node*) malloc(sizeof(Node));
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
  Node *n = createNode(x);
  n->next = list.head;
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
  Node *n = createNode(x);
  if (list.head == NULL) {
    list.head = n;
  } else {
    Node *cur = list.head;
    while (cur->next != NULL) cur = cur->next;
    cur->next = n;
  }
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

  if (pos == 1) {
    Node *n = createNode(x);
    n->next = list.head;
    list.head = n;
    list.count++;
    printf("\n[SUCCESS] Inserted %d at position %d\n", x, pos);
    return;
  }
  Node *prev = list.head;
  for (int i = 0; i < pos - 2; i++) prev = prev->next;
  Node *n = createNode(x);
  n->next = prev->next;
  prev->next = n;
  list.count++;
  printf("\n[SUCCESS] Inserted %d at position %d\n", x, pos);
}

void deleteFront() {
  printf("\n----- deleteFront() -----");
  if (list.head == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  Node *tmp = list.head;
  list.head = list.head->next;
  printf("\n[SUCCESS] Deleted %d from front\n", tmp->data);
  free(tmp);
  list.count--;
}

void deleteEnd() {
  printf("\n----- deleteEnd() -----");
  if (list.head == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  if (list.head->next == NULL) {
    printf("\n[SUCCESS] Deleted %d from end\n", list.head->data);
    free(list.head);
    list.head = NULL;
  } else {
    Node *prev = list.head;
    while (prev->next->next != NULL) prev = prev->next;
    printf("\n[SUCCESS] Deleted %d from end\n", prev->next->data);
    free(prev->next);
    prev->next = NULL;
  }
  list.count--;
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
  if (pos == 1) {
    deleteFront();
    return;
  }
  Node *prev = list.head;
  for (int i = 0; i < pos - 2; i++) prev = prev->next;
  Node *tmp = prev->next;
  prev->next = tmp->next;
  printf("\n[SUCCESS] Deleted %d from position %d\n", tmp->data, pos);
  free(tmp);
  list.count--;
}

void deleteByKey() {
  printf("\n----- deleteByKey() -----");
  if (list.head == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  int key;
  printf("\nEnter key to delete: ");
  scanf("%d", &key);
  flushStdin();

  if (list.head->data == key) {
    deleteFront();
    return;
  }
  Node *prev = list.head;
  Node *cur = prev->next;
  while (cur != NULL && cur->data != key) {
    prev = cur;
    cur = cur->next;
  }
  if (cur == NULL) {
    printf("\n[ERROR] Key %d not found\n", key);
    return;
  }
  prev->next = cur->next;
  printf("\n[SUCCESS] Deleted key %d\n", key);
  free(cur);
  list.count--;
}

int search() {
  printf("\n----- search() -----");
  int key;
  printf("\nEnter key to search: ");
  scanf("%d", &key);
  flushStdin();
  int pos = 1;
  Node *cur = list.head;
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

int length() {
  return list.count;
}

void display() {
  printf("\n----- display() -----");
  if (list.head == NULL) {
    printf("\n[ERROR] List is Empty\n");
    return;
  }
  printf("\nList: ");
  Node *cur = list.head;
  while (cur != NULL) {
    printf("%d -> ", cur->data);
    cur = cur->next;
  }
  printf("NULL");
  printf("\n[SUCCESS] Displayed all nodes\n");
}

void reverse() {
  printf("\n----- reverse() -----");
  Node *prev = NULL;
  Node *cur = list.head;
  while (cur != NULL) {
    Node *next = cur->next;
    cur->next = prev;
    prev = cur;
    cur = next;
  }
  list.head = prev;
  printf("\n[SUCCESS] Reversed the list\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
