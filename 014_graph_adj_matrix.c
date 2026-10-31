/*
 * Graph (Adjacency Matrix)
 *
 * ADT:
 *   init(G, n, directed)  - create graph with n vertices, no edges   O(V^2)
 *   addEdge(G, u, v, w)   - add edge u -> v                          O(1)
 *   removeEdge(G, u, v)   - delete edge                              O(1)
 *   hasEdge(G, u, v)      - is u adjacent to v                       O(1)
 *   outDegree(G, v)       - number of outgoing edges                 O(V)
 *   inDegree(G, v)        - number of incoming edges                 O(V)
 *   display(G)            - print the matrix                         O(V^2)
 *   BFS(G, s)             - breadth-first traversal                  O(V^2)
 *   DFS(G, s)             - depth-first traversal (recursive)        O(V^2)
 *   countComponents(G)    - connected components (undirected)        O(V^2)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define V 100

typedef struct GraphM {
  int n;
  int directed;
  int adj[V][V];
} GraphM;

GraphM g;
int visited[V];

void displayMenu();
void init();
void addEdge();
void removeEdge();
void hasEdge();
void outDegree();
void inDegree();
void display();
void BFS();
void DFS();
void dfsUtil(int u);
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
      case 4: outDegree(); break;
      case 5: inDegree(); break;
      case 6: display(); break;
      case 7: BFS(); break;
      case 8: DFS(); break;
      case 9: countComponents(); break;
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
        "\n4. outDegree()"
        "\n5. inDegree()"
        "\n6. display()"
        "\n7. BFS()"
        "\n8. DFS()"
        "\n9. countComponents()"
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
  for (int i = 0; i < g.n; i++)
    for (int j = 0; j < g.n; j++)
      g.adj[i][j] = 0;
  printf("\n[SUCCESS] Initialized Graph with %d vertices\n", g.n);
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
  g.adj[u][v] = w;
  if (!g.directed) g.adj[v][u] = w;
  printf("\n[SUCCESS] Added edge %d -> %d (weight %d)\n", u, v, w);
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
  g.adj[u][v] = 0;
  if (!g.directed) g.adj[v][u] = 0;
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
  if (g.adj[u][v] != 0) printf("\n[SUCCESS] Edge %d -> %d exists\n", u, v);
  else printf("\n[INFO] Edge %d -> %d does not exist\n", u, v);
}

void outDegree() {
  printf("\n----- outDegree() -----");
  int v;
  printf("\nEnter vertex: ");
  scanf("%d", &v);
  flushStdin();
  if (v < 0 || v >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  int count = 0;
  for (int j = 0; j < g.n; j++) if (g.adj[v][j] != 0) count++;
  printf("\n[SUCCESS] Out-degree of %d: %d\n", v, count);
}

void inDegree() {
  printf("\n----- inDegree() -----");
  int v;
  printf("\nEnter vertex: ");
  scanf("%d", &v);
  flushStdin();
  if (v < 0 || v >= g.n) {
    printf("\n[ERROR] Invalid vertex\n");
    return;
  }
  int count = 0;
  for (int i = 0; i < g.n; i++) if (g.adj[i][v] != 0) count++;
  printf("\n[SUCCESS] In-degree of %d: %d\n", v, count);
}

void display() {
  printf("\n----- display() -----");
  printf("\nAdjacency Matrix:\n    ");
  for (int j = 0; j < g.n; j++) printf("%3d", j);
  printf("\n");
  for (int i = 0; i < g.n; i++) {
    printf("%3d ", i);
    for (int j = 0; j < g.n; j++) printf("%3d", g.adj[i][j]);
    printf("\n");
  }
  printf("[SUCCESS] Displayed adjacency matrix\n");
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
    for (int v = 0; v < g.n; v++) {
      if (g.adj[u][v] != 0 && !visitedLocal[v]) {
        visitedLocal[v] = 1;
        queue[rear++] = v;
      }
    }
  }
  printf("\n[SUCCESS] BFS traversal complete\n");
}

void dfsUtil(int u) {
  visited[u] = 1;
  printf("%d ", u);
  for (int v = 0; v < g.n; v++) {
    if (g.adj[u][v] != 0 && !visited[v]) dfsUtil(v);
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
