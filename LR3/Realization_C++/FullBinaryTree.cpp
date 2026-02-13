#include "FullBinaryTree.h"
#include <iostream>
#include <queue>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

TreeNode::TreeNode(int value) : data(value), left(nullptr), right(nullptr) {}

FullBinaryTree::FullBinaryTree() : root(nullptr) {}

FullBinaryTree::~FullBinaryTree() {
    clear();
}

void FullBinaryTree::clearRecursive(TreeNode* current) {
    if (current == nullptr) return;
    clearRecursive(current->left);
    clearRecursive(current->right);
    delete current;
}

bool FullBinaryTree::searchRecursive(TreeNode* current, int value) {
    if (current == nullptr) return false;
    if (current->data == value) return true;
    return searchRecursive(current->left, value) || searchRecursive(current->right, value);
}

int FullBinaryTree::countNodes(TreeNode* current) {
    if (current == nullptr) return 0;
    return 1 + countNodes(current->left) + countNodes(current->right);
}

int FullBinaryTree::getHeight(TreeNode* current) {
    if (current == nullptr) return 0;
    int leftHeight = getHeight(current->left);
    int rightHeight = getHeight(current->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

void FullBinaryTree::printInOrderRecursive(TreeNode* current) {
    if (current == nullptr) return;
    printInOrderRecursive(current->left);
    cout << current->data << " ";
    printInOrderRecursive(current->right);
}

void FullBinaryTree::printPreOrderRecursive(TreeNode* current) {
    if (current == nullptr) return;
    cout << current->data << " ";
    printPreOrderRecursive(current->left);
    printPreOrderRecursive(current->right);
}

void FullBinaryTree::printPostOrderRecursive(TreeNode* current) {
    if (current == nullptr) return;
    printPostOrderRecursive(current->left);
    printPostOrderRecursive(current->right);
    cout << current->data << " ";
}

void FullBinaryTree::add(int value) {
    if (root == nullptr) {
        root = new TreeNode(value);
        cout << "Добавлен корень: " << value << endl;
        return;
    }

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* temp = q.front();
        q.pop();

        if (temp->left == nullptr) {
            temp->left = new TreeNode(value);
            cout << "Добавлен элемент: " << value << endl;
            return;
        } else {
            q.push(temp->left);
        }

        if (temp->right == nullptr) {
            temp->right = new TreeNode(value);
            cout << "Добавлен элемент: " << value << endl;
            return;
        } else {
            q.push(temp->right);
        }
    }
}

bool FullBinaryTree::search(int value) {
    bool found = searchRecursive(root, value);
    if (found) {
        cout << "Элемент " << value << " найден!" << endl;
    } else {
        cout << "Элемент " << value << " не найден!" << endl;
    }
    return found;
}

bool FullBinaryTree::isFull() {
    if (root == nullptr) {
        cout << "Дерево пусто!" << endl;
        return true;
    }

    int nodeCount = countNodes(root);
    int height = getHeight(root);
    int expectedNodes = pow(2, height) - 1;

    if (nodeCount == expectedNodes) {
        cout << "Дерево является полным бинарным деревом!" << endl;
        return true;
    } else {
        cout << "Дерево НЕ является полным бинарным деревом." << endl;
        cout << "Узлов: " << nodeCount << ", Ожидается: " << expectedNodes << endl;
        return false;
    }
}

void FullBinaryTree::printInOrder() {
    if (root == nullptr) {
        cout << "Дерево пусто!" << endl;
        return;
    }
    cout << "In-Order обход: ";
    printInOrderRecursive(root);
    cout << endl;
}

void FullBinaryTree::printPreOrder() {
    if (root == nullptr) {
        cout << "Дерево пусто!" << endl;
        return;
    }
    cout << "Pre-Order обход: ";
    printPreOrderRecursive(root);
    cout << endl;
}

void FullBinaryTree::printPostOrder() {
    if (root == nullptr) {
        cout << "Дерево пусто!" << endl;
        return;
    }
    cout << "Post-Order обход: ";
    printPostOrderRecursive(root);
    cout << endl;
}

void FullBinaryTree::printLevelOrder() {
    if (root == nullptr) {
        cout << "Дерево пусто!" << endl;
        return;
    }

    cout << "Level-Order обход: ";
    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        cout << current->data << " ";

        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }
    cout << endl;
}

int FullBinaryTree::getNodeCount() {
    return countNodes(root);
}

int FullBinaryTree::getTreeHeight() {
    return getHeight(root);
}

void FullBinaryTree::clear() {
    clearRecursive(root);
    root = nullptr;
    cout << "Дерево очищено!" << endl;
}

