#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#define SIZE 16

int height(Node *root){
    if (root == NULL){
        return -1;
    }
    return root->height;
}

void updateHeight(Node **proot){
    if(*proot == NULL) return;
    int leftHeight = height((*proot)->left);
    int rightHeight = height((*proot)->right);
    if (leftHeight > rightHeight){
        (*proot)->height = leftHeight + 1;
    }
    else{
        (*proot)->height = rightHeight + 1;
    }
}

void rightRotation(Node **proot){
    Node *k2 = *proot, *k1 = k2->left, *y = k1->right;
    *proot = k1;
    k1->right = k2;
    k2->left = y;
    updateHeight(&k2);
    updateHeight(&k1);
}

void leftRotation(Node **proot){
    Node *k2 = *proot, *k1 = k2->right, *y = k1->left;
    *proot = k1;
    k1->left = k2;
    k2->right = y;
    updateHeight(&k2);
    updateHeight(&k1);
}

void Rebalance(Node **proot){
    if(*proot == NULL) return;

    int rightHeight = height((*proot)->right);
    int leftHeight = height((*proot)->left);

    if(abs(rightHeight - leftHeight) < 2) return;

    // case 1 & case 2: left-
    if(leftHeight > rightHeight){
        // left-left
        if(height((*proot)->left->left) >= height((*proot)->left->right)){
            rightRotation(proot);
        }
        // left-right
        else{
            leftRotation(&(*proot)->left);
            rightRotation(proot);
        }
    }
    // case 3 & case 4: right-
    else{
        // right-right
        if(height((*proot)->right->left) <= height((*proot)->right->right)){
            leftRotation(proot);
        }
        // right-left
        else{
            rightRotation(&(*proot)->right);
            leftRotation(proot);
        }
    }
}

Node *createNode(Symbol symbol) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        // 内存分配失败处理
        return NULL;
    }
    strcpy(newNode->symbol.name, symbol.name);
    strcpy(newNode->symbol.type, symbol.type);
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->height = 0;
    return newNode;
}

// 插入节点
Node *insertNode(Node **proot, Symbol symbol) {
    if (*proot == NULL) {
        return *proot = createNode(symbol);
    }
    Node *newNode = NULL;
    if(strcmp((*proot)->symbol.name, symbol.name) == 0) {
        // 名称相同的符号处理
        return NULL;
    }
    else if (strcmp((*proot)->symbol.name, symbol.name) > 0) {
        newNode = insertNode(&(*proot)->left, symbol);
    }
    else {
        newNode = insertNode(&(*proot)->right, symbol);
    }

    return newNode;
}

Symbol* findNode(Node *root, char name[100]){
    if(root == NULL){
        return NULL;
    }
    if(strcmp(root->symbol.name,name) == 0){//root->key == x
        return &(root->symbol);
    }
    if(strcmp(root->symbol.name,name) > 0){//root->key > x
        return findNode(root->left, name);
    }
    return findNode(root->right, name);
}

Node *deleteMin(Node **proot){
    if(*proot == NULL) return NULL;
    Node *minNode = NULL;
    if((*proot)->left == NULL){
        minNode = *proot;
        *proot = (*proot)->right;
        return minNode;
    }
    minNode = deleteMin(&(*proot)->left);
	updateHeight(proot);
	Rebalance(proot);
	return minNode;
}

Node *deleteRoot(Node **proot){
	if(*proot == NULL) return NULL;
    Node *currNode = *proot;
    if((*proot)->left == NULL){
        *proot = (*proot)->right;
        return currNode;
    }
    else if((*proot)->right == NULL){
        *proot = (*proot)->left;
        return currNode;
    }
    Node *minNode = deleteMin(&(*proot)->right);
    minNode->left = (*proot)->left;
    minNode->right = (*proot)->right;
    (*proot) = minNode;
    return currNode;
}

Node *deleteNode(Node **proot, char name[100]){
    if(*proot == NULL) return NULL;
    Node* matchNode = NULL;
    if(strcmp((*proot)->symbol.name,name) > 0){//(*proot)->key > x
        matchNode = deleteNode(&(*proot)->left, name);
    }
    else if(strcmp((*proot)->symbol.name,name) < 0){//(*proot)->key < x
        matchNode = deleteNode(&(*proot)->right, name);
    }
    else{
        matchNode = deleteRoot(proot);
    }
    updateHeight(proot);
    Rebalance(proot);
    return matchNode;
}

void destroyTree(Node *root){
    if(root == NULL) return;
    if(root->left != NULL){
        destroyTree(root->left);
    }
    if(root->right != NULL){
        destroyTree(root->right);
    }
    free(root);
}

void printSymbolTable(Node* root) {
    // printf("-----SymbolTable-----");
    if (root != NULL) {
        printSymbolTable(root->left);
        printf("%s\t\t\t%s\n", root->symbol.name, root->symbol.type);
        printSymbolTable(root->right);
    }
    // printf("---------------------");
}

//int main(void) {
//    Node *symbolTable = NULL;
//    printSymbolTable(symbolTable);
//    printf("After inserting:\n");
//    Symbol symbol1 = {"x", "natural numbers"};
//    insertNode(&symbolTable, symbol1);
//    printSymbolTable(symbolTable);
//    Symbol symbol2 = {"y", "Boolean"};
//    insertNode(&symbolTable, symbol2);
//    printSymbolTable(symbolTable);
//    Symbol symbol3 = {"z", "functions"};
//    insertNode(&symbolTable, symbol3);
//    printSymbolTable(symbolTable);
//    printf(strcmp(findNode(symbolTable, "x")->name,"x") == 0 ? "true\n" : "false\n");
//    printf(strcmp(findNode(symbolTable, "y")->name,"y") == 0 ? "true\n" : "false\n");
//    printf(strcmp(findNode(symbolTable, "z")->name,"z") == 0 ? "true\n" : "false\n");
//    printf("delete\n");
//    // printf(strcmp(deleteNode(&symbolTable, "y")->symbol.name,"y") == 0 ? "true\n" : "false\n");
//    free(deleteNode(&symbolTable, "y"));
//    printSymbolTable(symbolTable);
//
//    destroyTree(symbolTable);
//
//    return 0;
//}



