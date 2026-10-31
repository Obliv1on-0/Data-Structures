/*
 * Stack (linked list)
 *
 * ADT:
 *   init(S)      - create empty stack             O(1)
 *   push(S, x)   - insert x on top                O(1)
 *   pop(S)       - remove and return top element  O(1)
 *   peek(S)      - return top without removing    O(1)
 *   isEmpty(S)   - true if no elements            O(1)
 *   size(S)      - number of elements             O(1)
 *   display(S)   - print top to bottom            O(n)
 *   clear(S)     - free all nodes                 O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define ERR INT_MIN

typedef struct SNode {
  int data;
  struct SNode *next;
} SNode;

typedef struct LStack {
  SNode *top;
  int count;
} LStack;

LStack s;

void displayMenu();
void init();
void push();
int pop();
int peek();
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
        "\n1. push()"
        "\n2. pop()"
        "\n3. peek()"
        "\n4. isEmpty()"
        "\n5. size()"
        "\n6. display()"
        "\n7. clear()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  s.top = NULL;
  s.count = 0;
  printf("\n[SUCCESS] Initialized Linked Stack\n");
}

void push() {
  printf("\n----- push() -----");
  int x;
  printf("\nEnter int data: ");
  if (scanf("%d", &x) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  SNode *n = (SNode*) malloc(sizeof(SNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    return;
  }
  n->data = x;
  n->next = s.top;
  s.top = n;
  s.count++;
  printf("\n[SUCCESS] Pushed element: %d\n", x);
}

int pop() {
  printf("\n----- pop() -----");
  if (isEmpty()) return ERR;
  SNode *tmp = s.top;
  int popped = tmp->data;
  s.top = tmp->next;
  free(tmp);
  s.count--;
  printf("\n[SUCCESS] Popped Element: %d\n", popped);
  return popped;
}

int peek() {
  printf("\n----- peek() -----");
  if (isEmpty()) return ERR;
  printf("\n[SUCCESS] Top Element: %d\n", s.top->data);
  return s.top->data;
}

bool isEmpty() {
  if (s.top == NULL) {
    printf("\n[ERROR] Stack is Empty\n");
    return true;
  }
  return false;
}

int size() {
  return s.count;
}

void display() {
  printf("\n----- display() -----");
  if (isEmpty()) return;
  printf("\nStack (top to bottom): ");
  SNode *cur = s.top;
  while (cur != NULL) {
    printf("\n    %d", cur->data);
    cur = cur->next;
  }
  printf("\n[SUCCESS] Displayed all Stack elements\n");
}

void clear() {
  printf("\n----- clear() -----");
  while (s.top != NULL) {
    SNode *tmp = s.top;
    s.top = s.top->next;
    free(tmp);
  }
  s.count = 0;
  printf("\n[SUCCESS] Cleared all Stack elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
