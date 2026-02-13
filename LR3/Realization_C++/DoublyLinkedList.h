#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <string>

class DoublyNode {
public:
    int key;
    DoublyNode* next;
    DoublyNode* prev;
};

class DoublyLinkedList {
private:
    DoublyNode* head;
    DoublyNode* tail;
    
    DoublyNode* createNode(int value);
    
public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    
    void addToHead(int value);
    void addToTail(int value);
    void addAfterValue(int targetValue, int newValue);
    void addBeforeValue(int targetValue, int newValue);
    void deleteFromHead();
    void deleteFromTail();
    void deleteByValue(int value);
    void deleteAfterValue(int targetValue);
    void deleteBeforeValue(int targetValue);
    bool findByValue(int value);
    void printForward();
    void printBackward();
    void clear();
    
    // Текстовая сериализация
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
    // Бинарная сериализация
    void saveToBinaryFile(const std::string& filename);
    void loadFromBinaryFile(const std::string& filename);
};

#endif 