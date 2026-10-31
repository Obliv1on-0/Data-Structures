/*
 * Graph (Adjacency List)
 *
 * ADT:
 *   init(G, n, directed)  - create graph with n vertices       O(V)
 *   addEdge(G, u, v, w)   - add edge                           O(1)
 *   removeEdge(G, u, v)   - delete edge                        O(deg(u))
 *   hasEdge(G, u, v)      - is v in u's list                   O(deg(u))
 *   degree(G, v)          - length of v's list                 O(deg(v))
 *   display(G)            - print every list                   O(V + E)
 *   BFS(G, s)             - breadth-first traversal            O(V + E)
 *   DFS(G, s)             - depth-first traversal              O(V + E)
 *   countComponents(G)    - connected components               O(V + E)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define V 100

typedef struct AdjNode {
  int dest;
  int weight;
  struct AdjNode *next;
} AdjNode;

typedef struct GraphL {
  int n;
  int directed;
  AdjNode *head[V];
} GraphL;

GraphL g;
int visited[V];

void displayMenu();
void init();
AdjNode* createNode(int dest, int weight);
void addEdge();
void removeEdge();
void removeEdgeDir(int u, int v);
void hasEdge();
void degree();
void display();
void BFS();
void dfsUtil(int u);
void DFS();
void countComponents();
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
      case 1: addEdge(); break;
      case 2: removeEdge(); break;
      case 3: hasEdge(); break;
      case 4: degree(); break;
      case 5: display(); break;
      case 6: BFS(); break;
      case 7: DFS(); break;
      case 8: countComponents(); break;
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
        "\n1. addEdge()"
        "\n2. removeEdge()"
        "\n3. hasEdge()"
        "\n4. degree()"
        "\n5. display()"
        "\n6. BFS()"
        "\n7. DFS()"
        "\n8. countComponents()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  printf("\nEnter number of vertices: ");
  scanf("%d", &g.n);
  printf("\nDirected graph? (1 = yes, 0 = no): ");
  scanf("%d", &g.directed);
  flushStdin();
  for (int i = 0; i < g.n; i++) g.head[i] = NULL;
  printf("\n[SUCCESS] Initialized Graph with %d vertices\n", g.n);
}

AdjNode* createNode(int dest, int weight) {
  AdjNode *p = (AdjNode*) malloc(sizeof(AdjNode));
  if (p == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  p->dest = dest;
  p->weight = weight;
  p->next = NULL;
  return p;
}

void addEdge() {
  printf("\n----- addEdge() -----");
  int u, v, w;
  printf("\nEnter u, v and weight: ");
  scanf("%d %d %d", &u, &v, &w);
  flushStdin();
  if (u < 0 || u >= g.n || v < 0 || v >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  AdjNode *p = createNode(v, w);
  p->next = g.head[u];
  g.head[u] = p;
  if (!g.directed) {
    AdjNode *q = createNode(u, w);
    q->next = g.head[v];
    g.head[v] = q;
  }
  printf("\n[SUCCESS] Added edge %d -> %d (weight %d)\n", u, v, w);
}

void removeEdgeDir(int u, int v) {
  AdjNode *prev = NULL;
  AdjNode *cur = g.head[u];
  while (cur != NULL && cur->dest != v) {
    prev = cur;
    cur = cur->next;
  }
  if (cur == NULL) return;
  if (prev == NULL) g.head[u] = cur->next;
  else prev->next = cur->next;
  free(cur);
}

void removeEdge() {
  printf("\n----- removeEdge() -----");
  int u, v;
  printf("\nEnter u and v: ");
  scanf("%d %d", &u, &v);
  flushStdin();
  if (u < 0 || u >= g.n || v < 0 || v >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  removeEdgeDir(u, v);
  if (!g.directed) removeEdgeDir(v, u);
  printf("\n[SUCCESS] Removed edge %d -> %d\n", u, v);
}

void hasEdge() {
  printf("\n----- hasEdge() -----");
  int u, v;
  printf("\nEnter u and v: ");
  scanf("%d %d", &u, &v);
  flushStdin();
  if (u < 0 || u >= g.n || v < 0 || v >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  AdjNode *cur = g.head[u];
  while (cur != NULL) {
    if (cur->dest == v) {
      printf("\n[SUCCESS] Edge %d -> %d exists\n", u, v);
      return;
    }
    cur = cur->next;
  }
  printf("\n[INFO] Edge %d -> %d does not exist\n", u, v);
}

void degree() {
  printf("\n----- degree() -----");
  int v;
  printf("\nEnter vertex: ");
  scanf("%d", &v);
  flushStdin();
  if (v < 0 || v >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  int count = 0;
  AdjNode *cur = g.head[v];
  while (cur != NULL) { count++; cur = cur->next; }
  printf("\n[SUCCESS] Degree of %d: %d\n", v, count);
}

void display() {
  printf("\n----- display() -----");
  printf("\nAdjacency List:");
  for (int i = 0; i < g.n; i++) {
    printf("\n  %d -> ", i);
    AdjNode *cur = g.head[i];
    while (cur != NULL) {
      printf("%d(w=%d) -> ", cur->dest, cur->weight);
      cur = cur->next;
    }
    printf("NULL");
  }
  printf("\n[SUCCESS] Displayed all lists\n");
}

void BFS() {
  printf("\n----- BFS() -----");
  int s;
  printf("\nEnter start vertex: ");
  scanf("%d", &s);
  flushStdin();
  if (s < 0 || s >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  int visitedLocal[V] = {0};
  int queue[V], front = 0, rear = 0;
  visitedLocal[s] = 1;
  queue[rear++] = s;
  printf("\nBFS Order: ");
  while (front < rear) {
    int u = queue[front++];
    printf("%d ", u);
    for (AdjNode *p = g.head[u]; p != NULL; p = p->next) {
      if (!visitedLocal[p->dest]) {
        visitedLocal[p->dest] = 1;
        queue[rear++] = p->dest;
      }
    }
  }
  printf("\n[SUCCESS] BFS traversal complete\n");
}

void dfsUtil(int u) {
  visited[u] = 1;
  printf("%d ", u);
  for (AdjNode *p = g.head[u]; p != NULL; p = p->next) {
    if (!visited[p->dest]) dfsUtil(p->dest);
  }
}

void DFS() {
  printf("\n----- DFS() -----");
  int s;
  printf("\nEnter start vertex: ");
  scanf("%d", &s);
  flushStdin();
  if (s < 0 || s >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  for (int i = 0; i < g.n; i++) visited[i] = 0;
  printf("\nDFS Order: ");
  dfsUtil(s);
  printf("\n[SUCCESS] DFS traversal complete\n");
}

void countComponents() {
  printf("\n----- countComponents() -----");
  for (int i = 0; i < g.n; i++) visited[i] = 0;
  int count = 0;
  for (int i = 0; i < g.n; i++) {
    if (!visited[i]) {
      dfsUtil(i);
      count++;
    }
  }
  printf("\n[SUCCESS] Number of connected components: %d\n", count);
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
