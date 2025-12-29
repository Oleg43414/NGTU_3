#include "../../include/include/Queue.h"
#include <iostream>
#include <fstream>

using namespace std;

void Queue::push(int value) {
    data.push(value);
    cout << "Добавлен элемент: " << value << endl;
}

void Queue::pop() {
    if (!data.empty()) {
        int frontValue = data.front();
        data.pop();
        cout << "Удален элемент: " << frontValue << endl;
    } else {
        cout << "Очередь пуста! Нельзя удалить элемент." << endl;
    }
}

int Queue::front() {
    if (!data.empty()) {
        return data.front();
    } else {
        cout << "Очередь пуста!" << endl;
        return -1;
    }
}

bool Queue::isEmpty() {
    return data.empty();
}

size_t Queue::size() {
    return data.size();
}

void Queue::display() {
    if (data.empty()) {
        cout << "Очередь пуста!" << endl;
        return;
    }

    cout << "Содержимое очереди: ";
    queue<int> temp = data;
    while (!temp.empty()) {
        cout << temp.front() << " ";
        temp.pop();
    }
    cout << endl;
}

void Queue::clear() {
    while(!data.empty()) {
        data.pop();
    }
}

void Queue::saveToFile(const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        queue<int> temp = data;
        while(!temp.empty()) {
            file << temp.front() << " ";
            temp.pop();
        }
        file.close();
        cout << "Очередь сохранена в файл: " << filename << endl;
    }
}

void Queue::loadFromFile(const string& filename) {
    ifstream file(filename);
    if(file.is_open()) {
        clear();
        int value;
        while(file >> value) {
            data.push(value);
        }
        file.close();
        cout << "Очередь загружена из файла: " << filename << endl;
    }
}
