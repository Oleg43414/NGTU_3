#ifndef SINGLYLINKEDLIST_H
#define SINGLYLINKEDLIST_H

#include <string>

class Node {
public:
    int key;
    Node* next;
};

class SinglyLinkedList {
private:
    Node* head;
    
    Node* createNode(int value);
    
public:
    SinglyLinkedList();
    ~SinglyLinkedList();
    
    void addToHead(int value);
    void addToTail(int value);
    void addAfter(int targetValue, int newValue);
    void addBefore(int targetValue, int newValue);
    void deleteFromHead();
    void deleteFromTail();
    void deleteByValue(int value);
    void deleteAfter(int targetValue);
    void deleteBefore(int targetValue);
    bool findByValue(int value);
    void print();
    void printReverse();
    void clear();
    
    // Текстовая сериализация
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
    // Бинарная сериализация
    void saveToBinaryFile(const std::string& filename);
    void loadFromBinaryFile(const std::string& filename);
};

#endif