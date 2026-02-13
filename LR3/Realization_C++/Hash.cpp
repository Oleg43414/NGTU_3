#include "Hash.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <cctype> 
#include <sstream>

using namespace std;

HashNode::HashNode(const std::string& k, const std::string& v) 
    : key(k), value(v), next(nullptr) {}

OpenHashEntry::OpenHashEntry() : used(false), deleted(false) {}

int stringHash(const std::string& key, int mod) {
    long long hashVal = 0;
    const int prime = 31;
    
    for (char ch : key) {
        hashVal = (hashVal * prime + abs(static_cast<int>(ch))) % mod;
    }
    return static_cast<int>(hashVal);
}

void parseCommand(const std::string& line, string& cmd, 
                  string& arg1, string& arg2) {
    cmd.clear(); 
    arg1.clear(); 
    arg2.clear();
    
    istringstream iss(line);
    
    iss >> cmd;
    if (cmd.empty()) return;
    
    iss >> arg1;
    if (arg1.empty()) return;

    string remaining;
    getline(iss, remaining);
    
    while (!remaining.empty() && (remaining[0] == ' ' || remaining[0] == '\t')) {
        remaining.erase(0, 1);
    }
    
    while (!remaining.empty() && (remaining.back() == ' ' || remaining.back() == '\t')) {
        remaining.pop_back();
    }
    
    arg2 = remaining;
}

void displayMenu() {
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
    cout << "0. Выход" << endl;
    cout << "Выберите действие: ";
}

ChainHashTable::ChainHashTable(int size) : capacity(size) {
    if (capacity < 1) capacity = 1;
    table = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
}

ChainHashTable::~ChainHashTable() {
    clear();
    delete[] table;
}

int ChainHashTable::hashFunction(const string& key) const {
    return stringHash(key, capacity);
}

void ChainHashTable::insert(const string& key, const string& value) {
    int index = hashFunction(key);

    HashNode* current = table[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value; // Обновляем значение
            cout << "Ключ '" << key << "' обновлен" << endl;
            return;
        }
        current = current->next;
    }
    
    // Вставляем новый узел в начало цепочки
    HashNode* newNode = new HashNode(key, value);
    newNode->next = table[index];
    table[index] = newNode;
    
    cout << "Добавлено: " << key << " -> " << value 
         << " (цепочка " << index << ")" << endl;
}

bool ChainHashTable::remove(const string& key) {
    int index = hashFunction(key);
    HashNode* current = table[index];
    HashNode* previous = nullptr;
    
    while (current != nullptr) {
        if (current->key == key) {
            if (previous == nullptr) {
                // Удаляем первый узел в цепочке
                table[index] = current->next;
            } else {
                previous->next = current->next;
            }
            
            delete current;
            cout << "Ключ '" << key << "' удален" << endl;
            return true;
        }
        
        previous = current;
        current = current->next;
    }
    
    cout << "Ключ '" << key << "' не найден" << endl;
    return false;
}

string ChainHashTable::find(const string& key) const {
    int index = hashFunction(key);
    HashNode* current = table[index];
    
    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    
    return ""; // Не найдено
}

void ChainHashTable::display() const {
    cout << "\n=== ЦЕПОЧЕЧНАЯ ХЕШ-ТАБЛИЦА (" << capacity << " ячеек) ===" << endl;
    
    bool isEmpty = true;
    for (int i = 0; i < capacity; i++) {
        if (table[i] != nullptr) {
            isEmpty = false;
            cout << "[" << i << "]: ";
            
            HashNode* current = table[i];
            while (current != nullptr) {
                cout << current->key << "=" << current->value;
                if (current->next != nullptr) {
                    cout << " -> ";
                }
                current = current->next;
            }
            cout << endl;
        }
    }
    
    if (isEmpty) {
        cout << "Таблица пуста" << endl;
    }
}

void ChainHashTable::clear() {
    for (int i = 0; i < capacity; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            delete current;
            current = next;
        }
        table[i] = nullptr;
    }
    cout << "Цепочечная таблица очищена" << endl;
}

void ChainHashTable::resize(int newSize) {
    if (newSize < 1) {
        cout << "Некорректный размер" << endl;
        return;
    }
    
    // Сохраняем старую таблицу
    HashNode** oldTable = table;
    int oldCapacity = capacity;
    
    // Создаем новую таблицу
    capacity = newSize;
    table = new HashNode*[capacity];
    for (int i = 0; i < capacity; i++) {
        table[i] = nullptr;
    }
    
    // Перехешируем все элементы
    for (int i = 0; i < oldCapacity; i++) {
        HashNode* current = oldTable[i];
        while (current != nullptr) {
            HashNode* next = current->next;
            
            // Перехешируем ключ
            int newIndex = hashFunction(current->key);
            
            // Вставляем в новую таблицу
            current->next = table[newIndex];
            table[newIndex] = current;
            
            current = next;
        }
    }
    
    delete[] oldTable;
    cout << "Таблица изменена до " << capacity << " ячеек" << endl;
}

