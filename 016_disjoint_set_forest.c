/*
 * Disjoint Set (Forest with union by rank and path compression)
 *
 * ADT:
 *   makeSet(D, x)         - create the set {x}                    O(1)
 *   findSet(D, x)         - return representative of x's set      O(alpha(n))
 *   unionSets(D, x, y)    - merge sets containing x and y         O(alpha(n))
 *   sameSet(D, x, y)      - true if x and y are in the same set   O(alpha(n))
 *   countSets(D)          - number of sets                        O(1)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 1000

typedef struct DSU {
  int parent[N];
  int rank[N];
  int sets;
} DSU;

DSU d;

void displayMenu();
void init(int n);
void makeSet(int x);
int findSet(int x);
void unionSets(int x, int y);
bool sameSet(int x, int y);
int countSets();
void display(int n);
static void flushStdin();

int total;

int main() {
  printf("\n----- main() -----");
  printf("\nEnter number of elements (0 .. n-1): ");
  scanf("%d", &total);
  flushStdin();
  init(total);

  int choice, x, y;
  do {
    displayMenu();
    if (scanf("%d", &choice) != 1) {
      printf("\n[ERROR] Invalid input choice.\n");
      flushStdin();
      continue;
    }
    flushStdin();
    switch (choice) {
      case 1:
        printf("\nEnter x and y to union: ");
        scanf("%d %d", &x, &y);
        flushStdin();
        unionSets(x, y);
        break;
      case 2:
        printf("\nEnter element to find set of: ");
        scanf("%d", &x);
        flushStdin();
        printf("\n[SUCCESS] Representative of %d is %d\n", x, findSet(x));
        break;
      case 3:
        printf("\nEnter x and y to check: ");
        scanf("%d %d", &x, &y);
        flushStdin();
        if (sameSet(x, y)) printf("\n[SUCCESS] %d and %d are in the same set\n", x, y);
        else printf("\n[INFO] %d and %d are in different sets\n", x, y);
        break;
      case 4:
        printf("\n[SUCCESS] Number of sets: %d\n", countSets());
        break;
      case 5: display(total); break;
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
        "\n1. unionSets()"
        "\n2. findSet()"
        "\n3. sameSet()"
        "\n4. countSets()"
        "\n5. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init(int n) {
  printf("\n----- init() -----");
  for (int i = 0; i < n; i++) makeSet(i);
  printf("\n[SUCCESS] Initialized %d singleton sets\n", n);
}

void makeSet(int x) {
  d.parent[x] = x;
  d.rank[x] = 0;
  d.sets++;
}

int findSet(int x) {
  if (d.parent[x] != x) d.parent[x] = findSet(d.parent[x]);
  return d.parent[x];
}

void unionSets(int x, int y) {
  printf("\n----- unionSets() -----");
  int a = findSet(x);
  int b = findSet(y);
  if (a == b) {
    printf("\n[INFO] %d and %d are already in the same set\n", x, y);
    return;
  }
  if (d.rank[a] < d.rank[b]) {
    d.parent[a] = b;
  } else if (d.rank[a] > d.rank[b]) {
    d.parent[b] = a;
  } else {
    d.parent[b] = a;
    d.rank[a]++;
  }
  d.sets--;
  printf("\n[SUCCESS] Union of %d and %d done\n", x, y);
}

bool sameSet(int x, int y) {
  return findSet(x) == findSet(y);
}

int countSets() {
  return d.sets;
}

void display(int n) {
  printf("\n----- display() -----");
  printf("\nElement -> Representative: ");
  for (int i = 0; i < n; i++) printf("\n    %d -> %d", i, findSet(i));
  printf("\n[SUCCESS] Displayed all elements\n");
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
