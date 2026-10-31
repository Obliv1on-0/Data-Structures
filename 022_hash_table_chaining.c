/*
 * Hash Table with Chaining (separate chaining)
 *
 * ADT:
 *   init(H, m)         - allocate m empty buckets           O(m)
 *   hash(H, k)         - bucket index for key               O(1)
 *   insert(H, k, v)    - add or update a key                O(1) average
 *   search(H, k)       - return value or ERR                O(1 + load factor)
 *   delete(H, k)       - remove a key                       O(1 + load factor)
 *   loadFactor(H)      - n / m                              O(1)
 *   rehash(H)          - grow table and reinsert all keys   O(n + m)
 *   display(H)         - print every bucket                 O(n + m)
 *
 * Hash Functions (ADT, used by hash()):
 *   hashDivision(k)       - k mod M                                   O(1)
 *   hashMultiplication(k) - floor(M * frac(k * A)), A = 0.6180339887  O(1)
 *   hashFolding(k)        - sum of digit groups, then mod M           O(digits)
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <math.h>

#define ERR INT_MIN
#define INITIAL_M 11
#define A_CONST 0.6180339887

typedef struct HNode {
  int key;
  int value;
  struct HNode *next;
} HNode;

typedef struct HashTable {
  int m;
  int n;
  HNode **table;
} HashTable;

HashTable h;

void displayMenu();
void init(int m);
int hashDivision(int k);
int hashMultiplication(int k);
int hashFolding(int k);
int hashKey(int k);
void insertKey();
int searchKey();
void deleteKey();
double loadFactor();
void rehash();
void display();
static void flushStdin();

int main() {
  printf("\n----- main() -----");
  init(INITIAL_M);
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
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init(int m) {
  printf("\n----- init() -----");
  h.m = m;
  h.n = 0;
  h.table = (HNode**) calloc(m, sizeof(HNode*));
  if (h.table == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  printf("\n[SUCCESS] Initialized Hash Table with %d buckets\n", m);
}

int hashDivision(int k) {
  return k % h.m;
}

int hashMultiplication(int k) {
  double x = k * A_CONST;
  double frac = x - (int)x;
  if (frac < 0) frac += 1;
  return (int)(h.m * frac);
}

int hashFolding(int k) {
  if (k < 0) k = -k;
  int sum = 0;
  while (k > 0) {
    sum += k % 100;
    k /= 100;
  }
  return sum % h.m;
}

/* using division method as the default hash() for insert/search/delete */
int hashKey(int k) {
  return hashDivision(k);
}

void insertKey() {
  printf("\n----- insert() -----");
  int k, v;
  printf("\nEnter key: ");
  if (scanf("%d", &k) != 1) {
    printf("\n[ERROR] Invalid integer input.\n");
    flushStdin();
    return;
  }
  printf("\nEnter value: ");
  if (scanf("%d", &v) != 1) {
    printf("\n[ERROR] Invalid integer input.\n");
    flushStdin();
    return;
  }
  flushStdin();

  int i = hashKey(k);
  HNode *cur = h.table[i];
  while (cur != NULL) {
    if (cur->key == k) {
      cur->value = v;
      printf("\n[SUCCESS] Updated key %d with value %d\n", k, v);
      return;
    }
    cur = cur->next;
  }

  HNode *p = (HNode*) malloc(sizeof(HNode));
  if (p == NULL) {
    printf("\n[ERROR] Memory error\n");
    return;
  }
  p->key = k;
  p->value = v;
  p->next = h.table[i];
  h.table[i] = p;
  h.n++;
  printf("\n[SUCCESS] Inserted key %d with value %d\n", k, v);

  if (loadFactor() > 0.75) rehash();
}

int searchKey() {
  printf("\n----- search() -----");
  int k;
  printf("\nEnter key to search: ");
  if (scanf("%d", &k) != 1) {
    printf("\n[ERROR] Invalid integer input.\n");
    flushStdin();
    return ERR;
  }
  flushStdin();

  int i = hashKey(k);
  HNode *cur = h.table[i];
  while (cur != NULL) {
    if (cur->key == k) {
      printf("\n[SUCCESS] Key %d found with value %d\n", k, cur->value);
      return cur->value;
    }
    cur = cur->next;
  }
  printf("\n[ERROR] Key %d not found\n", k);
  return ERR;
}

void deleteKey() {
  printf("\n----- delete() -----");
  int k;
  printf("\nEnter key to delete: ");
  if (scanf("%d", &k) != 1) {
    printf("\n[ERROR] Invalid integer input.\n");
    flushStdin();
    return;
  }
  flushStdin();

  int i = hashKey(k);
  HNode *prev = NULL;
  HNode *cur = h.table[i];
  while (cur != NULL && cur->key != k) {
    prev = cur;
    cur = cur->next;
  }
  if (cur == NULL) {
    printf("\n[ERROR] Key %d not found\n", k);
    return;
  }
  if (prev == NULL) h.table[i] = cur->next;
  else prev->next = cur->next;
  free(cur);
  h.n--;
  printf("\n[SUCCESS] Deleted key %d\n", k);
}

double loadFactor() {
  return (double) h.n / h.m;
}

void rehash() {
  printf("\n----- rehash() -----");
  int oldM = h.m;
  HNode **oldTable = h.table;

  int newM = 2 * oldM + 1;
  h.table = (HNode**) calloc(newM, sizeof(HNode*));
  if (h.table == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  h.m = newM;
  h.n = 0;

  for (int i = 0; i < oldM; i++) {
    HNode *cur = oldTable[i];
    while (cur != NULL) {
      HNode *next = cur->next;
      int idx = hashKey(cur->key);
      cur->next = h.table[idx];
      h.table[idx] = cur;
      h.n++;
      cur = next;
    }
  }
  free(oldTable);
  printf("\n[SUCCESS] Rehashed table to %d buckets\n", newM);
}

void display() {
  printf("\n----- display() -----");
  printf("\nHash Table (m = %d, n = %d):", h.m, h.n);
  for (int i = 0; i < h.m; i++) {
    printf("\n  [%d]: ", i);
    HNode *cur = h.table[i];
    while (cur != NULL) {
      printf("(%d,%d) -> ", cur->key, cur->value);
      cur = cur->next;
    }
    printf("NULL");
  }
  printf("\n[SUCCESS] Displayed all buckets\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
