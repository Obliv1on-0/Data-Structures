/*
 * Deque (Double-Ended Queue, circular array)
 *
 * ADT:
 *   init(D)            - create empty deque       O(1)
 *   insertFront(D, x)  - insert at front          O(1)
 *   insertRear(D, x)   - insert at rear           O(1)
 *   deleteFront(D)     - remove and return front  O(1)
 *   deleteRear(D)      - remove and return rear   O(1)
 *   getFront(D), getRear(D)                       O(1)
 *   isEmpty(D), isFull(D), size(D)                O(1)
 *   display(D)                                    O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100
#define ERR INT_MIN

typedef struct Deque {
  int arr[MAX];
  int front;
  int rear;
  int count;
} Deque;

Deque d;

void displayMenu();
void init();
bool isFull();
bool isEmpty();
void insertFront();
void insertRear();
int deleteFront();
int deleteRear();
int getFront();
int getRear();
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
      case 1: insertFront(); break;
      case 2: insertRear(); break;
      case 3: deleteFront(); break;
      case 4: deleteRear(); break;
      case 5: getFront(); break;
      case 6: getRear(); break;
      case 7: isFull(); break;
      case 8: isEmpty(); break;
      case 9: printf("\n[SUCCESS] Size: %d\n", size()); break;
      case 10: display(); break;
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
        "\n2. insertRear()"
        "\n3. deleteFront()"
        "\n4. deleteRear()"
        "\n5. getFront()"
        "\n6. getRear()"
        "\n7. isFull()"
        "\n8. isEmpty()"
        "\n9. size()"
        "\n10. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  d.front = 0;
  d.rear = MAX - 1;
  d.count = 0;
  printf("\n[SUCCESS] Initialized Deque\n");
}

bool isFull() {
  if (d.count == MAX) {
    printf("\n[ERROR] Deque is Full\n");
    return true;
  }
  return false;
}

bool isEmpty() {
  if (d.count == 0) {
    printf("\n[ERROR] Deque is Empty\n");
    return true;
  }
  return false;
}

void insertFront() {
  printf("\n----- insertFront() -----");
  if (isFull()) return;
  int x;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  d.front = (d.front - 1 + MAX) % MAX;
  d.arr[d.front] = x;
  d.count++;
  printf("\n[SUCCESS] Inserted %d at front\n", x);
}

void insertRear() {
  printf("\n----- insertRear() -----");
  if (isFull()) return;
  int x;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  d.rear = (d.rear + 1) % MAX;
  d.arr[d.rear] = x;
  d.count++;
  printf("\n[SUCCESS] Inserted %d at rear\n", x);
}

int deleteFront() {
  printf("\n----- deleteFront() -----");
  if (isEmpty()) return ERR;
  int x = d.arr[d.front];
  d.front = (d.front + 1) % MAX;
  d.count--;
  printf("\n[SUCCESS] Deleted %d from front\n", x);
  return x;
}

int deleteRear() {
  printf("\n----- deleteRear() -----");
  if (isEmpty()) return ERR;
  int x = d.arr[d.rear];
  d.rear = (d.rear - 1 + MAX) % MAX;
  d.count--;
  printf("\n[SUCCESS] Deleted %d from rear\n", x);
  return x;
}

int getFront() {
  printf("\n----- getFront() -----");
  if (isEmpty()) return ERR;
  printf("\n[SUCCESS] Front Element: %d\n", d.arr[d.front]);
  return d.arr[d.front];
}

int getRear() {
  printf("\n----- getRear() -----");
  if (isEmpty()) return ERR;
  printf("\n[SUCCESS] Rear Element: %d\n", d.arr[d.rear]);
  return d.arr[d.rear];
}

int size() {
  return d.count;
}

void display() {
  printf("\n----- display() -----");
  if (isEmpty()) return;
  printf("\nDeque (front to rear): ");
  for (int k = 0; k < d.count; k++) {
    int idx = (d.front + k) % MAX;
    printf("\n    %d", d.arr[idx]);
  }
  printf("\n[SUCCESS] Displayed all Deque elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
