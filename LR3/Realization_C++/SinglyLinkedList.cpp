#include "SinglyLinkedList.h"
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

Node* SinglyLinkedList::createNode(int value) {
    Node* newNode = new Node;
    newNode->key = value;
    newNode->next = nullptr;
    return newNode;
}

SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}

SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

void SinglyLinkedList::addToHead(int value) {
    Node* newNode = createNode(value);
    newNode->next = head;
    head = newNode;
    cout << "Добавлен в начало: " << value << endl;
}

void SinglyLinkedList::addToTail(int value) {
    Node* newNode = createNode(value);
    
    if(head == nullptr) {
        head = newNode;
        cout << "Добавлен в конец: " << value << endl;
        return;
    }

    Node* current = head;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;
    cout << "Добавлен в конец: " << value << endl;
}

void SinglyLinkedList::addAfter(int targetValue, int newValue) {
    if (head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }

    Node* current = head;
    while (current != nullptr && current->key != targetValue) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << targetValue << " не найден!" << endl;
        return;
    }

    Node* newNode = createNode(newValue);
    newNode->next = current->next;
    current->next = newNode;
    cout << "Добавлен после " << targetValue << ": " << newValue << endl;
}

void SinglyLinkedList::addBefore(int targetValue, int newValue) {
    if (head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }

    if (head->key == targetValue) {
        addToHead(newValue);
        return;
    }

    Node* current = head;
    while (current->next != nullptr && current->next->key != targetValue) {
        current = current->next;
    }

    if (current->next == nullptr) {
        cout << targetValue << " не найден!" << endl;
        return;
    }

    Node* newNode = createNode(newValue);
    newNode->next = current->next;
    current->next = newNode;
    cout << "Добавлен перед " << targetValue << ": " << newValue << endl;
}

void SinglyLinkedList::deleteBefore(int targetValue) {
    if (head == nullptr || head->next == nullptr) {
        cout << "Недостаточно элементов для удаления!" << endl;
        return;
    }

    if (head->key == targetValue) {
        cout << "Невозможно удалить элемент перед первым элементом!" << endl;
        return;
    }

    if (head->next->key == targetValue) {
        Node* temp = head;
        head = head->next;
        cout << "Удален элемент: " << temp->key << " перед элементом " << targetValue << endl;
        delete temp;
        return;
    }

    Node* current = head;
    while (current->next->next != nullptr && current->next->next->key != targetValue) {
        current = current->next;
    }

    if (current->next->next == nullptr) {
        cout << "Элемент " << targetValue << " не найден!" << endl;
        return;
    }

    Node* nodeToDelete = current->next;
    current->next = current->next->next;
    cout << "Удален элемент: " << nodeToDelete->key << " перед элементом " << targetValue << endl;
    delete nodeToDelete;
}

void SinglyLinkedList::deleteAfter(int targetValue) {
    if (head == nullptr) {
        cout << "Список пуст! Невозможно удалить элемент." << endl;
        return;
    }
    
    Node* current = head;
    
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
    
    Node* nodeToDelete = current->next;
    current->next = current->next->next;
    
    cout << "Удален элемент: " << nodeToDelete->key << " после элемента " << targetValue << endl;
    delete nodeToDelete;
}

void SinglyLinkedList::print() {
    if (head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    Node* current = head;
    while (current != nullptr) {
        cout << current->key << " -> ";
        current = current->next;
    }
    cout << "nullptr" << endl;
}

void SinglyLinkedList::printReverse() {
    if (head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    stack<int> st;
    Node* current = head;
    
    while (current != nullptr) {
        st.push(current->key);
        current = current->next;
    }
    
    cout << "Список в обратном порядке: ";
    while (!st.empty()) {
        cout << st.top() << " -> ";
        st.pop();
    }
    cout << "nullptr" << endl;
}

void SinglyLinkedList::deleteFromHead() {
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }
    Node* temp = head;
    head = head->next;
    cout << "Удален из начала: " << temp->key << endl;
    delete temp;
}

void SinglyLinkedList::deleteFromTail() {
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }
    
    if (head->next == nullptr) {
        cout << "Удален из конца: " << head->key << endl;
        delete head;
        head = nullptr;
        return;
    }
    
    Node* current = head;
    while (current->next->next != nullptr) {
        current = current->next;
    }
    
    Node* temp = current->next;
    current->next = nullptr;
    cout << "Удален из конца: " << temp->key << endl;
    delete temp;
}

void SinglyLinkedList::deleteByValue(int value) {
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }

    if (head->key == value) {
        Node* temp = head;
        head = head->next;
        cout << "Удален элемент: " << temp->key << endl;
        delete temp;
        return;
    }
    
    Node* current = head;
    Node* prev = nullptr;
    
    while (current != nullptr && current->key != value) {
        prev = current;
        current = current->next;
    }
    
    if (current == nullptr) {
        cout << "Элемент " << value << " не найден!" << endl;
        return;
    }
    
    if (prev != nullptr) {
        prev->next = current->next;
    }
    
    cout << "Удален элемент: " << current->key << endl;
    delete current;
}

bool SinglyLinkedList::findByValue(int value) {
    Node* current = head;
    while (current != nullptr) {
        if (current->key == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

void SinglyLinkedList::clear() {
    while (head != nullptr){
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

void SinglyLinkedList::saveToFile(const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        Node* current = head;
        while(current != nullptr) {
            file << current->key << " ";
            current = current->next;
        }
        file.close();
        cout << "Односвязный список сохранён в файл: " << filename << endl;
    }
}

void SinglyLinkedList::loadFromFile(const string& filename) {
    ifstream file(filename);
    if(file.is_open()) {
        clear();
        int value;
        while(file >> value) {
            addToTail(value);
        }
        file.close();
        cout << "Односвязный список загружен из файла: " << filename << endl;
    }
}

void SinglyLinkedList::saveToBinaryFile(const string& filename) {
    ofstream file(filename, ios::binary);
    if(file.is_open()) {
        Node* current = head;
        int count = 0;
        
        while(current != nullptr) {
            count++;
            current = current->next;
        }
        
        file.write(reinterpret_cast<const char*>(&count), sizeof(int));
        
        current = head;
        while(current != nullptr) {
            file.write(reinterpret_cast<const char*>(&current->key), sizeof(int));
            current = current->next;
        }
        
        file.close();
        cout << "Односвязный список сохранён в бинарный файл: " << filename << endl;
    } else {
        cout << "Ошибка открытия бинарного файла для записи: " << filename << endl;
    }
}

void SinglyLinkedList::loadFromBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if(file.is_open()) {
        clear();
        
        int count = 0;
        file.read(reinterpret_cast<char*>(&count), sizeof(int));
        
        for(int i = 0; i < count; i++) {
            int value;
            file.read(reinterpret_cast<char*>(&value), sizeof(int));
            addToTail(value);
        }
        
        file.close();
        cout << "Односвязный список загружен из бинарного файла: " << filename << endl;
    } else {
        cout << "Ошибка открытия бинарного файла для чтения: " << filename << endl;
    }
}
