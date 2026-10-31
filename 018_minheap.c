/*
 * Binary Heap (Min-Heap, array-based)
 *
 * ADT:
 *   init(H)               - create empty heap           O(1)
 *   isEmpty(H), isFull(H)                               O(1)
 *   insert(H, x)          - add element                 O(log n)
 *   getMin(H)             - read smallest element       O(1)
 *   extractMin(H)         - remove and return smallest  O(log n)
 *   siftUp(H, i)          - move element up             O(log n)
 *   heapify(H, i)         - move element down           O(log n)
 *   decreaseKey(H, i, v)  - lower value at index i      O(log n)
 *   deleteAt(H, i)        - remove element at index i   O(log n)
 *   buildHeap(H, arr, n)  - make heap from array        O(n)
 *   display(H)            - print the array             O(n)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX 100
#define ERR INT_MIN

typedef struct MinHeap {
  int a[MAX];
  int size;
} MinHeap;

MinHeap h;

void displayMenu();
void init();
bool isFull();
bool isEmpty();
void insert();
int getMin();
void siftUp(int i);
void heapify(int i);
int extractMin();
void decreaseKey();
void deleteAt();
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
      case 1: insert(); break;
      case 2: getMin(); break;
      case 3: extractMin(); break;
      case 4: decreaseKey(); break;
      case 5: deleteAt(); break;
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
        "\n1. insert()"
        "\n2. getMin()"
        "\n3. extractMin()"
        "\n4. decreaseKey()"
        "\n5. deleteAt()"
        "\n6. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  h.size = 0;
  printf("\n[SUCCESS] Initialized MinHeap\n");
}

bool isFull() {
  if (h.size == MAX) {
    printf("\n[ERROR] Heap is Full\n");
    return true;
  }
  return false;
}

bool isEmpty() {
  if (h.size == 0) {
    printf("\n[ERROR] Heap is Empty\n");
    return true;
  }
  return false;
}

void siftUp(int i) {
  while (i > 0 && h.a[(i - 1) / 2] > h.a[i]) {
    int tmp = h.a[i];
    h.a[i] = h.a[(i - 1) / 2];
    h.a[(i - 1) / 2] = tmp;
    i = (i - 1) / 2;
  }
}

void heapify(int i) {
  int smallest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;
  if (l < h.size && h.a[l] < h.a[smallest]) smallest = l;
  if (r < h.size && h.a[r] < h.a[smallest]) smallest = r;
  if (smallest != i) {
    int tmp = h.a[i];
    h.a[i] = h.a[smallest];
    h.a[smallest] = tmp;
    heapify(smallest);
  }
}

void insert() {
  printf("\n----- insert() -----");
  if (isFull()) return;
  int n;
  printf("\nEnter int data: ");
  if (scanf("%d", &n) != 1) {
    printf("\n[ERROR] Invalid integer input.\n");
    flushStdin();
    return;
  }
  flushStdin();
  h.a[h.size] = n;
  siftUp(h.size);
  h.size++;
  printf("\n[SUCCESS] Inserted element: %d\n", n);
}

int getMin() {
  printf("\n----- getMin() -----");
  if (isEmpty()) return ERR;
  printf("\n[SUCCESS] Minimum Element: %d\n", h.a[0]);
  return h.a[0];
}

int extractMin() {
  printf("\n----- extractMin() -----");
  if (isEmpty()) return ERR;
  int min = h.a[0];
  h.a[0] = h.a[h.size - 1];
  h.size--;
  heapify(0);
  printf("\n[SUCCESS] Extracted Minimum: %d\n", min);
  return min;
}

void decreaseKey() {
  printf("\n----- decreaseKey() -----");
  if (isEmpty()) return;
  int i, v;
  printf("\nEnter index: ");
  scanf("%d", &i);
  flushStdin();
  if (i < 0 || i >= h.size) {
    printf("\n[ERROR] Invalid index\n");
    return;
  }
  printf("\nEnter new (smaller) value: ");
  scanf("%d", &v);
  flushStdin();
  if (v > h.a[i]) {
    printf("\n[ERROR] New value is greater than current value\n");
    return;
  }
  h.a[i] = v;
  siftUp(i);
  printf("\n[SUCCESS] Decreased key at index %d to %d\n", i, v);
}

void deleteAt() {
  printf("\n----- deleteAt() -----");
  if (isEmpty()) return;
  int i;
  printf("\nEnter index to delete: ");
  scanf("%d", &i);
  flushStdin();
  if (i < 0 || i >= h.size) {
    printf("\n[ERROR] Invalid index\n");
    return;
  }
  h.a[i] = INT_MIN;
  siftUp(i);
  extractMin();
  printf("\n[SUCCESS] Deleted element at index %d\n", i);
}

void display() {
  printf("\n----- display() -----");
  if (isEmpty()) return;
  printf("\nHeap array: ");
  for (int i = 0; i < h.size; i++) printf("\n    %d", h.a[i]);
  printf("\n[SUCCESS] Displayed all Heap elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
