#include "../../include/include/Steck.h"
#include <iostream>
#include <fstream>

using namespace std;

Stack::Stack() : top(-1) {}

void Stack::push(int value) {
    if(top >= MAX_SIZE - 1) {
        cout << "Стек переполнен!" << endl;
        return;
    }
    key[++top] = value;
    cout << "Добавлен элемент: " << value << endl;
}

int Stack::pop() {
    if(isEmpty()) {
        cout << "Стек пуст!" << endl;
        return -1;
    }
    int value = key[top--];
    cout << "Удален элемент: " << value << endl;
    return value;
}

int Stack::peek() {
    if(isEmpty()) {
        cout << "Стек пуст!" << endl;
        return -1;
    }
    return key[top];
}

bool Stack::isEmpty() {
    return top == -1;
}

int Stack::size() {
    return top + 1;
}

void Stack::print() {
    if (isEmpty()) {
        cout << "Стек пуст" << endl;
        return;
    }
    cout << "Стек (сверху вниз): ";
    for (int i = top; i >= 0; i--) {
        cout << key[i] << " ";
    }
    cout << endl;
}

void Stack::clear() {
    top = -1;
}

void Stack::saveToFile(const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        file << size() << endl;
        for(int i = 0; i <= top; i++) {
            file << key[i] << " ";
        }
        file.close();
        cout << "Стек сохранён в файл: " << filename << endl;
    }
}

void Stack::loadFromFile(const string& filename) {
    ifstream file(filename);
    if(file.is_open()) {
        clear();
        int n;
        file >> n;
        for(int i = 0; i < n; i++) {
            int value;
            file >> value;
            if(top < MAX_SIZE - 1) {
                key[++top] = value;
            }
        }
        file.close();
        cout << "Стек загружен из файла: " << filename << endl;
    }
}
