#ifndef FULLBINARYTREE_H
#define FULLBINARYTREE_H

#include <string>

struct TreeNode {
    int data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int value);
};

struct FullBinaryTree {
private:
    TreeNode* root;
    
    void clearRecursive(TreeNode* current);
    bool searchRecursive(TreeNode* current, int value);
    int countNodes(TreeNode* current);
    int getHeight(TreeNode* current);
    void printInOrderRecursive(TreeNode* current);
    void printPreOrderRecursive(TreeNode* current);
    void printPostOrderRecursive(TreeNode* current);

public:
    FullBinaryTree();
    ~FullBinaryTree();
    
    void add(int value);
    bool search(int value);
    bool isFull();
    void printInOrder();
    void printPreOrder();
    void printPostOrder();
    void printLevelOrder();
    int getNodeCount();
    int getTreeHeight();
    void clear();
    void saveToFile(std::string filename);
    void loadFromFile(std::string filename);
};

#endif
