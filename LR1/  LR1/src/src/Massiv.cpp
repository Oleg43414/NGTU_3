#include "../../include/include/Massiv.h"
#include <iostream>
#include <fstream>

using namespace std;

void Arry::resize(int newCapacity) {
    int* newKey = new int[newCapacity];
    for(int i = 0; i < size; i++) {
        newKey[i] = key[i];
    }
    delete[] key;
    key = newKey;
    capacity = newCapacity;
}

Arry::Arry(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    key = new int[capacity];
}

Arry::~Arry() {
    delete[] key;
}

void Arry::pushBack(int value) {
    if(size >= capacity) {
        resize(capacity * 2);
    }
    key[size++] = value;
}

void Arry::insert(int index, int value) {
    if(index < 0 || index > size) {
        cout << "Неверный индекс!" << endl;
        return;
    }
    if(size >= capacity) {
        resize(capacity * 2);
    }
    for(int i = size; i > index; i--) {
        key[i] = key[i - 1];
    }
    key[index] = value;
    size++;
}

void Arry::remove(int index) {
    if(index < 0 || index >= size) {
        cout << "Неверный элемент" << endl;
        return;
    }
    for(int i = index; i < size-1; i++) {
        key[i] = key[i + 1];
    }
    size--;
    if(size < capacity / 4 && capacity > 10) {
        resize(capacity / 2);
    }
}

int Arry::get(int index) {
    if(index < 0 || index >= size){
        cout << "Неверный индекс" << endl;
        return -1;
    }
    return key[index];
}

void Arry::set(int index, int value) {
    if(index < 0 || index >= size) {
        cout << "Неверный индекс" << endl;
        return;
    }
    key[index] = value;
}

int Arry::getSize() {
    return size;
}

void Arry::print() {
    cout << "Массив[" << size << "/" << capacity << "]: ";
    for(int i = 0; i < size; i++) {
        cout << key[i] << " ";
    }
    cout << endl;
}

void Arry::clear() {
    size = 0;
}

void Arry::saveToFile(const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        file << size << endl;
        for(int i = 0; i < size; i++) {
            file << key[i] << " ";
        }
        file.close();
        cout << "Массив сохранён в файл: " << filename << endl;
    }
}

void Arry::loadFromFile(const string& filename) {
    ifstream file(filename);
    if(file.is_open()) {
        clear();
        int n;
        file >> n;
        for(int i = 0; i < n; i++) {
            int value;
            file >> value;
            pushBack(value);
        }
        file.close();
        cout << "Массив загружен из файла: " << filename << endl;
    }
}