int ChainHashTable::getSize() const {
    int count = 0;
    for (int i = 0; i < capacity; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            count++;
            current = current->next;
        }
    }
    return count;
}

int ChainHashTable::getCapacity() const {
    return capacity;
}

void ChainHashTable::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    file << "CHAIN_HASH_TABLE" << endl;
    file << capacity << endl;
    
    for (int i = 0; i < capacity; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            file << current->key << " " << current->value << endl;
            current = current->next;
        }
    }
    
    file.close();
    cout << "Цепочечная таблица сохранена в " << filename << endl;
}

void ChainHashTable::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    string type;
    file >> type;
    
    if (type != "CHAIN_HASH_TABLE") {
        cout << "Неверный формат файла" << endl;
        return;
    }
    
    int fileCapacity;
    file >> fileCapacity;
    
    clear();
    if (fileCapacity != capacity) {
        delete[] table;
        capacity = fileCapacity;
        table = new HashNode*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }
    
    string key, value;
    while (file >> key >> value) {
        insert(key, value);
    }
    
    file.close();
    cout << "Цепочечная таблица загружена из " << filename << endl;
}

// Исправленная версия loadFromBinaryFile (без vector)
void ChainHashTable::loadFromBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Ошибка открытия бинарного файла для чтения" << endl;
        return;
    }
    
    // Читаем сигнатуру
    char signature[16];
    file.read(signature, 16);
    if (string(signature) != "CHAIN_HASH_BIN") {
        cout << "Неверный формат файла" << endl;
        file.close();
        return;
    }
    
    int fileCapacity;
    file.read(reinterpret_cast<char*>(&fileCapacity), sizeof(int));
    
    clear();
    if (fileCapacity != capacity) {
        delete[] table;
        capacity = fileCapacity;
        table = new HashNode*[capacity];
        for (int i = 0; i < capacity; i++) {
            table[i] = nullptr;
        }
    }
    
    int numElements;
    file.read(reinterpret_cast<char*>(&numElements), sizeof(int));
    
    for (int i = 0; i < numElements; i++) {
        int keyLen, valueLen;
        
        // Читаем длину ключа
        file.read(reinterpret_cast<char*>(&keyLen), sizeof(int));
        // Выделяем память для ключа
        char* keyBuf = new char[keyLen + 1];
        file.read(keyBuf, keyLen);
        keyBuf[keyLen] = '\0';
        string key(keyBuf);
        delete[] keyBuf;
        
        // Читаем длину значения
        file.read(reinterpret_cast<char*>(&valueLen), sizeof(int));
        // Выделяем память для значения
        char* valueBuf = new char[valueLen + 1];
        file.read(valueBuf, valueLen);
        valueBuf[valueLen] = '\0';
        string value(valueBuf);
        delete[] valueBuf;
        
        insert(key, value);
    }
    
    file.close();
    cout << "Цепочечная таблица загружена из " << filename << endl;
}

void ChainHashTable::saveToBinaryFile(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Ошибка открытия бинарного файла для записи" << endl;
        return;
    }
    
    // Записываем сигнатуру (14 символов + 2 нулевых для выравнивания до 16)
    const char* signature = "CHAIN_HASH_BIN";
    file.write(signature, 16);
    
    file.write(reinterpret_cast<const char*>(&capacity), sizeof(int));
    
    // Считаем общее количество элементов
    int numElements = getSize();
    file.write(reinterpret_cast<const char*>(&numElements), sizeof(int));
    
    // Записываем все элементы
    for (int i = 0; i < capacity; i++) {
        HashNode* current = table[i];
        while (current != nullptr) {
            int keyLen = current->key.length();
            int valueLen = current->value.length();
            
            file.write(reinterpret_cast<const char*>(&keyLen), sizeof(int));
            file.write(current->key.c_str(), keyLen);
            
            file.write(reinterpret_cast<const char*>(&valueLen), sizeof(int));
            file.write(current->value.c_str(), valueLen);
            
            current = current->next;
        }
    }
    
    file.close();
    cout << "Цепочечная таблица сохранена в бинарный файл: " << filename << endl;
}

OpenHashTable::OpenHashTable(int size) : capacity(size) {
    if (capacity < 1) capacity = 1;
    table = new OpenHashEntry[capacity];
}