void FullBinaryTree::saveToFile(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи!" << endl;
        return;
    }

    if (root == nullptr) {
        file.close();
        cout << "Дерево пусто, файл пуст." << endl;
        return;
    }

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        file << current->data << " ";

        if (current->left != nullptr) {
            q.push(current->left);
        }
        if (current->right != nullptr) {
            q.push(current->right);
        }
    }

    file.close();
    cout << "Дерево сохранено в файл: " << filename << endl;
}

void FullBinaryTree::loadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения!" << endl;
        return;
    }

    clear();

    int value;
    while (file >> value) {
        add(value);
    }

    file.close();
    cout << "Дерево загружено из файла: " << filename << endl;
}

// Бинарная сериализация (Level-Order для сохранения структуры дерева)
void FullBinaryTree::saveToBinaryFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if(!file.is_open()) {
        std::cout << "Ошибка открытия бинарного файла для записи!" << std::endl;
        return;
    }

    if(root == nullptr) {
        int empty = 0;
        file.write(reinterpret_cast<const char*>(&empty), sizeof(int));
        file.close();
        std::cout << "Пустое дерево сохранено в бинарный файл: " << filename << std::endl;
        return;
    }

    std::queue<TreeNode*> q;
    q.push(root);
    int nodeCount = getNodeCount();
    
    // Записываем количество узлов
    file.write(reinterpret_cast<const char*>(&nodeCount), sizeof(int));
    
    // Записываем узлы в порядке Level-Order
    while(!q.empty()) {
        TreeNode* current = q.front();
        q.pop();

        file.write(reinterpret_cast<const char*>(&current->data), sizeof(int));

        bool hasLeft = (current->left != nullptr);
        bool hasRight = (current->right != nullptr);
        file.write(reinterpret_cast<const char*>(&hasLeft), sizeof(bool));
        file.write(reinterpret_cast<const char*>(&hasRight), sizeof(bool));

        if(hasLeft) q.push(current->left);
        if(hasRight) q.push(current->right);
    }

    file.close();
    std::cout << "Дерево сохранено в бинарный файл: " << filename << std::endl;
}

void FullBinaryTree::loadFromBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if(!file.is_open()) {
        std::cout << "Ошибка открытия бинарного файла для чтения!" << std::endl;
        return;
    }

    clear();

    int nodeCount;
    file.read(reinterpret_cast<char*>(&nodeCount), sizeof(int));
    
    if(nodeCount == 0) {
        file.close();
        std::cout << "Пустое дерево загружено из бинарного файла: " << filename << std::endl;
        return;
    }
    
    std::queue<TreeNode*> q;
    int rootValue;
    bool hasLeft, hasRight;
    
    // Читаем корень
    file.read(reinterpret_cast<char*>(&rootValue), sizeof(int));
    file.read(reinterpret_cast<char*>(&hasLeft), sizeof(bool));
    file.read(reinterpret_cast<char*>(&hasRight), sizeof(bool));
    
    root = new TreeNode(rootValue);
    q.push(root);
    nodeCount--;
    
    while(!q.empty() && nodeCount > 0) {
        TreeNode* current = q.front();
        q.pop();
        
        // Читаем левого потомка
        if(hasLeft && nodeCount > 0) {
            int leftValue;
            bool leftHasLeft, leftHasRight;
            file.read(reinterpret_cast<char*>(&leftValue), sizeof(int));
            file.read(reinterpret_cast<char*>(&leftHasLeft), sizeof(bool));
            file.read(reinterpret_cast<char*>(&leftHasRight), sizeof(bool));
            
            current->left = new TreeNode(leftValue);
            q.push(current->left);
            nodeCount--;
        }
        
        // Читаем правого потомка
        if(hasRight && nodeCount > 0) {
            int rightValue;
            bool rightHasLeft, rightHasRight;
            file.read(reinterpret_cast<char*>(&rightValue), sizeof(int));
            file.read(reinterpret_cast<char*>(&rightHasLeft), sizeof(bool));
            file.read(reinterpret_cast<char*>(&rightHasRight), sizeof(bool));
            
            current->right = new TreeNode(rightValue);
            q.push(current->right);
            nodeCount--;
        }
        
        // Читаем флаги для следующего узла
        if(!q.empty() && nodeCount > 0) {
            file.read(reinterpret_cast<char*>(&hasLeft), sizeof(bool));
            file.read(reinterpret_cast<char*>(&hasRight), sizeof(bool));
        }
    }

    file.close();
    std::cout << "Дерево загружено из бинарного файла: " << filename << std::endl;
}