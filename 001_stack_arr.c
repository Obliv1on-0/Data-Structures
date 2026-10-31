/*
 * Stack (Array-based)
 *
 * ADT:
 *   init(S)      - create empty stack               O(1)
 *   push(S, x)   - insert x on top                  O(1)
 *   pop(S)       - remove and return top element    O(1)
 *   peek(S)      - return top without removing      O(1)
 *   isEmpty(S)   - true if no elements              O(1)
 *   isFull(S)    - true if no space left            O(1)
 *   size(S)      - number of elements               O(1)
 *   display(S)   - print elements top to bottom     O(n)
 *   clear(S)     - remove all elements              O(1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100
#define ERR INT_MIN

typedef struct Stack {
  int arr[MAX];
  int top;
} Stack;

Stack s;

void displayMenu();
void init();
void push();
int pop();
int peek();
bool isFull();
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
      case 1: push(); break;
      case 2: pop(); break;
      case 3: peek(); break;
      case 4: isFull(); break;
      case 5: isEmpty(); break;
      case 6: size(); break;
      case 7: display(); break;
      case 8: clear(); break;
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
        "\n1. push()"
        "\n2. pop()"
        "\n3. peek()"
        "\n4. isFull()"
        "\n5. isEmpty()"
        "\n6. size()"
        "\n7. display()"
        "\n8. clear()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  s.top = -1;
  printf("\n[SUCCESS] Initialized Stack\n");
}

void push() {
  printf("\n----- push() -----");
  if (isFull()) return;
  int n;
  do {
    printf("\nEnter int data: ");
    if (scanf("%d", &n) != 1) {
      printf("\n[ERROR] Invalid integer input.");
      flushStdin();
    } else break;
  } while (true);
  s.arr[++s.top] = n;
  printf("\n[SUCCESS] Added element: %d\n", n);
}

int pop() {
  printf("\n----- pop() -----");
  if (isEmpty()) return ERR;
  int popped = s.arr[s.top--];
  printf("\n[SUCCESS] Popped Element: %d\n", popped);
  return popped;
}

int peek() {
  printf("\n----- peek() -----");
  if (isEmpty()) return ERR;
  int peeked = s.arr[s.top];
  printf("\n[SUCCESS] Top Element: %d\n", peeked);
  return peeked;
}

bool isFull() {
  printf("\n----- isFull() -----");
  if (s.top >= MAX - 1) {
    printf("\n[ERROR] Stack is Full\n");
    return true;
  }
  printf("\n[INFO] Stack is not full\n");
  return false;
}

bool isEmpty() {
  printf("\n----- isEmpty() -----");
  if (s.top < 0) {
    printf("\n[ERROR] Stack is Empty\n");
    return true;
  }
  printf("\n[INFO] Stack is not empty\n");
  return false;
}

int size() {
  printf("\n----- size() -----");
  int n = s.top + 1;
  printf("\n[SUCCESS] Stack has %d elements\n", n);
  return n;
}

void display() {
  printf("\n----- display() -----");
  if (isEmpty()) return;
  printf("\nStack (top to bottom): ");
  for (int i = s.top; i >= 0; --i) printf("\n    %d", s.arr[i]);
  printf("\n[SUCCESS] Displayed all Stack elements\n");
}

void clear() {
  printf("\n----- clear() -----");
  if (isEmpty()) return;
  s.top = -1;
  printf("\n[SUCCESS] Cleared all Stack elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
