#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <string>

class DynamicArray {
private:
    std::string* data;
    int size;
    int capacity;
    
    void resize(int newCapacity);
    
public:
    DynamicArray(int initialCapacity = 10);
    ~DynamicArray();
    
    void pushBack(const std::string& value);
    void insert(int index, const std::string& value);
    void remove(int index);
    std::string get(int index);
    void set(int index, const std::string& value);
    int getSize();
    int getCapacity();
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