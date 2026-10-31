/*
 * Trie (prefix tree, lowercase letters a-z)
 *
 * ADT:
 *   createNode()           - allocate a node with no children        O(ALPHA)
 *   insert(root, word)     - add a word                              O(L)
 *   search(root, word)     - is the whole word stored                O(L)
 *   startsWith(root, pre)  - does any word start with pre            O(L)
 *   delete(root, word)     - remove a word                           O(L)
 *   printAll(root)         - list all stored words alphabetically    O(total chars)
 *   countWords(root)       - number of stored words                  O(nodes)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHA 26
#define MAXWORD 100

typedef struct TrieNode {
  struct TrieNode *child[ALPHA];
  int isEnd;
} TrieNode;

TrieNode *root;

void displayMenu();
TrieNode* createNode();
void init();
void insertWord(char *word);
bool searchWord(char *word);
bool startsWith(char *pre);
bool hasChildren(TrieNode *node);
TrieNode* deleteWord(TrieNode *node, char *word, int depth);
void printAllUtil(TrieNode *node, char *buffer, int level);
int countWordsUtil(TrieNode *node);
void insertMenu();
void searchMenu();
void startsWithMenu();
void deleteMenu();
void printAllMenu();
void countWordsMenu();
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
      case 2: searchMenu(); break;
      case 3: startsWithMenu(); break;
      case 4: deleteMenu(); break;
      case 5: printAllMenu(); break;
      case 6: countWordsMenu(); break;
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
        "\n3. startsWith()"
        "\n4. delete()"
        "\n5. printAll()"
        "\n6. countWords()"
        "\n0. exit()"
        "\nEnter your choice: ");
}

TrieNode* createNode() {
  TrieNode *n = (TrieNode*) malloc(sizeof(TrieNode));
  if (n == NULL) {
    printf("\n[ERROR] Memory error\n");
    exit(1);
  }
  n->isEnd = 0;
  for (int i = 0; i < ALPHA; i++) n->child[i] = NULL;
  return n;
}

void init() {
  printf("\n----- init() -----");
  root = createNode();
  printf("\n[SUCCESS] Initialized Trie\n");
}

void insertWord(char *word) {
  TrieNode *cur = root;
  for (int i = 0; word[i] != '\0'; i++) {
    int idx = word[i] - 'a';
    if (idx < 0 || idx >= ALPHA) {
      printf("\n[ERROR] Only lowercase letters a-z are supported\n");
      return;
    }
    if (cur->child[idx] == NULL) cur->child[idx] = createNode();
    cur = cur->child[idx];
  }
  cur->isEnd = 1;
  printf("\n[SUCCESS] Inserted word: %s\n", word);
}

bool searchWord(char *word) {
  TrieNode *cur = root;
  for (int i = 0; word[i] != '\0'; i++) {
    int idx = word[i] - 'a';
    if (idx < 0 || idx >= ALPHA || cur->child[idx] == NULL) return false;
    cur = cur->child[idx];
  }
  return cur->isEnd == 1;
}

bool startsWith(char *pre) {
  TrieNode *cur = root;
  for (int i = 0; pre[i] != '\0'; i++) {
    int idx = pre[i] - 'a';
    if (idx < 0 || idx >= ALPHA || cur->child[idx] == NULL) return false;
    cur = cur->child[idx];
  }
  return true;
}

bool hasChildren(TrieNode *node) {
  for (int i = 0; i < ALPHA; i++) if (node->child[i] != NULL) return true;
  return false;
}

TrieNode* deleteWord(TrieNode *node, char *word, int depth) {
  if (node == NULL) return NULL;

  if (depth == (int) strlen(word)) {
    if (node->isEnd) node->isEnd = 0;
    if (!hasChildren(node)) {
      free(node);
      return NULL;
    }
    return node;
  }

  int idx = word[depth] - 'a';
  if (idx < 0 || idx >= ALPHA) return node;
  node->child[idx] = deleteWord(node->child[idx], word, depth + 1);

  if (!hasChildren(node) && node->isEnd == 0 && node != root) {
    free(node);
    return NULL;
  }
  return node;
}

void printAllUtil(TrieNode *node, char *buffer, int level) {
  if (node->isEnd) {
    buffer[level] = '\0';
    printf("%s ", buffer);
  }
  for (int i = 0; i < ALPHA; i++) {
    if (node->child[i] != NULL) {
      buffer[level] = 'a' + i;
      printAllUtil(node->child[i], buffer, level + 1);
    }
  }
}

int countWordsUtil(TrieNode *node) {
  if (node == NULL) return 0;
  int count = node->isEnd;
  for (int i = 0; i < ALPHA; i++) count += countWordsUtil(node->child[i]);
  return count;
}

void insertMenu() {
  printf("\n----- insert() -----");
  char word[MAXWORD];
  printf("\nEnter word (lowercase a-z): ");
  scanf("%s", word);
  flushStdin();
  insertWord(word);
}

void searchMenu() {
  printf("\n----- search() -----");
  char word[MAXWORD];
  printf("\nEnter word to search: ");
  scanf("%s", word);
  flushStdin();
  if (searchWord(word)) printf("\n[SUCCESS] Word \"%s\" found\n", word);
  else printf("\n[ERROR] Word \"%s\" not found\n", word);
}

void startsWithMenu() {
  printf("\n----- startsWith() -----");
  char pre[MAXWORD];
  printf("\nEnter prefix: ");
  scanf("%s", pre);
  flushStdin();
  if (startsWith(pre)) printf("\n[SUCCESS] Some word starts with \"%s\"\n", pre);
  else printf("\n[ERROR] No word starts with \"%s\"\n", pre);
}

void deleteMenu() {
  printf("\n----- delete() -----");
  char word[MAXWORD];
  printf("\nEnter word to delete: ");
  scanf("%s", word);
  flushStdin();
  if (!searchWord(word)) {
    printf("\n[ERROR] Word \"%s\" not found\n", word);
    return;
  }
  root = deleteWord(root, word, 0);
  if (root == NULL) root = createNode();
  printf("\n[SUCCESS] Deleted word: %s\n", word);
}

void printAllMenu() {
  printf("\n----- printAll() -----");
  char buffer[MAXWORD];
  printf("\nStored words: ");
  printAllUtil(root, buffer, 0);
  printf("\n[SUCCESS] Displayed all words\n");
}

void countWordsMenu() {
  printf("\n----- countWords() -----");
  printf("\n[SUCCESS] Total words: %d\n", countWordsUtil(root));
}

static void flushStdin(void) {
  int c;
  while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}
