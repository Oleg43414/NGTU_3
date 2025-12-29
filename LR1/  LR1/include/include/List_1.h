// List_1.h
#ifndef LIST_1_H
#define LIST_1_H

#include <string>

// Структура узла односвязного списка
struct Node1 {
    int key;        // Значение узла
    Node1* next;    // Указатель на следующий узел
};

// Класс односвязного списка
class SinglyLinkedList {
private:
    Node1* head;    // Указатель на начало списка

    // Вспомогательные методы
    Node1* createNode(int value);  // Создание нового узла

public:
    // Конструктор и деструктор
    SinglyLinkedList();
    ~SinglyLinkedList();

    // Добавление элементов
    void addToHead(int value);                 // Добавление в начало
    void addToTail(int value);                 // Добавление в конец
    void addAfter(int targetValue, int newValue);  // Добавление после элемента
    void addBefore(int targetValue, int newValue); // Добавление перед элементом

    // Удаление элементов
    void deleteFromHead();                     // Удаление из начала
    void deleteFromTail();                     // Удаление из конца
    void deleteByValue(int value);             // Удаление по значению
    void deleteAfter(int targetValue);         // Удаление после элемента
    void deleteBefore(int targetValue);        // Удаление перед элементом

    // Поиск и вывод
    bool findByValue(int value);               // Поиск по значению
    void print();                              // Вывод списка
    void printReverse();                       // Вывод в обратном порядке

    // Вспомогательные методы
    void clear();                              // Очистка списка
    void saveToFile(const std::string& filename);  // Сохранение в файл
    void loadFromFile(const std::string& filename); // Загрузка из файла
};

#endif // LIST_1_H