#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../../include/include/Massiv.h"
#include "../../include/include/List_1.h"
#include "../../include/include/List_2.h"
#include "../../include/include/Steck.h"
#include "../../include/include/Queue.h"
#include "../../include/include/FullBinaryTree.h"

using namespace std;

void executeCommand(const string &query, const string &filename)
{
    istringstream iss(query);
    string command;
    iss >> command;

    if (command == "MPUSH")
    {
        int value;
        iss >> value;
        Arry arr;
        arr.loadFromFile(filename);
        arr.pushBack(value);
        arr.saveToFile(filename);
        cout << "-> добавлен " << value << endl;
    }
    else if (command == "MINSERT")
    {
        int index, value;
        iss >> index >> value;
        Arry arr;
        arr.loadFromFile(filename);
        arr.insert(index, value);
        arr.saveToFile(filename);
        cout << "-> вставлен " << value << " по индексу " << index << endl;
    }
    else if (command == "MGET")
    {
        int index;
        iss >> index;
        Arry arr;
        arr.loadFromFile(filename);
        int val = arr.get(index);
        if (val != -1)
            cout << "-> " << val << endl;
    }
    else if (command == "MREMOVE")
    {
        int index;
        iss >> index;
        Arry arr;
        arr.loadFromFile(filename);
        arr.remove(index);
        arr.saveToFile(filename);
        cout << "-> удалён элемент по индексу " << index << endl;
    }
    else if (command == "MSET")
    {
        int index, value;
        iss >> index >> value;
        Arry arr;
        arr.loadFromFile(filename);
        arr.set(index, value);
        arr.saveToFile(filename);
        cout << "-> заменён элемент по индексу " << index << " на " << value << endl;
    }
    else if (command == "MSIZE")
    {
        Arry arr;
        arr.loadFromFile(filename);
        cout << "-> " << arr.getSize() << endl;
    }
    else if (command == "MPRINT")
    {
        Arry arr;
        arr.loadFromFile(filename);
        cout << "-> ";
        arr.print();
    }
    else if (command == "MCLEAR")
    {
        Arry arr;
        arr.clear();
        arr.saveToFile(filename);
        cout << "-> массив очищен" << endl;
    }
    // КОМАНДЫ ДЛЯ ОДНОСВЯЗНОГО СПИСКА
    else if (command == "FADD_HEAD")
    {
        int value;
        iss >> value;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.addToHead(value);
        list.saveToFile(filename);
    }
    else if (command == "FADD_TAIL")
    {
        int value;
        iss >> value;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.addToTail(value);
        list.saveToFile(filename);
    }
    else if (command == "FADD_AFTER")
    {
        int target, value;
        iss >> target >> value;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.addAfter(target, value);
        list.saveToFile(filename);
    }
    else if (command == "FADD_BEFORE")
    {
        int target, value;
        iss >> target >> value;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.addBefore(target, value);
        list.saveToFile(filename);
    }
    else if (command == "FREMOVE_HEAD")
    {
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.deleteFromHead();
        list.saveToFile(filename);
    }
    else if (command == "FREMOVE_TAIL")
    {
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.deleteFromTail();
        list.saveToFile(filename);
    }
    else if (command == "FREMOVE_VALUE")
    {
        int value;
        iss >> value;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.deleteByValue(value);
        list.saveToFile(filename);
    }
    // НОВАЯ КОМАНДА ДЛЯ ОДНОСВЯЗНОГО СПИСКА - удаление после элемента
    else if (command == "FREMOVE_AFTER")
    {
        int target;
        iss >> target;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.deleteAfter(target);
        list.saveToFile(filename);
        cout << "-> удалён элемент после " << target << endl;
    }
    // НОВАЯ КОМАНДА ДЛЯ ОДНОСВЯЗНОГО СПИСКА - удаление перед элементом
    else if (command == "FREMOVE_BEFORE")
    {
        int target;
        iss >> target;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        list.deleteBefore(target);
        list.saveToFile(filename);
        cout << "-> удалён элемент перед " << target << endl;
    }
    else if (command == "FFIND")
    {
        int value;
        iss >> value;
        SinglyLinkedList list;
        list.loadFromFile(filename);
        bool found = list.findByValue(value);
        cout << "-> " << (found ? "найден " : "не найден ") << value << endl;
    }
    else if (command == "FPRINT")
    {
        SinglyLinkedList list;
        list.loadFromFile(filename);
        cout << "-> ";
        list.print();
    }
    // НОВАЯ КОМАНДА ДЛЯ ОДНОСВЯЗНОГО СПИСКА - печать в обратном порядке
    else if (command == "FPRINT_BACK")
    {
        SinglyLinkedList list;
        list.loadFromFile(filename);
        cout << "-> ";
        list.printReverse();
    }
    // КОМАНДЫ ДЛЯ ДВУСВЯЗНОГО СПИСКА
    else if (command == "LADD_HEAD")
    {
        int value;
        iss >> value;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.addToHead(value);
        list.saveToFile(filename);
    }
    else if (command == "LADD_TAIL")
    {
        int value;
        iss >> value;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.addToTail(value);
        list.saveToFile(filename);
    }
    else if (command == "LADD_AFTER")
    {
        int target, value;
        iss >> target >> value;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.addAfterValue(target, value);
        list.saveToFile(filename);
        cout << "-> добавлен " << value << " после " << target << endl;
    }
    else if (command == "LADD_BEFORE")
    {
        int target, value;
        iss >> target >> value;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.addBeforeValue(target, value);
        list.saveToFile(filename);
        cout << "-> добавлен " << value << " перед " << target << endl;
    }
    else if (command == "LREMOVE_HEAD")
    {
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.deleteFromHead();
        list.saveToFile(filename);
    }
    else if (command == "LREMOVE_TAIL")
    {
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.deleteFromTail();
        list.saveToFile(filename);
    }
    else if (command == "LREMOVE_VALUE")
    {
        int value;
        iss >> value;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.deleteByValue(value);
        list.saveToFile(filename);
    }
    else if (command == "LREMOVE_AFTER")
    {
        int target;
        iss >> target;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.deleteAfterValue(target);
        list.saveToFile(filename);
        cout << "-> удалён элемент после " << target << endl;
    }
    // НОВАЯ КОМАНДА ДЛЯ ДВУСВЯЗНОГО СПИСКА - удаление перед элементом
    else if (command == "LREMOVE_BEFORE")
    {
        int target;
        iss >> target;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        list.deleteBeforeValue(target);
        list.saveToFile(filename);
        cout << "-> удалён элемент перед " << target << endl;
    }
    else if (command == "LFIND")
    {
        int value;
        iss >> value;
        DoublyLinkedList list;
        list.loadFromFile(filename);
        bool found = list.findByValue(value);
        cout << "-> " << (found ? "найден " : "не найден ") << value << endl;
    }
    else if (command == "LPRINT")
    {
        DoublyLinkedList list;
        list.loadFromFile(filename);
        cout << "-> ";
        list.printForward();
    }
    else if (command == "LPRINT_BACK")
    {
        DoublyLinkedList list;
        list.loadFromFile(filename);
        cout << "-> ";
        list.printBackward();
    }
    // КОМАНДЫ ДЛЯ СТЕКА
    else if (command == "SPUSH")
    {
        int value;
        iss >> value;
        Stack stack;
        stack.loadFromFile(filename);
        stack.push(value);
        stack.saveToFile(filename);
    }
    else if (command == "SPOP")
    {
        Stack stack;
        stack.loadFromFile(filename);
        int val = stack.pop();
        if (val != -1)
        {
            stack.saveToFile(filename);
        }
    }
    else if (command == "SPEEK")
    {
        Stack stack;
        stack.loadFromFile(filename);
        int val = stack.peek();
        if (val != -1)
            cout << "-> " << val << endl;
    }
    else if (command == "SPRINT")
    {
        Stack stack;
        stack.loadFromFile(filename);
        cout << "-> ";
        stack.print();
    }
    else if (command == "SCLEAR")
    {
        Stack stack;
        stack.clear();
        stack.saveToFile(filename);
        cout << "-> стек очищен" << endl;
    }
    // КОМАНДЫ ДЛЯ ОЧЕРЕДИ
    else if (command == "QPUSH")
    {
        int value;
        iss >> value;
        Queue queue;
        queue.loadFromFile(filename);
        queue.push(value);
        queue.saveToFile(filename);
    }
    else if (command == "QPOP")
    {
        Queue queue;
        queue.loadFromFile(filename);
        queue.pop();
        queue.saveToFile(filename);
    }
    else if (command == "QFRONT")
    {
        Queue queue;
        queue.loadFromFile(filename);
        int val = queue.front();
        if (val != -1)
            cout << "-> " << val << endl;
    }
    else if (command == "QPRINT")
    {
        Queue queue;
        queue.loadFromFile(filename);
        cout << "-> ";
        queue.display();
    }
    else if (command == "QCLEAR")
    {
        Queue queue;
        queue.clear();
        queue.saveToFile(filename);
        cout << "-> очередь очищена" << endl;
    }
    // КОМАНДЫ ДЛЯ ДЕРЕВА
    else if (command == "TADD")
    {
        int value;
        iss >> value;
        FullBinaryTree tree;
        tree.loadFromFile(filename);
        tree.add(value);
        tree.saveToFile(filename);
    }
    else if (command == "TFIND")
    {
        int value;
        iss >> value;
        FullBinaryTree tree;
        tree.loadFromFile(filename);
        bool found = tree.search(value);
        cout << "-> " << (found ? "найден " : "не найден ") << value << endl;
    }
    else if (command == "TISFULL")
    {
        FullBinaryTree tree;
        tree.loadFromFile(filename);
        bool full = tree.isFull();
        cout << "-> " << (full ? "FULL" : "NOT FULL") << endl;
    }
    else if (command == "TINORDER")
    {
        FullBinaryTree tree;
        tree.loadFromFile(filename);
        tree.printInOrder();
    }
    else if (command == "TPREORDER")
    {
        FullBinaryTree tree;
        tree.loadFromFile(filename);
        tree.printPreOrder();
    }
    else if (command == "TPOSTORDER")
    {
        FullBinaryTree tree;
        tree.loadFromFile(filename);
        tree.printPostOrder();
    }
    else if (command == "TLEVELORDER")
    {
        FullBinaryTree tree;
        tree.loadFromFile(filename);
        tree.printLevelOrder();
    }
    else if (command == "TCLEAR")
    {
        FullBinaryTree tree;
        tree.clear();
        tree.saveToFile(filename);
        cout << "-> дерево очищено" << endl;
    }
    else
    {
        cerr << "Неизвестная команда: " << command << endl;
    }
}

