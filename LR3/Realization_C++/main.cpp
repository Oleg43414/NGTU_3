#include <iostream>
#include "FullBinaryTree.h"
#include "SinglyLinkedList.h"
#include "DoublyLinkedList.h"
#include "DynamicArray.h"
#include "Queue.h"
#include "Stack.h"
#include "Hash.h"

using namespace std;

void demonstrateFullBinaryTree() {
    cout << "\n--- ДЕМОНСТРАЦИЯ ПОЛНОГО БИНАРНОГО ДЕРЕВА ---" << endl;
    
    FullBinaryTree tree;
    
    tree.add(10);
    tree.add(5);
    tree.add(15);
    tree.add(3);
    tree.add(7);
    tree.add(12);
    tree.add(18);
    
    tree.printInOrder();
    tree.printPreOrder();
    tree.printPostOrder();
    tree.printLevelOrder();
    
    tree.search(7);
    tree.search(100);
    
    tree.isFull();
    
    cout << "Количество узлов: " << tree.getNodeCount() << endl;
    cout << "Высота дерева: " << tree.getTreeHeight() << endl;
    
    tree.saveToFile("tree.txt");
    
    FullBinaryTree tree2;
    tree2.loadFromFile("tree.txt");
    tree2.printLevelOrder();
}

void demonstrateSinglyLinkedList() {
    cout << "\n--- ДЕМОНСТРАЦИЯ ОДНОСВЯЗНОГО СПИСКА ---" << endl;
    
    SinglyLinkedList list;
    
    list.addToHead(30);
    list.addToHead(20);
    list.addToHead(10);
    list.addToTail(40);
    list.addToTail(50);
    
    list.print();
    list.printReverse();
    
    list.addAfter(30, 35);
    list.addBefore(40, 38);
    list.print();
    
    list.deleteFromHead();
    list.deleteFromTail();
    list.deleteByValue(35);
    list.print();
    
    cout << "Поиск 38: " << (list.findByValue(38) ? "найден" : "не найден") << endl;
    cout << "Поиск 100: " << (list.findByValue(100) ? "найден" : "не найден") << endl;
    
    list.saveToFile("singly_list.txt");
    
    SinglyLinkedList list2;
    list2.loadFromFile("singly_list.txt");
    list2.print();
}

void demonstrateDoublyLinkedList() {
    cout << "\n--- ДЕМОНСТРАЦИЯ ДВУСВЯЗНОГО СПИСКА ---" << endl;
    
    DoublyLinkedList dlist;
    
    dlist.addToHead(30);
    dlist.addToHead(20);
    dlist.addToHead(10);
    dlist.addToTail(40);
    dlist.addToTail(50);
    
    dlist.printForward();
    dlist.printBackward();
    
    dlist.addAfterValue(30, 35);
    dlist.addBeforeValue(40, 38);
    dlist.printForward();
    
    dlist.deleteFromHead();
    dlist.deleteFromTail();
    dlist.deleteByValue(35);
    dlist.printForward();
    
    cout << "Поиск 38: " << (dlist.findByValue(38) ? "найден" : "не найден") << endl;
    
    dlist.saveToFile("doubly_list.txt");
    
    DoublyLinkedList dlist2;
    dlist2.loadFromFile("doubly_list.txt");
    dlist2.printForward();
}

void demonstrateDynamicArray() {
    cout << "\n--- ДЕМОНСТРАЦИЯ ДИНАМИЧЕСКОГО МАССИВА ---" << endl;
    
    DynamicArray arr(5);
    
    arr.pushBack("Hello");
    arr.pushBack("World");
    arr.pushBack("C++");
    arr.pushBack("Programming");
    arr.pushBack("Data Structures");
    
    arr.print();
    
    arr.insert(2, "Awesome");
    arr.print();
    
    arr.set(3, "Changed");
    arr.print();
    
    arr.remove(1);
    arr.print();
    
    cout << "Элемент с индексом 2: " << arr.get(2) << endl;
    
    cout << "Размер: " << arr.getSize() << endl;
    cout << "Емкость: " << arr.getCapacity() << endl;
    
    arr.saveToFile("array.txt");
    
    DynamicArray arr2;
    arr2.loadFromFile("array.txt");
    arr2.print();
}

void demonstrateQueue() {
    cout << "\n--- ДЕМОНСТРАЦИЯ ОЧЕРЕДИ ---" << endl;
    
    Queue queue;
    
    queue.push(10);
    queue.push(20);
    queue.push(30);
    queue.push(40);
    queue.push(50);
    
    queue.display();
    
    queue.pop();
    queue.pop();
    queue.display();
    
    cout << "Первый элемент: " << queue.front() << endl;
    
    cout << "Размер очереди: " << queue.size() << endl;
    cout << "Очередь пуста: " << (queue.isEmpty() ? "да" : "нет") << endl;
    
    queue.saveToFile("queue.txt");
    
    Queue queue2;
    queue2.loadFromFile("queue.txt");
    queue2.display();
}

