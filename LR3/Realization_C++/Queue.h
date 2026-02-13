#ifndef QUEUE_H
#define QUEUE_H

#include <string>

class Queue {
private:
    int* arr;
    size_t capacity;
    size_t front_idx;
    size_t rear_idx;
    size_t current_size;
    
    void resize();
    
public:
    Queue();
    ~Queue();
    Queue(const Queue& other);
    Queue& operator=(const Queue& other);
    
    void push(int value);
    void pop();
    int front();
    bool isEmpty();
    size_t size();
    void display();
    void clear();
    
    // Текстовая сериализация
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    
    // Бинарная сериализация
    void saveToBinaryFile(const std::string& filename);
    void loadFromBinaryFile(const std::string& filename);
};

#endif