int main()
{
    string query;
    string filename;

    cout << "Введите имя файла для работы: ";
    cin >> filename;
    cin.ignore();

    cout << "\n=== ДОСТУПНЫЕ КОМАНДЫ ===" << endl;
    cout << "МАССИВ: MPUSH(добавить), MINSERT(вставить), MGET(получить), MREMOVE(удалить), MSET(установить), MSIZE(размер), MPRINT(печать), MCLEAR(очистить)" << endl;
    cout << "ОДНОСВЯЗНЫЙ СПИСОК: FADD_HEAD(добавить в начало), FADD_TAIL(добавить в конец), FADD_AFTER(добавить после), FADD_BEFORE(добавить перед)," << endl; 
    cout << "                    FREMOVE_HEAD(удалить из начала), FREMOVE_TAIL(удалить из конца), FREMOVE_VALUE(удалить по значению), " << endl;
    cout << "                    FREMOVE_AFTER(удалить после), FREMOVE_BEFORE(удалить перед), FFIND(найти), FPRINT(печать), FPRINT_BACK(печать в обратном порядке)" << endl;
    cout << "ДВУСВЯЗНЫЙ СПИСОК:  LADD_HEAD(добавить в начало), LADD_TAIL(добавить в конец), LADD_AFTER(добавить после), LADD_BEFORE(добавить перед)," << endl;
    cout << "                    LREMOVE_HEAD(удалить из начала), LREMOVE_TAIL(удалить из конца), LREMOVE_VALUE(удалить по значению)," << endl;
    cout << "                    LREMOVE_AFTER(удалить после), LREMOVE_BEFORE(удалить перед), LFIND(найти), LPRINT(печать), LPRINT_BACK(печать в обратном порядке)" << endl;
    cout << "СТЕК: SPUSH(добавить), SPOP(извлечь), SPEEK(посмотреть), SPRINT(печать), SCLEAR(очистить)" << endl;
    cout << "ОЧЕРЕДЬ: QPUSH(добавить), QPOP(извлечь), QFRONT(первый элемент), QPRINT(печать), QCLEAR(очистить)" << endl;
    cout << "ДЕРЕВО: TADD(добавить), TFIND(найти), TISFULL(проверить полноту), TINORDER(обход in-order), TPREORDER(обход pre-order)," << endl;
    cout << "TPOSTORDER(обход post-order), TLEVELORDER(обход по уровням), TCLEAR(очистить)" << endl;


    while (true)
    {
        cout << "> ";
        getline(cin, query);

        if (query == "exit")
        {
            break;
        }

        if (!query.empty())
        {
            executeCommand(query, filename);
        }
    }

    return 0;
}