void demonstrateStack() {
    cout << "\n--- ДЕМОНСТРАЦИЯ СТЕКА ---" << endl;
    
    Stack stack;
    
    stack.push(10);
    stack.push(20);
    stack.push(30);
    stack.push(40);
    stack.push(50);
    
    stack.print();
    
    stack.pop();
    stack.pop();
    stack.print();
    
    cout << "Верхний элемент: " << stack.peek() << endl;
    
    cout << "Размер стека: " << stack.size() << endl;
    cout << "Стек пуст: " << (stack.isEmpty() ? "да" : "нет") << endl;
    
    stack.saveToFile("stack.txt");
    
    Stack stack2;
    stack2.loadFromFile("stack.txt");
    stack2.print();
}

void demonstrateHashTable() {
    cout << "\n--- ДЕМОНСТРАЦИЯ ХЕШ-ТАБЛИЦ ---" << endl;
    
    ChainHashTable chainTable(5);
    OpenHashTable openTable(5);
    
    cout << "\n1. Добавление элементов:" << endl;
    chainTable.insert("name", "Ivan");
    openTable.insert("name", "Ivan");
    
    chainTable.insert("age", "25");
    openTable.insert("age", "25");
    
    chainTable.insert("city", "Moscow");
    openTable.insert("city", "Moscow");
    
    chainTable.insert("job", "Developer");
    openTable.insert("job", "Developer");
    
    chainTable.insert("country", "Russia");
    openTable.insert("country", "Russia");
    
    cout << "\n2. Содержимое таблиц:" << endl;
    chainTable.display();
    openTable.display();
    
    cout << "\n3. Поиск элементов:" << endl;
    cout << "Поиск 'name': " << chainTable.find("name") << endl;
    cout << "Поиск 'age': " << openTable.find("age") << endl;
    cout << "Поиск несуществующего 'salary': " << chainTable.find("salary") << endl;
    
    cout << "\n4. Демонстрация коллизий:" << endl;
    chainTable.insert("abc", "value1");
    chainTable.insert("cba", "value2");
    
    openTable.insert("abc", "value1");
    openTable.insert("cba", "value2");
    
    chainTable.display();
    openTable.display();
    
    cout << "\n5. Удаление элементов:" << endl;
    chainTable.remove("city");
    openTable.remove("city");
    
    cout << "\n6. После удаления 'city':" << endl;
    chainTable.display();
    openTable.display();
    
    cout << "\n7. Изменение размера таблиц:" << endl;
    chainTable.resize(10);
    openTable.resize(10);
    
    cout << "После изменения размера:" << endl;
    chainTable.display();
    openTable.display();
    
    cout << "\n8. Статистика:" << endl;
    cout << "Цепочечная таблица:" << endl;
    cout << "  Емкость: " << chainTable.getCapacity() << endl;
    cout << "  Элементов: " << chainTable.getSize() << endl;
    
    cout << "Таблица с открытой адресацией:" << endl;
    cout << "  Емкость: " << openTable.getCapacity() << endl;
    cout << "  Элементов: " << openTable.getSize() << endl;
    cout << "  Коэффициент заполнения: " << openTable.getLoadFactor() * 100 << "%" << endl;
    
    cout << "\n9. Сохранение и загрузка:" << endl;
    chainTable.saveToFile("chain_table.txt");
    openTable.saveToFile("open_table.txt");
    
    ChainHashTable chainTable2(5);
    OpenHashTable openTable2(5);
    
    chainTable2.loadFromFile("chain_table.txt");
    openTable2.loadFromFile("open_table.txt");
    
    cout << "Загруженные таблицы:" << endl;
    chainTable2.display();
    openTable2.display();
}

