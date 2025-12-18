#ifndef STECK_H
#define STECK_H

#include <string>

struct Stack {
private:
    static const int MAX_SIZE = 100;
    int key[MAX_SIZE];
    int top;

public:
    Stack();
    
    void push(int value);
    int pop();
    int peek();
    bool isEmpty();
    int size();
    void print();
    void clear();
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};

#endif
