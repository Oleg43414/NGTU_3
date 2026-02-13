#ifndef STACK_H
#define STACK_H

#include <string>

class Stack {
private:
    static const int MAX_SIZE = 100;
    int data[MAX_SIZE];
    int top_idx;
    
public:
    Stack();
    
    void push(int value);
    int pop();
    int peek();
    bool isEmpty();
    int size();
    void print();
    void clear();
    
    // Текстовая сериализация
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
    // Бинарная сериализация
    void saveToBinaryFile(const std::string& filename);
    void loadFromBinaryFile(const std::string& filename);
};

#endif