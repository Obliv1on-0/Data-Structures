/*
 * Quadtree (point quadtree with bucket capacity)
 *
 * ADT:
 *   createNode(boundary)      - allocate an empty node           O(1)
 *   contains(rect, p)         - does the rectangle contain p     O(1)
 *   intersects(r1, r2)        - do two rectangles overlap        O(1)
 *   subdivide(node)           - create NW, NE, SW, SE children   O(1)
 *   insert(node, p)           - insert a point                   O(log n) avg
 *   query(node, range, out)   - collect all points in a range    O(log n + r)
 *   countPoints(node)         - total number of points           O(nodes)
 *   display(node, level)      - print the tree                   O(nodes)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CAP 4

typedef struct { double x, y; } Point;

typedef struct {
  double cx, cy;
  double hw, hh;
} Rect;

typedef struct QTNode {
  Rect boundary;
  Point pts[CAP];
  int count;
  int divided;
  struct QTNode *nw, *ne, *sw, *se;
} QTNode;

QTNode *root = NULL;

void displayMenu();
void init();
QTNode* createNode(Rect b);
bool rectContains(Rect r, Point p);
bool rectIntersects(Rect a, Rect b);
void subdivide(QTNode *node);
bool insertPoint(QTNode *node, Point p);
void query(QTNode *node, Rect range, int *foundCount);
int countPoints(QTNode *node);
void display(QTNode *node, int level);
void destroy(QTNode *node);
void insertMenu();
void queryMenu();
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
      case 1: insertMenu(); break;
      case 2: queryMenu(); break;
      case 3: printf("\n[SUCCESS] Total points: %d\n", countPoints(root)); break;
      case 4:
        printf("\n----- display() -----");
        display(root, 0);
        printf("[SUCCESS] Displayed the quadtree\n");
        break;
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
        "\n2. query()"
        "\n3. countPoints()"
        "\n4. display()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

void init() {
  printf("\n----- init() -----");
  double cx, cy, hw, hh;
  printf("\nEnter boundary centre x, y and half-width, half-height: ");
  scanf("%lf %lf %lf %lf", &cx, &cy, &hw, &hh);
  flushStdin();
  Rect b = {cx, cy, hw, hh};
  root = createNode(b);
  printf("\n[SUCCESS] Initialized Quadtree\n");
}

QTNode* createNode(Rect b) {
  QTNode *n = (QTNode*) malloc(sizeof(QTNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->boundary = b;
  n->count = 0;
  n->divided = 0;
  n->nw = n->ne = n->sw = n->se = NULL;
  return n;
}

bool rectContains(Rect r, Point p) {
  return p.x >= r.cx - r.hw && p.x <= r.cx + r.hw &&
         p.y >= r.cy - r.hh && p.y <= r.cy + r.hh;
}

bool rectIntersects(Rect a, Rect b) {
  if (a.cx + a.hw < b.cx - b.hw || b.cx + b.hw < a.cx - a.hw) return false;
  if (a.cy + a.hh < b.cy - b.hh || b.cy + b.hh < a.cy - a.hh) return false;
  return true;
}

void subdivide(QTNode *node) {
  double x = node->boundary.cx, y = node->boundary.cy;
  double w = node->boundary.hw / 2, h = node->boundary.hh / 2;
  Rect nwR = {x - w, y - h, w, h};
  Rect neR = {x + w, y - h, w, h};
  Rect swR = {x - w, y + h, w, h};
  Rect seR = {x + w, y + h, w, h};
  node->nw = createNode(nwR);
  node->ne = createNode(neR);
  node->sw = createNode(swR);
  node->se = createNode(seR);
  node->divided = 1;
}

bool insertPoint(QTNode *node, Point p) {
  if (!rectContains(node->boundary, p)) return false;

  if (node->count < CAP && !node->divided) {
    node->pts[node->count++] = p;
    return true;
  }

  if (!node->divided) subdivide(node);

  if (insertPoint(node->nw, p)) return true;
  if (insertPoint(node->ne, p)) return true;
  if (insertPoint(node->sw, p)) return true;
  if (insertPoint(node->se, p)) return true;
  return false;
}

void query(QTNode *node, Rect range, int *foundCount) {
  if (node == NULL || !rectIntersects(node->boundary, range)) return;
  for (int i = 0; i < node->count; i++) {
    if (rectContains(range, node->pts[i])) {
      printf("(%.2f, %.2f) ", node->pts[i].x, node->pts[i].y);
      (*foundCount)++;
    }
  }
  if (node->divided) {
    query(node->nw, range, foundCount);
    query(node->ne, range, foundCount);
    query(node->sw, range, foundCount);
    query(node->se, range, foundCount);
  }
}

int countPoints(QTNode *node) {
  if (node == NULL) return 0;
  int c = node->count;
  if (node->divided) {
    c += countPoints(node->nw);
    c += countPoints(node->ne);
    c += countPoints(node->sw);
    c += countPoints(node->se);
  }
  return c;
}

void display(QTNode *node, int level) {
  if (node == NULL) return;
  for (int i = 0; i < level; i++) printf("    ");
  printf("Node [centre (%.1f,%.1f), half (%.1f,%.1f)]: ", node->boundary.cx, node->boundary.cy, node->boundary.hw, node->boundary.hh);
  for (int i = 0; i < node->count; i++) printf("(%.2f,%.2f) ", node->pts[i].x, node->pts[i].y);
  printf("\n");
  if (node->divided) {
    display(node->nw, level + 1);
    display(node->ne, level + 1);
    display(node->sw, level + 1);
    display(node->se, level + 1);
  }
}

void destroy(QTNode *node) {
  if (node == NULL) return;
  if (node->divided) {
    destroy(node->nw);
    destroy(node->ne);
    destroy(node->sw);
    destroy(node->se);
  }
  free(node);
}

void insertMenu() {
  printf("\n----- insert() -----");
  Point p;
  printf("\nEnter point x and y: ");
  scanf("%lf %lf", &p.x, &p.y);
  flushStdin();
  if (insertPoint(root, p)) printf("\n[SUCCESS] Inserted point (%.2f, %.2f)\n", p.x, p.y);
  else printf("\n[ERROR] Point (%.2f, %.2f) is outside the boundary\n", p.x, p.y);
}

void queryMenu() {
  printf("\n----- query() -----");
  double cx, cy, hw, hh;
  printf("\nEnter query rectangle centre x, y and half-width, half-height: ");
  scanf("%lf %lf %lf %lf", &cx, &cy, &hw, &hh);
  flushStdin();
  Rect range = {cx, cy, hw, hh};
  printf("\nPoints in range: ");
  int foundCount = 0;
  query(root, range, &foundCount);
  if (foundCount == 0) printf("(none)");
  printf("\n[SUCCESS] Query complete, %d point(s) found\n", foundCount);
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
