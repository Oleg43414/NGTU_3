#ifndef LIST_2_H
#define LIST_2_H

#include <string>

struct Node2 {
    int key;
    Node2* next;
    Node2* prev;
};

class DoublyLinkedList {
private:
    Node2* head;
    Node2* tail;
    Node2* createNode(int value);

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
    void deleteBeforeValue(int targetValue); // ДОБАВЬ ЭТУ СТРОКУ
    
    bool findByValue(int value);
    void printForward();
    void printBackward();
    void clear();
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};

#endif