void demonstrateHashTableInteractive() {
    cout << "\n--- ИНТЕРАКТИВНАЯ РАБОТА С ХЕШ-ТАБЛИЦАМИ ---" << endl;
    
    ChainHashTable chainTable(10);
    OpenHashTable openTable(10);
    
    int choice;
    string key, value;
    
    do {
        cout << "\n=== МЕНЮ ХЕШ-ТАБЛИЦ ===" << endl;
        cout << "1. Добавить элемент" << endl;
        cout << "2. Найти элемент" << endl;
        cout << "3. Удалить элемент" << endl;
        cout << "4. Показать таблицы" << endl;
        cout << "5. Очистить таблицы" << endl;
        cout << "6. Изменить размер таблиц" << endl;
        cout << "7. Сохранить в файл" << endl;
        cout << "8. Загрузить из файла" << endl;
        cout << "9. Статистика" << endl;
        cout << "0. Вернуться в главное меню" << endl;
        cout << "Выберите действие: ";
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: {
                cout << "Введите ключ: ";
                getline(cin, key);
                cout << "Введите значение: ";
                getline(cin, value);
                
                chainTable.insert(key, value);
                openTable.insert(key, value);
                break;
            }
            
            case 2: {
                cout << "Введите ключ для поиска: ";
                getline(cin, key);
                
                string chainResult = chainTable.find(key);
                string openResult = openTable.find(key);
                
                if (!chainResult.empty()) {
                    cout << "Цепочечная таблица: " << key << " -> " << chainResult << endl;
                } else {
                    cout << "Цепочечная таблица: ключ '" << key << "' не найден" << endl;
                }
                
                if (!openResult.empty()) {
                    cout << "Открытая адресация: " << key << " -> " << openResult << endl;
                } else {
                    cout << "Открытая адресация: ключ '" << key << "' не найден" << endl;
                }
                break;
            }
            
            case 3: {
                cout << "Введите ключ для удаления: ";
                getline(cin, key);
                
                chainTable.remove(key);
                openTable.remove(key);
                break;
            }
            
            case 4: {
                chainTable.display();
                openTable.display();
                break;
            }
            
            case 5: {
                chainTable.clear();
                openTable.clear();
                cout << "Обе таблицы очищены" << endl;
                break;
            }
            
            case 6: {
                int newSize;
                cout << "Введите новый размер таблиц: ";
                cin >> newSize;
                
                if (newSize > 0) {
                    chainTable.resize(newSize);
                    openTable.resize(newSize);
                } else {
                    cout << "Размер должен быть положительным числом" << endl;
                }
                break;
            }
            
            case 7: {
                chainTable.saveToFile("chain_hash_interactive.txt");
                openTable.saveToFile("open_hash_interactive.txt");
                break;
            }
            
            case 8: {
                chainTable.loadFromFile("chain_hash_interactive.txt");
                openTable.loadFromFile("open_hash_interactive.txt");
                break;
            }
            
            case 9: {
                cout << "\n--- СТАТИСТИКА ---" << endl;
                cout << "Цепочечная таблица:" << endl;
                cout << "  Емкость таблицы: " << chainTable.getCapacity() << endl;
                cout << "  Количество элементов: " << chainTable.getSize() << endl;
                cout << "  Заполнение: " << chainTable.getSize() << "/" 
                     << chainTable.getCapacity() << " элементов" << endl;
                
                cout << "\nОткрытая адресация:" << endl;
                cout << "  Емкость таблицы: " << openTable.getCapacity() << endl;
                cout << "  Количество элементов: " << openTable.getSize() << endl;
                cout << "  Коэффициент заполнения: " << openTable.getLoadFactor() * 100 << "%" << endl;
                break;
            }
            
            case 0: {
                cout << "Возврат в главное меню" << endl;
                break;
            }
            
            default: {
                cout << "Неверный выбор. Попробуйте снова." << endl;
            }
        }
        
    } while (choice != 0);
}

void demonstrateBinarySerialization() {
    cout << "\n--- ДЕМОНСТРАЦИЯ БИНАРНОЙ СЕРИАЛИЗАЦИИ ---" << endl;
    
    DoublyLinkedList binaryList;
    binaryList.addToHead(100);
    binaryList.addToTail(200);
    binaryList.addToTail(300);
    
    cout << "\nИсходный список:" << endl;
    binaryList.printForward();
    
    binaryList.saveToBinaryFile("binary_list.bin");
    
    DoublyLinkedList loadedList;
    loadedList.loadFromBinaryFile("binary_list.bin");
    
    cout << "\nЗагруженный список:" << endl;
    loadedList.printForward();
    
    cout << "\n=== ДИНАМИЧЕСКИЙ МАССИВ ===" << endl;
    DynamicArray binaryArray;
    binaryArray.pushBack("Hello");
    binaryArray.pushBack("World");
    binaryArray.pushBack("Binary");
    
    cout << "\nИсходный массив:" << endl;
    binaryArray.print();
    
    binaryArray.saveToBinaryFile("binary_array.bin");
    
    DynamicArray loadedArray;
    loadedArray.loadFromBinaryFile("binary_array.bin");
    
    cout << "\nЗагруженный массив:" << endl;
    loadedArray.print();
    
    cout << "\n=== ОЧЕРЕДЬ ===" << endl;
    Queue binaryQueue;
    binaryQueue.push(555);
    binaryQueue.push(666);
    binaryQueue.push(777);
    
    cout << "\nИсходная очередь:" << endl;
    binaryQueue.display();
    
    binaryQueue.saveToBinaryFile("binary_queue.bin");
    
    Queue loadedQueue;
    loadedQueue.loadFromBinaryFile("binary_queue.bin");
    
    cout << "\nЗагруженная очередь:" << endl;
    loadedQueue.display();
    
    cout << "\n--- СТЕК ---" << endl;
    Stack binaryStack;
    binaryStack.push(111);
    binaryStack.push(222);
    binaryStack.push(333);
    
    cout << "\nИсходный стек:" << endl;
    binaryStack.print();
    
    binaryStack.saveToBinaryFile("binary_stack.bin");
    
    Stack loadedStack;
    loadedStack.loadFromBinaryFile("binary_stack.bin");
    
    cout << "\nЗагруженный стек:" << endl;
    loadedStack.print();
    
    cout << "\nБинарная сериализация успешно продемонстрирована!" << endl;
    cout << "Созданы файлы: binary_list.bin, binary_array.bin, binary_queue.bin, binary_stack.bin" << endl;
}