OpenHashTable::~OpenHashTable() {
    delete[] table;
}

int OpenHashTable::hashFunction(const string& key) const {
    return stringHash(key, capacity);
}

int OpenHashTable::probeFunction(int index, int attempt) const {
    // Линейное пробирование
    return (index + attempt) % capacity;
}

void OpenHashTable::insert(const string& key, const string& value) {
    int index = hashFunction(key);
    
    for (int attempt = 0; attempt < capacity; attempt++) {
        int currentIndex = probeFunction(index, attempt);
        
        if (!table[currentIndex].used || 
            (table[currentIndex].deleted && !table[currentIndex].used)) {
            table[currentIndex].key = key;
            table[currentIndex].value = value;
            table[currentIndex].used = true;
            table[currentIndex].deleted = false;
            
            cout << "Добавлено: " << key << " -> " << value 
                 << " (ячейка " << currentIndex << ")" << endl;
            return;
        }
        
        if (table[currentIndex].used && table[currentIndex].key == key) {
            table[currentIndex].value = value;
            cout << "Ключ '" << key << "' обновлен" << endl;
            return;
        }
    }
    
    cout << "Таблица переполнена, невозможно добавить '" << key << "'" << endl;
}

bool OpenHashTable::remove(const string& key) {
    int index = hashFunction(key);
    
    for (int attempt = 0; attempt < capacity; attempt++) {
        int currentIndex = probeFunction(index, attempt);
        
        if (table[currentIndex].used && table[currentIndex].key == key) {
            table[currentIndex].used = false;
            table[currentIndex].deleted = true;
            cout << "Ключ '" << key << "' удален" << endl;
            return true;
        }
        
        if (!table[currentIndex].used && !table[currentIndex].deleted) {
            break;
        }
    }
    
    cout << "Ключ '" << key << "' не найден" << endl;
    return false;
}

string OpenHashTable::find(const string& key) const {
    int index = hashFunction(key);
    
    for (int attempt = 0; attempt < capacity; attempt++) {
        int currentIndex = probeFunction(index, attempt);
        
        if (table[currentIndex].used && table[currentIndex].key == key) {
            return table[currentIndex].value;
        }
        
        if (!table[currentIndex].used && !table[currentIndex].deleted) {
            break;
        }
    }
    
    return "";
}

void OpenHashTable::display() const {
    cout << "\n=== ХЕШ-ТАБЛИЦА С ОТКРЫТОЙ АДРЕСАЦИЕЙ (" << capacity << " ячеек) ===" << endl;
    
    bool isEmpty = true;
    for (int i = 0; i < capacity; i++) {
        if (table[i].used) {
            isEmpty = false;
            cout << "[" << i << "]: " << table[i].key << "=" << table[i].value;
            if (table[i].deleted) {
                cout << " [УДАЛЕНО]";
            }
            cout << endl;
        } else if (table[i].deleted) {
            isEmpty = false;
            cout << "[" << i << "]: [УДАЛЕНО]" << endl;
        }
    }
    
    if (isEmpty) {
        cout << "Таблица пуста" << endl;
    }
}

void OpenHashTable::clear() {
    for (int i = 0; i < capacity; i++) {
        table[i].used = false;
        table[i].deleted = false;
    }
    cout << "Таблица с открытой адресацией очищена" << endl;
}

void OpenHashTable::resize(int newSize) {
    if (newSize < 1) {
        cout << "Некорректный размер" << endl;
        return;
    }
    
    // Сохраняем старую таблицу
    OpenHashEntry* oldTable = table;
    int oldCapacity = capacity;
    
    // Создаем новую таблицу
    capacity = newSize;
    table = new OpenHashEntry[capacity];
    
    // Перехешируем все элементы
    for (int i = 0; i < oldCapacity; i++) {
        if (oldTable[i].used && !oldTable[i].deleted) {
            const string& key = oldTable[i].key;
            const string& value = oldTable[i].value;
            int index = hashFunction(key);
            
            // Находим свободную ячейку
            for (int attempt = 0; attempt < capacity; attempt++) {
                int currentIndex = probeFunction(index, attempt);
                
                if (!table[currentIndex].used || 
                    (table[currentIndex].deleted && !table[currentIndex].used)) {
                    table[currentIndex].key = key;
                    table[currentIndex].value = value;
                    table[currentIndex].used = true;
                    table[currentIndex].deleted = false;
                    break;
                }
            }
        }
    }
    
    delete[] oldTable;
    cout << "Таблица изменена до " << capacity << " ячеек" << endl;
}

int OpenHashTable::getSize() const {
    int count = 0;
    for (int i = 0; i < capacity; i++) {
        if (table[i].used) {
            count++;
        }
    }
    return count;
}

