#include "../../include/include/List_2.h"
#include <iostream>
#include <fstream>

using namespace std;

Node2* DoublyLinkedList::createNode(int value) {
    Node2* newNode = new Node2;
    newNode->key = value;
    newNode->next = nullptr;
    newNode->prev = nullptr;
    return newNode;
}

DoublyLinkedList::DoublyLinkedList() : head(nullptr), tail(nullptr) {}

DoublyLinkedList::~DoublyLinkedList() {
    clear();
}

void DoublyLinkedList::addToHead(int value) {
    Node2* newNode = createNode(value);

    if(head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    cout << "Добавлен в начало списка: " << value << endl;
}

void DoublyLinkedList::addToTail(int value) {
    Node2* newNode = createNode(value);

    if(tail == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    cout << "Добавлен в конец списка: " << value << endl;
}

void DoublyLinkedList::addAfterValue(int targetValue, int newValue) {
    if (head == nullptr) {
        cout << "Список пуст! Невозможно добавить после значения." << endl;
        return;
    }
    
    Node2* current = head;
    
    while (current != nullptr && current->key != targetValue) {
        current = current->next;
    }
    
    if (current == nullptr) {
        cout << "Элемент " << targetValue << " не найден в списке!" << endl;
        return;
    }
    
    Node2* newNode = createNode(newValue);
    
    newNode->next = current->next;
    newNode->prev = current;
    
    if (current->next != nullptr) {
        current->next->prev = newNode;
    } else {
        tail = newNode;
    }
    
    current->next = newNode;
    
    cout << "Добавлен элемент " << newValue << " после элемента " << targetValue << endl;
}

void DoublyLinkedList::addBeforeValue(int targetValue, int newValue) {
    if (head == nullptr) {
        cout << "Список пуст! Невозможно добавить перед значением." << endl;
        return;
    }
    
    Node2* current = head;
    
    while (current != nullptr && current->key != targetValue) {
        current = current->next;
    }
    
    if (current == nullptr) {
        cout << "Элемент " << targetValue << " не найден в списке!" << endl;
        return;
    }
    
    Node2* newNode = createNode(newValue);
    
    newNode->prev = current->prev;
    newNode->next = current;
    
    if (current->prev != nullptr) {
        current->prev->next = newNode;
    } else {
        head = newNode;
    }
    
    current->prev = newNode;
    
    cout << "Добавлен элемент " << newValue << " перед элементом " << targetValue << endl;
}

void DoublyLinkedList::deleteFromHead() {
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }
    
    Node2* temp = head;
    head = head->next;
    
    if (head != nullptr) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    
    cout << "Удален из начала: " << temp->key << endl;
    delete temp;
}

void DoublyLinkedList::deleteFromTail() {
    if (tail == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }
    
    Node2* temp = tail;
    tail = tail->prev;
    
    if (tail != nullptr) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    
    cout << "Удален из конца: " << temp->key << endl;
    delete temp;
}

void DoublyLinkedList::deleteByValue(int value) {
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }
    
    Node2* current = head;
    
    while (current != nullptr && current->key != value) {
        current = current->next;
    }
    
    if (current == nullptr) {
        cout << "Элемент " << value << " не найден!" << endl;
        return;
    }
    
    if (current->prev != nullptr) {
        current->prev->next = current->next;
    } else {
        head = current->next;
    }
    
    if (current->next != nullptr) {
        current->next->prev = current->prev;
    } else {
        tail = current->prev;
    }
    
    cout << "Удален элемент: " << current->key << endl;
    delete current;
}

void DoublyLinkedList::deleteAfterValue(int targetValue) {
    if (head == nullptr) {
        cout << "Список пуст! Невозможно удалить элемент." << endl;
        return;
    }
    
    Node2* current = head;
    
    while (current != nullptr && current->key != targetValue) {
        current = current->next;
    }
    
    if (current == nullptr) {
        cout << "Элемент " << targetValue << " не найден в списке!" << endl;
        return;
    }
    
    if (current->next == nullptr) {
        cout << "После элемента " << targetValue << " нет элементов для удаления!" << endl;
        return;
    }
    
    Node2* nodeToDelete = current->next;
    
    current->next = nodeToDelete->next;
    
    if (nodeToDelete->next != nullptr) {
        nodeToDelete->next->prev = current;
    } else {
        tail = current;
    }
    
    cout << "Удален элемент: " << nodeToDelete->key << " после элемента " << targetValue << endl;
    delete nodeToDelete;
}

// НОВЫЙ МЕТОД - удаление перед элементом
void DoublyLinkedList::deleteBeforeValue(int targetValue) {
    if (head == nullptr) {
        cout << "Список пуст! Невозможно удалить элемент." << endl;
        return;
    }
    
    Node2* current = head;
    
    while (current != nullptr && current->key != targetValue) {
        current = current->next;
    }
    
    if (current == nullptr) {
        cout << "Элемент " << targetValue << " не найден в списке!" << endl;
        return;
    }
    
    if (current->prev == nullptr) {
        cout << "Перед элементом " << targetValue << " нет элементов для удаления!" << endl;
        return;
    }
    
    Node2* nodeToDelete = current->prev;
    
    current->prev = nodeToDelete->prev;
    
    if (nodeToDelete->prev != nullptr) {
        nodeToDelete->prev->next = current;
    } else {
        head = current;
    }
    
    cout << "Удален элемент: " << nodeToDelete->key << " перед элементом " << targetValue << endl;
    delete nodeToDelete;
}

bool DoublyLinkedList::findByValue(int value) {
    Node2* current = head;
    while (current != nullptr) {
        if (current->key == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void DoublyLinkedList::printForward() {
    cout << "Список (начало -> конец): ";
    Node2* current = head;
    while (current != nullptr) {
        cout << current->key << " ";
        current = current->next;
    }
    cout << endl;
}

void DoublyLinkedList::printBackward() {
    cout << "Список (конец -> начало): ";
    Node2* current = tail;
    while (current != nullptr) {
        cout << current->key << " ";
        current = current->prev;
    }
    cout << endl;
}

void DoublyLinkedList::clear() {
    while (head != nullptr) {
        Node2* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
}

void DoublyLinkedList::saveToFile(const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        Node2* current = head;
        while(current != nullptr) {
            file << current->key << " ";
            current = current->next;
        }
        file.close();
        cout << "Двусвязный список сохранён в файл: " << filename << endl;
    }
}

void DoublyLinkedList::loadFromFile(const string& filename) {
    ifstream file(filename);
    if(file.is_open()) {
        clear();
        int value;
        while(file >> value) {
            addToTail(value);
        }
        file.close();
        cout << "Двусвязный список загружен из файла: " << filename << endl;
    }
}