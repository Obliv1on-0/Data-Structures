/*
 * Queue (linked list)
 *
 * ADT:
 *   init(Q)        - create empty queue           O(1)
 *   enqueue(Q, x)  - insert at rear               O(1)
 *   dequeue(Q)     - remove and return front      O(1)
 *   front(Q)       - return front without remove  O(1)
 *   isEmpty(Q), size(Q)                           O(1)
 *   display(Q)     - print front to rear          O(n)
 *   clear(Q)       - free all nodes               O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define ERR INT_MIN

typedef struct QNode {
  int data;
  struct QNode *next;
} QNode;

typedef struct LQueue {
  QNode *front;
  QNode *rear;
  int count;
} LQueue;

LQueue q;

void displayMenu();
void init();
void enqueue();
int dequeue();
int front();
bool isEmpty();
int size();
void display();
void clear();
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
      case 3: front(); break;
      case 4: isEmpty(); break;
      case 5: printf("\n[SUCCESS] Size: %d\n", size()); break;
      case 6: display(); break;
      case 7: clear(); break;
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
        "\n3. front()"
        "\n4. isEmpty()"
        "\n5. size()"
        "\n6. display()"
        "\n7. clear()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  q.front = NULL;
  q.rear = NULL;
  q.count = 0;
  printf("\n[SUCCESS] Initialized Linked Queue\n");
}

void enqueue() {
  printf("\n----- enqueue() -----");
  int x;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  QNode *n = (QNode*) malloc(sizeof(QNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    return;
  }
  n->data = x;
  n->next = NULL;
  if (q.front == NULL) {
    q.front = n;
    q.rear = n;
  } else {
    q.rear->next = n;
    q.rear = n;
  }
  q.count++;
  printf("\n[SUCCESS] Enqueued element: %d\n", x);
}

int dequeue() {
  printf("\n----- dequeue() -----");
  if (isEmpty()) return ERR;
  QNode *tmp = q.front;
  int removed = tmp->data;
  q.front = q.front->next;
  if (q.front == NULL) q.rear = NULL;
  free(tmp);
  q.count--;
  printf("\n[SUCCESS] Dequeued Element: %d\n", removed);
  return removed;
}

int front() {
  printf("\n----- front() -----");
  if (isEmpty()) return ERR;
  printf("\n[SUCCESS] Front Element: %d\n", q.front->data);
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
  printf("\nQueue (front to rear): ");
  QNode *cur = q.front;
  while (cur != NULL) {
    printf("\n    %d", cur->data);
    cur = cur->next;
  }
  printf("\n[SUCCESS] Displayed all Queue elements\n");
}

void clear() {
  printf("\n----- clear() -----");
  while (q.front != NULL) {
    QNode *tmp = q.front;
    q.front = q.front->next;
    free(tmp);
  }
  q.rear = NULL;
  q.count = 0;
  printf("\n[SUCCESS] Cleared all Queue elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
