#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 16

typedef struct Symbol {
    char name[640];
    char type[64];
} Symbol;

typedef struct Node {
    Symbol symbol;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

int height(Node *root);
void updateHeight(Node **proot);
void rightRotation(Node **proot);
void leftRotation(Node **proot);
void Rebalance(Node **proot);
Node *createNode(Symbol symbol);
Node *insertNode(Node **proot, Symbol symbol);
Symbol* findNode(Node *root, char name[100]);
Node *deleteMin(Node **proot);
Node *deleteRoot(Node **proot);
Node *deleteNode(Node **proot, char name[100]);
void destroyTree(Node *root);
void printSymbolTable(Node* root);


