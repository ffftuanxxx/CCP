// parse_tree.h

#ifndef PARSE_TREE_H
#define PARSE_TREE_H

// Define the structure for a node in the parse tree
typedef struct TreeNode {
    char value; // For simplicity, let's assume each node holds a character (operator or operand)
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Function to create a new node in the parse tree
TreeNode* createNode111(char value);

// Function to insert a new node with a given value as the left child of a parent node
void insertLeft(TreeNode* parent, char value);

// Function to insert a new node with a given value as the right child of a parent node
void insertRight(TreeNode* parent, char value);

// Function to delete a node and its subtree
void deleteTree(TreeNode* root);

// Function to print the parse tree in postfix notation
void printPostfix(TreeNode* root);

#endif // PARSE_TREE_H