void demonstrateAllSerialization() {
    cout << "\n--- ДЕМОНСТРАЦИЯ ВСЕХ ВИДОВ СЕРИАЛИЗАЦИИ ---" << endl;
    
    cout << "\n1. ОДНОСВЯЗНЫЙ СПИСОК:" << endl;
    SinglyLinkedList list;
    list.addToHead(50);
    list.addToTail(60);
    list.addToTail(70);
    
    cout << "Исходный список: ";
    list.print();
    
    list.saveToFile("test_singly.txt");
    list.saveToBinaryFile("test_singly.bin");
    
    SinglyLinkedList listFromText, listFromBinary;
    listFromText.loadFromFile("test_singly.txt");
    listFromBinary.loadFromBinaryFile("test_singly.bin");
    
    cout << "Из текстового файла: ";
    listFromText.print();
    cout << "Из бинарного файла: ";
    listFromBinary.print();
    
    cout << "\n2. ХЕШ-ТАБЛИЦЫ:" << endl;
    ChainHashTable hashTable(3);
    hashTable.insert("test", "value1");
    hashTable.insert("key", "value2");
    
    cout << "Исходная таблица:" << endl;
    hashTable.display();
    
    hashTable.saveToFile("test_hash.txt");
    hashTable.saveToBinaryFile("test_hash.bin");
    
    ChainHashTable hashFromText(3), hashFromBinary(3);
    hashFromText.loadFromFile("test_hash.txt");
    hashFromBinary.loadFromBinaryFile("test_hash.bin");
    
    cout << "Из текстового файла:" << endl;
    hashFromText.display();
    cout << "Из бинарного файла:" << endl;
    hashFromBinary.display();
    
    cout << "\nВсе виды сериализации успешно протестированы!" << endl;
}

int main() {
    cout << "ДЕМОНСТРАЦИЯ РЕАЛИЗАЦИИ СТРУКТУР ДАННЫХ НА C++" << endl;
    
    int choice;
    
    do {
        cout << "\n--- ГЛАВНОЕ МЕНЮ ---" << endl;
        cout << "1. Полное бинарное дерево" << endl;
        cout << "2. Односвязный список" << endl;
        cout << "3. Двусвязный список" << endl;
        cout << "4. Динамический массив" << endl;
        cout << "5. Очередь" << endl;
        cout << "6. Стек" << endl;
        cout << "7. Хеш-таблицы (демонстрация)" << endl;
        cout << "8. Хеш-таблицы (интерактивный режим)" << endl;
        cout << "9. Бинарная сериализация" << endl;
        cout << "10. Все виды сериализации" << endl;
        cout << "11. Все структуры данных" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите вариант: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                demonstrateFullBinaryTree();
                break;
            case 2:
                demonstrateSinglyLinkedList();
                break;
            case 3:
                demonstrateDoublyLinkedList();
                break;
            case 4:
                demonstrateDynamicArray();
                break;
            case 5:
                demonstrateQueue();
                break;
            case 6:
                demonstrateStack();
                break;
            case 7:
                demonstrateHashTable();
                break;
            case 8:
                demonstrateHashTableInteractive();
                break;
            case 9:
                demonstrateBinarySerialization();
                break;
            case 10:
                demonstrateAllSerialization();
                break;
            case 11:
                demonstrateFullBinaryTree();
                demonstrateSinglyLinkedList();
                demonstrateDoublyLinkedList();
                demonstrateDynamicArray();
                demonstrateQueue();
                demonstrateStack();
                demonstrateHashTable();
                break;
            case 0:
                cout << "Выход из программы." << endl;
                break;
            default:
                cout << "Неверный выбор!" << endl;
        }
    } while (choice != 0);
    
    return 0;
}