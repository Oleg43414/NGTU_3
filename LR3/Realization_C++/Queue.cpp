#include "Queue.h"
#include <iostream>
#include <fstream>

using namespace std;

Queue::Queue() : arr(nullptr), capacity(0), front_idx(0), rear_idx(0), current_size(0) {
    resize();
}

Queue::~Queue() {
    delete[] arr;
}

Queue::Queue(const Queue& other) : arr(nullptr), capacity(0), front_idx(0), rear_idx(0), current_size(0) {
    *this = other;
}

Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        delete[] arr;
        capacity = other.capacity;
        current_size = other.current_size;
        front_idx = 0;
        rear_idx = 0;
        
        arr = new int[capacity];
        size_t i = 0;
        size_t j = other.front_idx;
        while (i < current_size) {
            arr[i] = other.arr[j];
            j = (j + 1) % capacity;
            i++;
        }
        rear_idx = current_size;
    }
    return *this;
}

void Queue::resize() {
    size_t new_capacity = capacity == 0 ? 4 : capacity * 2;
    int* new_arr = new int[new_capacity];
    
    size_t i = 0;
    size_t j = front_idx;
    while (i < current_size) {
        new_arr[i] = arr[j];
        j = (j + 1) % capacity;
        i++;
    }
    
    delete[] arr;
    arr = new_arr;
    capacity = new_capacity;
    front_idx = 0;
    rear_idx = current_size;
}

void Queue::push(int value) {
    if (current_size == capacity) {
        resize();
    }
    
    arr[rear_idx] = value;
    rear_idx = (rear_idx + 1) % capacity;
    current_size++;
    
    cout << "Добавлен элемент: " << value << endl;
}

void Queue::pop() {
    if (isEmpty()) {
        cout << "Очередь пуста! Нельзя удалить элемент." << endl;
        return;
    }
    
    int frontValue = arr[front_idx];
    front_idx = (front_idx + 1) % capacity;
    current_size--;
    
    cout << "Удален элемент: " << frontValue << endl;
}

int Queue::front() {
    if (isEmpty()) {
        cout << "Очередь пуста!" << endl;
        return -1;
    }
    return arr[front_idx];
}

bool Queue::isEmpty() {
    return current_size == 0;
}

size_t Queue::size() {
    return current_size;
}

void Queue::display() {
    if (isEmpty()) {
        cout << "Очередь пуста!" << endl;
        return;
    }

    cout << "Содержимое очереди: ";
    size_t idx = front_idx;
    for (size_t i = 0; i < current_size; i++) {
        cout << arr[idx] << " ";
        idx = (idx + 1) % capacity;
    }
    cout << endl;
}

void Queue::clear() {
    front_idx = 0;
    rear_idx = 0;
    current_size = 0;
    cout << "Очередь очищена!" << endl;
}

void Queue::saveToFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        size_t idx = front_idx;
        for (size_t i = 0; i < current_size; i++) {
            file << arr[idx] << " ";
            idx = (idx + 1) % capacity;
        }
        file.close();
        cout << "Очередь сохранена в файл: " << filename << endl;
    } else {
        cout << "Ошибка открытия файла: " << filename << endl;
    }
}

void Queue::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        clear();
        int value;
        while (file >> value) {
            push(value);
        }
        file.close();
        cout << "Очередь загружена из файла: " << filename << endl;
    } else {
        cout << "Ошибка открытия файла: " << filename << endl;
    }
}

// Бинарная сериализация
void Queue::saveToBinaryFile(const string& filename) {
    ofstream file(filename, ios::binary);
    if(file.is_open()) {
        size_t queueSize = current_size;
        file.write(reinterpret_cast<const char*>(&queueSize), sizeof(size_t));
        
        size_t idx = front_idx;
        for(size_t i = 0; i < current_size; i++) {
            file.write(reinterpret_cast<const char*>(&arr[idx]), sizeof(int));
            idx = (idx + 1) % capacity;
        }
        
        file.close();
        cout << "Очередь сохранена в бинарный файл: " << filename << endl;
    } else {
        cout << "Ошибка открытия бинарного файла для записи: " << filename << endl;
    }
}

void Queue::loadFromBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if(file.is_open()) {
        clear();
        
        size_t queueSize;
        file.read(reinterpret_cast<char*>(&queueSize), sizeof(size_t));
        
        for(size_t i = 0; i < queueSize; i++) {
            int value;
            file.read(reinterpret_cast<char*>(&value), sizeof(int));
            push(value);
        }
        
        file.close();
        cout << "Очередь загружена из бинарного файла: " << filename << endl;
    } else {
        cout << "Ошибка открытия бинарного файла для чтения: " << filename << endl;
    }
}