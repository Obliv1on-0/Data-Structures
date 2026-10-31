/*
 * Circular Queue (Array-based)
 *
 * ADT:
 *   init(Q)        - create empty queue          O(1)
 *   enqueue(Q, x)  - insert x at rear            O(1)
 *   dequeue(Q)     - remove and return front     O(1)
 *   front(Q)       - return front without remove O(1)
 *   isEmpty(Q)     - true if count == 0          O(1)
 *   isFull(Q)      - true if count == MAX        O(1)
 *   size(Q)        - number of elements          O(1)
 *   display(Q)     - print front to rear         O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100
#define ERR INT_MIN

typedef struct CQueue {
  int arr[MAX];
  int front;
  int rear;
  int count;
} CQueue;

CQueue q;

void displayMenu();
void init();
void enqueue();
int dequeue();
int front();
bool isFull();
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
      case 3: front(); break;
      case 4: isFull(); break;
      case 5: isEmpty(); break;
      case 6: size(); break;
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
        "\n1. enqueue()"
        "\n2. dequeue()"
        "\n3. front()"
        "\n4. isFull()"
        "\n5. isEmpty()"
        "\n6. size()"
        "\n7. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  q.front = 0;
  q.rear = MAX - 1;
  q.count = 0;
  printf("\n[SUCCESS] Initialized Circular Queue\n");
}

void enqueue() {
  printf("\n----- enqueue() -----");
  if (isFull()) return;
  int n;
  do {
    printf("\nEnter int data: ");
    if (scanf("%d", &n) != 1) {
      printf("\n[ERROR] Invalid integer input.");
      flushStdin();
    } else break;
  } while (true);
  q.rear = (q.rear + 1) % MAX;
  q.arr[q.rear] = n;
  q.count++;
  printf("\n[SUCCESS] Added element: %d\n", n);
}

int dequeue() {
  printf("\n----- dequeue() -----");
  if (isEmpty()) return ERR;
  int removed = q.arr[q.front];
  q.front = (q.front + 1) % MAX;
  q.count--;
  printf("\n[SUCCESS] Dequeued Element: %d\n", removed);
  return removed;
}

int front() {
  printf("\n----- front() -----");
  if (isEmpty()) return ERR;
  int f = q.arr[q.front];
  printf("\n[SUCCESS] Front Element: %d\n", f);
  return f;
}

bool isFull() {
  printf("\n----- isFull() -----");
  if (q.count == MAX) {
    printf("\n[ERROR] Queue is Full\n");
    return true;
  }
  printf("\n[INFO] Queue is not full\n");
  return false;
}

bool isEmpty() {
  printf("\n----- isEmpty() -----");
  if (q.count == 0) {
    printf("\n[ERROR] Queue is Empty\n");
    return true;
  }
  printf("\n[INFO] Queue is not empty\n");
  return false;
}

int size() {
  printf("\n----- size() -----");
  printf("\n[SUCCESS] Queue has %d elements\n", q.count);
  return q.count;
}

void display() {
  printf("\n----- display() -----");
  if (isEmpty()) return;
  printf("\nQueue (front to rear): ");
  for (int k = 0; k < q.count; k++) {
    int idx = (q.front + k) % MAX;
    printf("\n    %d", q.arr[idx]);
  }
  printf("\n[SUCCESS] Displayed all Queue elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
