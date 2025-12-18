#ifndef QUEUE_H
#define QUEUE_H

#include <queue>
#include <string>

struct Queue {
private:
    std::queue<int> data;

public:
    void push(int value);
    void pop();
    int front();
    bool isEmpty();
    size_t size();
    void display();
    void clear();
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};

#endif