int OpenHashTable::getCapacity() const {
    return capacity;
}

double OpenHashTable::getLoadFactor() const {
    return static_cast<double>(getSize()) / capacity;
}

void OpenHashTable::saveToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для записи" << endl;
        return;
    }
    
    file << "OPEN_HASH_TABLE" << endl;
    file << capacity << endl;
    
    for (int i = 0; i < capacity; i++) {
        if (table[i].used) {
            file << table[i].key << " " << table[i].value << endl;
        }
    }
    
    file.close();
    cout << "Таблица с открытой адресацией сохранена в " << filename << endl;
}

void OpenHashTable::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла для чтения" << endl;
        return;
    }
    
    string type;
    file >> type;
    
    if (type != "OPEN_HASH_TABLE") {
        cout << "Неверный формат файла" << endl;
        return;
    }
    
    int fileCapacity;
    file >> fileCapacity;
    
    clear();
    if (fileCapacity != capacity) {
        delete[] table;
        capacity = fileCapacity;
        table = new OpenHashEntry[capacity];
    }
    
    string key, value;
    while (file >> key >> value) {
        insert(key, value);
    }
    
    file.close();
    cout << "Таблица с открытой адресацией загружена из " << filename << endl;
}

void OpenHashTable::loadFromBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Ошибка открытия бинарного файла для чтения" << endl;
        return;
    }
    
    char signature[16];
    file.read(signature, 16);
    if (string(signature) != "OPEN_HASH_BIN") {
        cout << "Неверный формат файла" << endl;
        file.close();
        return;
    }
    
    int fileCapacity;
    file.read(reinterpret_cast<char*>(&fileCapacity), sizeof(int));
    
    clear();
    if (fileCapacity != capacity) {
        delete[] table;
        capacity = fileCapacity;
        table = new OpenHashEntry[capacity];
    }
    
    int numElements;
    file.read(reinterpret_cast<char*>(&numElements), sizeof(int));
    
    for (int i = 0; i < numElements; i++) {
        int keyLen, valueLen;
        
        file.read(reinterpret_cast<char*>(&keyLen), sizeof(int));
        char* keyBuf = new char[keyLen + 1];
        file.read(keyBuf, keyLen);
        keyBuf[keyLen] = '\0';
        string key(keyBuf);
        delete[] keyBuf;

        file.read(reinterpret_cast<char*>(&valueLen), sizeof(int));
        char* valueBuf = new char[valueLen + 1];
        file.read(valueBuf, valueLen);
        valueBuf[valueLen] = '\0';
        string value(valueBuf);
        delete[] valueBuf;
        
        // Вставляем напрямую, чтобы избежать повторного хеширования
        // Ищем свободную ячейку
        bool inserted = false;
        for (int attempt = 0; attempt < capacity; attempt++) {
            int index = probeFunction(hashFunction(key), attempt);
            if (!table[index].used || table[index].deleted) {
                table[index].key = key;
                table[index].value = value;
                table[index].used = true;
                table[index].deleted = false;
                inserted = true;
                break;
            }
        }
        
        if (!inserted) {
            cout << "Предупреждение: не удалось вставить элемент " << key << " при загрузке" << endl;
        }
    }
    
    file.close();
    cout << "Таблица с открытой адресацией загружена из бинарного файла: " << filename << endl;
}

void OpenHashTable::saveToBinaryFile(const string& filename) const {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Ошибка открытия бинарного файла для записи" << endl;
        return;
    }
    
    const char* signature = "OPEN_HASH_BIN";
    file.write(signature, 16);
    
    // Записываем capacity
    file.write(reinterpret_cast<const char*>(&capacity), sizeof(int));
    
    // Считаем количество активных элементов
    int numElements = 0;
    for (int i = 0; i < capacity; i++) {
        if (table[i].used && !table[i].deleted) {
            numElements++;
        }
    }
    file.write(reinterpret_cast<const char*>(&numElements), sizeof(int));
    
    // Записываем все активные элементы
    for (int i = 0; i < capacity; i++) {
        if (table[i].used && !table[i].deleted) {
            int keyLen = table[i].key.length();
            int valueLen = table[i].value.length();
            
            file.write(reinterpret_cast<const char*>(&keyLen), sizeof(int));
            file.write(table[i].key.c_str(), keyLen);
            
            file.write(reinterpret_cast<const char*>(&valueLen), sizeof(int));
            file.write(table[i].value.c_str(), valueLen);
        }
    }
    
    file.close();
    cout << "Таблица с открытой адресацией сохранена в бинарный файл: " << filename << endl;
}