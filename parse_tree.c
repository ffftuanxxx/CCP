#include <stdio.h>
#include <stdlib.h>
#include "parse_tree.h"

// Function to create a new node in the parse tree
TreeNode* createNode111(char value) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->value = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Function to print the parse tree in postfix notation
void printPostfix(TreeNode* root) {
    if (root != NULL) {
        printPostfix(root->left);
        printPostfix(root->right);
        printf("%c ", root->value);
    }
}

void insertLeft(TreeNode* parent, char value) {
    if (parent != NULL) {
        parent->left = createNode111(value);
    }
}

// Function to insert a new node with a given value as the right child of a parent node
void insertRight(TreeNode* parent, char value) {
    if (parent != NULL) {
        parent->right = createNode111(value);
    }
}

// Function to delete a node and its subtree
void deleteTree(TreeNode* root) {
    if (root != NULL) {
        deleteTree(root->left);
        deleteTree(root->right);
        free(root);
    }
}