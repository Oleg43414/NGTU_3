#include "../../include/include/List_1.h"
#include <iostream>
#include <fstream>
#include <stack> // для печати в обратном порядке

using namespace std;

Node1* SinglyLinkedList::createNode(int value) {
    Node1* newNode = new Node1;
    newNode->key = value;
    newNode->next = nullptr;
    return newNode;
}

SinglyLinkedList::SinglyLinkedList() : head(nullptr) {}

SinglyLinkedList::~SinglyLinkedList() {
    clear();
}

void SinglyLinkedList::addToHead(int value) {
    Node1* newNode = createNode(value);
    newNode->next = head;
    head = newNode;
    cout << "Добавлен в начало: " << value << endl;
}

void SinglyLinkedList::addToTail(int value) {
    Node1* newNode = createNode(value);
    
    if(head == nullptr) {
        head = newNode;
        cout << "Добавлен в конец: " << value << endl;
        return;
    }

    Node1* current = head;
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

    Node1* current = head;
    while (current != nullptr && current->key != targetValue) {
        current = current->next;
    }

    if (current == nullptr) {
        cout << targetValue << " не найден!" << endl;
        return;
    }

    Node1* newNode = createNode(newValue);
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

    Node1* current = head;
    while (current->next != nullptr && current->next->key != targetValue) {
        current = current->next;
    }

    if (current->next == nullptr) {
        cout << targetValue << " не найден!" << endl;
        return;
    }

    Node1* newNode = createNode(newValue);
    newNode->next = current->next;
    current->next = newNode;
    cout << "Добавлен перед " << targetValue << ": " << newValue << endl;
}

//  Удаление после элемента
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
        deleteFromHead();
        return;
    }

    Node1* current = head;
    while (current->next->next != nullptr && current->next->next->key != targetValue) {
        current = current->next;
    }

    if (current->next->next == nullptr) {
        cout << "Элемент " << targetValue << " не найден!" << endl;
        return;
    }

    Node1* nodeToDelete = current->next;
    current->next = current->next->next;
    cout << "Удален элемент: " << nodeToDelete->key << " перед элементом " << targetValue << endl;
    delete nodeToDelete;
}

void SinglyLinkedList::deleteAfter(int targetValue) {
    if (head == nullptr) {
        cout << "Список пуст! Невозможно удалить элемент." << endl;
        return;
    }
    
    Node1* current = head;
    
    // Поиск узла с целевым значением
    while (current != nullptr && current->key != targetValue) {
        current = current->next;
    }
    
    if (current == nullptr) {
        cout << "Элемент " << targetValue << " не найден в списке!" << endl;
        return;
    }
    
    // Проверка, есть ли элемент после найденного
    if (current->next == nullptr) {
        cout << "После элемента " << targetValue << " нет элементов для удаления!" << endl;
        return;
    }
    
    Node1* nodeToDelete = current->next;
    current->next = nodeToDelete->next;
    
    cout << "Удален элемент: " << nodeToDelete->key << " после элемента " << targetValue << endl;
    delete nodeToDelete;
}

void SinglyLinkedList::print() {
    if (head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    Node1* current = head;
    while (current != nullptr) {
        cout << current->key << " -> ";
        current = current->next;
    }
    cout << "nullptr" << endl;
}

// ДОБАВЛЕННЫЙ МЕТОД: Печать в обратном порядке
void SinglyLinkedList::printReverse() {
    if (head == nullptr) {
        cout << "Список пуст" << endl;
        return;
    }
    
    // Используем стек для хранения элементов
    stack<int> st;
    Node1* current = head;
    
    // Проходим по списку и добавляем элементы в стек
    while (current != nullptr) {
        st.push(current->key);
        current = current->next;
    }
    
    // Выводим элементы из стека (в обратном порядке)
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
    Node1* temp = head;
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
    
    Node1* current = head;
    while (current->next->next != nullptr) {
        current = current->next;
    }
    
    cout << "Удален из конца: " << current->next->key << endl;
    delete current->next;
    current->next = nullptr;
}

void SinglyLinkedList::deleteByValue(int value) {
    if (head == nullptr) {
        cout << "Список пуст!" << endl;
        return;
    }
    
    if (head->key == value) {
        deleteFromHead();
        return;
    }
    
    Node1* current = head;
    while (current->next != nullptr && current->next->key != value) {
        current = current->next;
    }
    
    if (current->next == nullptr) {
        cout << "Элемент " << value << " не найден!" << endl;
        return;
    }
    
    Node1* temp = current->next;
    current->next = current->next->next;
    cout << "Удален элемент: " << temp->key << endl;
    delete temp;
}

bool SinglyLinkedList::findByValue(int value) {
    Node1* current = head;
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
        Node1* temp = head;
        head = head->next;
        delete temp;
    }
}

void SinglyLinkedList::saveToFile(const string& filename) {
    ofstream file(filename);
    if(file.is_open()) {
        Node1* current = head;
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