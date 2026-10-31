/*
 * Hash Table with Open Addressing (closed hashing)
 *
 * ADT:
 *   init(H, m)                - allocate m EMPTY slots             O(m)
 *   h1(k), h2(k)              - primary / secondary hash           O(1)
 *   probe(k, i, method)       - slot examined on i-th attempt      O(1)
 *   insert(H, k, v, method)   - add or update                      O(1) average
 *   search(H, k, method)      - return value or ERR                O(1) average
 *   delete(H, k, method)      - remove key using a tombstone       O(1) average
 *   loadFactor(H)             - n / m (must stay below 1)          O(1)
 *   rehash(H)                 - grow and reinsert                  O(n + m)
 *   display(H)                - print every slot                   O(m)
 *
 * method: LINEAR, QUADRATIC, DOUBLE
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define ERR INT_MIN

typedef enum { EMPTY, OCCUPIED, DELETED } State;

typedef struct Slot {
  int key;
  int value;
  State state;
} Slot;

typedef struct OAHash {
  int m;
  int n;
  Slot *table;
} OAHash;

OAHash h;
int method; /* 1 = LINEAR, 2 = QUADRATIC, 3 = DOUBLE */

void displayMenu();
void init(int m);
int h1(int k);
int h2(int k);
int probe(int k, int i);
void chooseMethod();
void insertKey();
void searchKey();
void deleteKey();
double loadFactor();
void rehash();
void display();
static void flushStdin();

int main() {
  printf("\n----- main() -----");
  printf("\nEnter initial table size (prefer a prime): ");
  int m;
  scanf("%d", &m);
  flushStdin();
  init(m);
  chooseMethod();

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
      case 4: printf("\n[SUCCESS] Load Factor: %.2f\n", loadFactor()); break;
      case 5: display(); break;
      case 6: chooseMethod(); break;
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
        "\n4. loadFactor()"
        "\n5. display()"
        "\n6. change probing method"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init(int m) {
  printf("\n----- init() -----");
  h.m = m;
  h.n = 0;
  h.table = (Slot*) malloc(m * sizeof(Slot));
  if (h.table == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  for (int i = 0; i < m; i++) h.table[i].state = EMPTY;
  printf("\n[SUCCESS] Initialized Hash Table with %d slots\n", m);
}

void chooseMethod() {
  printf("\nChoose probing method (1=LINEAR, 2=QUADRATIC, 3=DOUBLE): ");
  scanf("%d", &method);
  flushStdin();
  if (method < 1 || method > 3) {
    printf("\n[ERROR] Invalid method, defaulting to LINEAR\n");
    method = 1;
  }
}

int h1(int k) {
  return k % h.m;
}

int h2(int k) {
  return 1 + (k % (h.m - 1));
}

int probe(int k, int i) {
  if (method == 1) return (h1(k) + i) % h.m;
  if (method == 2) return (h1(k) + i * i) % h.m;
  return (h1(k) + i * h2(k)) % h.m;
}

void insertKey() {
  printf("\n----- insert() -----");
  if (h.n == h.m) {
    printf("\n[ERROR] Table is Full\n");
    return;
  }
  int k, v;
  printf("\nEnter key: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  printf("\nEnter value: ");
  if (scanf("%d", &v) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();

  for (int i = 0; i < h.m; i++) {
    int j = probe(k, i);
    if (h.table[j].state == OCCUPIED && h.table[j].key == k) {
      h.table[j].value = v;
      printf("\n[SUCCESS] Updated key %d with value %d\n", k, v);
      return;
    }
    if (h.table[j].state != OCCUPIED) {
      h.table[j].key = k;
      h.table[j].value = v;
      h.table[j].state = OCCUPIED;
      h.n++;
      printf("\n[SUCCESS] Inserted key %d with value %d at slot %d\n", k, v, j);
      if (loadFactor() > 0.6) rehash();
      return;
    }
  }
  printf("\n[ERROR] Probe sequence cycled, could not insert. Try rehash.\n");
}

void searchKey() {
  printf("\n----- search() -----");
  int k;
  printf("\nEnter key to search: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();

  for (int i = 0; i < h.m; i++) {
    int j = probe(k, i);
    if (h.table[j].state == EMPTY) {
      printf("\n[ERROR] Key %d not found\n", k);
      return;
    }
    if (h.table[j].state == OCCUPIED && h.table[j].key == k) {
      printf("\n[SUCCESS] Key %d found with value %d at slot %d\n", k, h.table[j].value, j);
      return;
    }
  }
  printf("\n[ERROR] Key %d not found\n", k);
}

void deleteKey() {
  printf("\n----- delete() -----");
  int k;
  printf("\nEnter key to delete: ");
  if (scanf("%d", &k) != 1) { printf("\n[ERROR] Invalid integer input.\n"); flushStdin(); return; }
  flushStdin();

  for (int i = 0; i < h.m; i++) {
    int j = probe(k, i);
    if (h.table[j].state == EMPTY) {
      printf("\n[ERROR] Key %d not found\n", k);
      return;
    }
    if (h.table[j].state == OCCUPIED && h.table[j].key == k) {
      h.table[j].state = DELETED;
      h.n--;
      printf("\n[SUCCESS] Deleted key %d from slot %d\n", k, j);
      return;
    }
  }
  printf("\n[ERROR] Key %d not found\n", k);
}

double loadFactor() {
  return (double) h.n / h.m;
}

bool isPrime(int x) {
  if (x < 2) return false;
  for (int i = 2; (long)i * i <= x; i++) if (x % i == 0) return false;
  return true;
}

int nextPrime(int x) {
  while (!isPrime(x)) x++;
  return x;
}

void rehash() {
  printf("\n----- rehash() -----");
  int oldM = h.m;
  Slot *oldTable = h.table;

  int newM = nextPrime(2 * oldM);
  h.table = (Slot*) malloc(newM * sizeof(Slot));
  if (h.table == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  for (int i = 0; i < newM; i++) h.table[i].state = EMPTY;
  h.m = newM;
  h.n = 0;

  for (int i = 0; i < oldM; i++) {
    if (oldTable[i].state == OCCUPIED) {
      int k = oldTable[i].key, v = oldTable[i].value;
      for (int p = 0; p < h.m; p++) {
        int j = probe(k, p);
        if (h.table[j].state != OCCUPIED) {
          h.table[j].key = k;
          h.table[j].value = v;
          h.table[j].state = OCCUPIED;
          h.n++;
          break;
        }
      }
    }
  }
  free(oldTable);
  printf("\n[SUCCESS] Rehashed table to size %d\n", newM);
}

void display() {
  printf("\n----- display() -----");
  printf("\nHash Table (m = %d, n = %d):", h.m, h.n);
  for (int i = 0; i < h.m; i++) {
    printf("\n  [%d]: ", i);
    if (h.table[i].state == OCCUPIED) printf("key=%d value=%d", h.table[i].key, h.table[i].value);
    else if (h.table[i].state == DELETED) printf("DELETED");
    else printf("EMPTY");
  }
  printf("\n[SUCCESS] Displayed all slots\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
