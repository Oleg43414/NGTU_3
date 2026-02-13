#include "DynamicArray.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void DynamicArray::resize(int newCapacity) {
    string* newData = new string[newCapacity];
    for(int i = 0; i < size; i++) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

DynamicArray::DynamicArray(int initialCapacity) {
    capacity = initialCapacity;
    size = 0;
    data = new string[capacity];
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

void DynamicArray::pushBack(const string& value) {
    if(size >= capacity) {
        resize(capacity * 2);
    }
    data[size++] = value;
}

void DynamicArray::insert(int index, const string& value) {
    if(index < 0 || index > size) {
        cout << "Неверный индекс!" << endl;
        return;
    }
    if(size >= capacity) {
        resize(capacity * 2);
    }
    for(int i = size; i > index; i--) {
        data[i] = data[i - 1];
    }
    data[index] = value;
    size++;
}

void DynamicArray::remove(int index) {
    if(index < 0 || index >= size) {
        cout << "Неверный индекс" << endl;
        return;
    }
    for(int i = index; i < size-1; i++) {
        data[i] = data[i + 1];
    }
    size--;
    if(size < capacity / 4 && capacity > 10) {
        resize(capacity / 2);
    }
}

string DynamicArray::get(int index) {
    if(index < 0 || index >= size){
        cout << "Неверный индекс" << endl;
        return "";
    }
    return data[index];
}

void DynamicArray::set(int index, const string& value) {
    if(index < 0 || index >= size) {
        cout << "Неверный индекс" << endl;
        return;
    }
    data[index] = value;
}

int DynamicArray::getSize() {
    return size;
}

int DynamicArray::getCapacity() {
    return capacity;
}

void DynamicArray::print() {
    cout << "Массив[" << size << "/" << capacity << "]: ";
    for(int i = 0; i < size; i++) {
        cout << "\"" << data[i] << "\" ";
    }
    cout << endl;
}

void DynamicArray::clear() {
    size = 0;
}

void DynamicArray::saveToFile(const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        file << size << endl;
        for(int i = 0; i < size; i++) {
            file << data[i] << endl;
        }
        file.close();
        cout << "Массив сохранён в файл: " << filename << endl;
    } else {
        cout << "Ошибка создания файла: " << filename << endl;
    }
}

void DynamicArray::loadFromFile(const string& filename) {
    ifstream file(filename);
    if(file.is_open()) {
        clear();
        int n;
        file >> n;
        string dummy;
        getline(file, dummy);
        
        for(int i = 0; i < n; i++) {
            string value;
            getline(file, value);
            if (!value.empty() && value.back() == '\n') {
                value.pop_back();
            }
            if (!value.empty() && value.back() == '\r') {
                value.pop_back();
            }
            pushBack(value);
        }
        file.close();
        cout << "Массив загружен из файла: " << filename << endl;
    } else {
        cout << "Ошибка открытия файла: " << filename << endl;
    }
}

// Бинарная сериализация
void DynamicArray::saveToBinaryFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if(file.is_open()) {
        file.write(reinterpret_cast<const char*>(&size), sizeof(int));
        for(int i = 0; i < size; i++) {
            int strLength = data[i].length();
            file.write(reinterpret_cast<const char*>(&strLength), sizeof(int));
            file.write(data[i].c_str(), strLength);
        }
        
        file.close();
        std::cout << "Динамический массив сохранён в бинарный файл: " << filename << std::endl;
    } else {
        std::cout << "Ошибка открытия бинарного файла для записи: " << filename << std::endl;
    }
}

void DynamicArray::loadFromBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if(file.is_open()) {
        clear();
        
        int fileSize;
        file.read(reinterpret_cast<char*>(&fileSize), sizeof(int));
        
        for(int i = 0; i < fileSize; i++) {
            int strLength;
            file.read(reinterpret_cast<char*>(&strLength), sizeof(int));
            
            char* buffer = new char[strLength + 1];
            file.read(buffer, strLength);
            buffer[strLength] = '\0';
            
            pushBack(std::string(buffer));
            delete[] buffer;
        }
        
        file.close();
        std::cout << "Динамический массив загружен из бинарного файла: " << filename << std::endl;
    } else {
        std::cout << "Ошибка открытия бинарного файла для чтения: " << filename << std::endl;
    }
}