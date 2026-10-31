/*
 * Direct-Address Table
 *
 * ADT:
 *   init(T)          - clear the table               O(U)
 *   insert(T, k, v)  - T[k] = v                      O(1)
 *   search(T, k)     - return value or ERR           O(1)
 *   delete(T, k)     - remove key                    O(1)
 *   display(T)       - print all present keys        O(U)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define U 100
#define ERR INT_MIN

typedef struct DirectTable {
  int used[U];
  int value[U];
} DirectTable;

DirectTable t;

void displayMenu();
void init();
void insertKey();
void searchKey();
void deleteKey();
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
      case 1: insertKey(); break;
      case 2: searchKey(); break;
      case 3: deleteKey(); break;
      case 4: display(); break;
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
        "\n2. search()"
        "\n3. delete()"
        "\n4. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  for (int i = 0; i < U; i++) t.used[i] = 0;
  printf("\n[SUCCESS] Initialized Direct-Address Table (universe 0..%d)\n", U - 1);
}

void insertKey() {
  printf("\n----- insert() -----");
  int k, v;
  printf("\nEnter key (0..%d): ", U - 1);
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  printf("\nEnter value: ");
  if (scanf("%d", &v) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  if (k < 0 || k >= U) {
    printf("\n[ERROR] Invalid key, out of universe range\n");
    return;
  }
  t.used[k] = 1;
  t.value[k] = v;
  printf("\n[SUCCESS] Inserted key %d with value %d\n", k, v);
}

void searchKey() {
  printf("\n----- search() -----");
  int k;
  printf("\nEnter key to search: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  if (k < 0 || k >= U || !t.used[k]) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }
  printf("\n[SUCCESS] Key %d found with value %d\n", k, t.value[k]);
}

void deleteKey() {
  printf("\n----- delete() -----");
  int k;
  printf("\nEnter key to delete: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();
  if (k < 0 || k >= U || !t.used[k]) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }
  t.used[k] = 0;
  printf("\n[SUCCESS] Deleted key %d\n", k);
}

void display() {
  printf("\n----- display() -----");
  printf("\nPresent keys: ");
  bool any = false;
  for (int k = 0; k < U; k++) {
    if (t.used[k]) {
      printf("\n    %d -> %d", k, t.value[k]);
      any = true;
    }
  }
  if (!any) printf("\n    (none)");
  printf("\n[SUCCESS] Displayed all present keys\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
