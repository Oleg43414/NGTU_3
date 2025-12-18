#ifndef MASSIV_H
#define MASSIV_H

#include <string>

struct Arry {
private:
    int* key;
    int capacity;
    int size;
    void resize(int newCapacity);

public:
    Arry(int initialCapacity = 10);
    ~Arry();
    
    void pushBack(int value);
    void insert(int index, int value);
    void remove(int index);
    int get(int index);
    void set(int index, int value);
    int getSize();
    void print();
    void clear();
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
};

#endif
