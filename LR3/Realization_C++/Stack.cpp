#include "Stack.h"
#include <iostream>
#include <fstream>

using namespace std;

Stack::Stack() : top_idx(-1) {}

void Stack::push(int value) {
    if (top_idx >= MAX_SIZE - 1) {
        cout << "Стек переполнен!" << endl;
        return;
    }
    data[++top_idx] = value;
}

int Stack::pop() {
    if (isEmpty()) {
        cout << "Стек пуст!" << endl;
        return -1;
    }
    return data[top_idx--];
}

int Stack::peek() {
    if (isEmpty()) {
        return -1;
    }
    return data[top_idx];
}

bool Stack::isEmpty() {
    return top_idx == -1;
}

int Stack::size() {
    return top_idx + 1;
}

void Stack::print() {
    if (isEmpty()) {
        cout << "Стек пуст!" << endl;
        return;
    }
    cout << "Стек (сверху вниз): ";
    for (int i = top_idx; i >= 0; i--) {
        cout << data[i] << " ";
    }
    cout << endl;
}

void Stack::clear() {
    top_idx = -1;
}

void Stack::saveToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i <= top_idx; i++) {
            file << data[i] << " ";
        }
        file.close();
        cout << "Стек сохранен в файл: " << filename << endl;
    } else {
        cout << "Ошибка открытия файла: " << filename << endl;
    }
}

void Stack::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        clear();
        int value;
        while (file >> value) {
            push(value);
        }
        file.close();
        cout << "Стек загружен из файла: " << filename << endl;
    } else {
        cout << "Ошибка открытия файла: " << filename << endl;
    }
}

// Бинарная сериализация
void Stack::saveToBinaryFile(const string& filename) {
    ofstream file(filename, ios::binary);
    if (file.is_open()) {
        int stackSize = size();
        file.write(reinterpret_cast<const char*>(&stackSize), sizeof(int));
        
        for (int i = 0; i <= top_idx; i++) {
            file.write(reinterpret_cast<const char*>(&data[i]), sizeof(int));
        }
        
        file.close();
        cout << "Стек сохранен в бинарный файл: " << filename << endl;
    } else {
        cout << "Ошибка открытия бинарного файла: " << filename << endl;
    }
}

void Stack::loadFromBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (file.is_open()) {
        clear();
        int stackSize;
        file.read(reinterpret_cast<char*>(&stackSize), sizeof(int));
        
        for (int i = 0; i < stackSize; i++) {
            int value;
            file.read(reinterpret_cast<char*>(&value), sizeof(int));
            push(value);
        }
        
        file.close();
        cout << "Стек загружен из бинарного файла: " << filename << endl;
    } else {
        cout << "Ошибка открытия бинарного файла: " << filename << endl;
    }
}