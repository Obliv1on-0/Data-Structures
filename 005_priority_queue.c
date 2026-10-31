/*
 * Priority Queue (sorted linked list)
 *
 * ADT:
 *   init(Q)            - create empty queue              O(1)
 *   enqueue(Q, x, pr)  - insert x with priority pr       O(n)
 *   dequeue(Q)         - remove highest priority         O(1)
 *   peek(Q)            - return highest priority         O(1)
 *   isEmpty(Q), size(Q)                                  O(1)
 *   display(Q)                                           O(n)
 *
 * (smaller priority number = higher priority; equal priorities
 *  leave in FIFO order)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define ERR INT_MIN

typedef struct PNode {
  int data;
  int priority;
  struct PNode *next;
} PNode;

typedef struct PQueue {
  PNode *front;
  int count;
} PQueue;

PQueue q;

void displayMenu();
void init();
void enqueue();
int dequeue();
int peek();
bool isEmpty();
int size();
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
      case 1: enqueue(); break;
      case 2: dequeue(); break;
      case 3: peek(); break;
      case 4: isEmpty(); break;
      case 5: printf("\n[SUCCESS] Size: %d\n", size()); break;
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
        "\n1. enqueue()"
        "\n2. dequeue()"
        "\n3. peek()"
        "\n4. isEmpty()"
        "\n5. size()"
        "\n6. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  q.front = NULL;
  q.count = 0;
  printf("\n[SUCCESS] Initialized Priority Queue\n");
}

void enqueue() {
  printf("\n----- enqueue() -----");
  int x, pr;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  printf("\nEnter priority (smaller = higher priority): ");
  if (scanf("%d", &pr) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();

  PNode *n = (PNode*) malloc(sizeof(PNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    return;
  }
  n->data = x;
  n->priority = pr;
  n->next = NULL;

  if (q.front == NULL || pr < q.front->priority) {
    n->next = q.front;
    q.front = n;
  } else {
    PNode *cur = q.front;
    while (cur->next != NULL && cur->next->priority <= pr) cur = cur->next;
    n->next = cur->next;
    cur->next = n;
  }
  q.count++;
  printf("\n[SUCCESS] Enqueued %d with priority %d\n", x, pr);
}

int dequeue() {
  printf("\n----- dequeue() -----");
  if (isEmpty()) return ERR;
  PNode *tmp = q.front;
  int removed = tmp->data;
  q.front = q.front->next;
  free(tmp);
  q.count--;
  printf("\n[SUCCESS] Dequeued Element: %d\n", removed);
  return removed;
}

int peek() {
  printf("\n----- peek() -----");
  if (isEmpty()) return ERR;
  printf("\n[SUCCESS] Highest Priority Element: %d (priority %d)\n", q.front->data, q.front->priority);
  return q.front->data;
}

bool isEmpty() {
  if (q.front == NULL) {
    printf("\n[ERROR] Queue is Empty\n");
    return true;
  }
  return false;
}

int size() {
  return q.count;
}

void display() {
  printf("\n----- display() -----");
  if (isEmpty()) return;
  printf("\nQueue (data, priority): ");
  PNode *cur = q.front;
  while (cur != NULL) {
    printf("\n    (%d, %d)", cur->data, cur->priority);
    cur = cur->next;
  }
  printf("\n[SUCCESS] Displayed all Queue elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
