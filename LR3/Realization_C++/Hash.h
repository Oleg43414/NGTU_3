#ifndef HASH_H
#define HASH_H

#include <string>

// Структуры данных
struct HashNode {
    std::string key;
    std::string value;
    HashNode* next;
    
    HashNode(const std::string& k, const std::string& v);
};

struct OpenHashEntry {
    std::string key;
    std::string value;
    bool used;
    bool deleted;
    
    OpenHashEntry();
};

// Хеш-таблица с цепочками
class ChainHashTable {
private:
    HashNode** table;
    int capacity;
    
    int hashFunction(const std::string& key) const;
    
public:
    ChainHashTable(int size = 10);
    ~ChainHashTable();
    
    void insert(const std::string& key, const std::string& value);
    bool remove(const std::string& key);
    std::string find(const std::string& key) const;
    void display() const;
    void clear();
    void resize(int newSize);
    
    int getSize() const;
    int getCapacity() const;
    
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinaryFile(const std::string& filename) const;
    void loadFromBinaryFile(const std::string& filename);
};

// Хеш-таблица с открытой адресацией
class OpenHashTable {
private:
    OpenHashEntry* table;
    int capacity;
    
    int hashFunction(const std::string& key) const;
    int probeFunction(int index, int attempt) const;
    
public:
    OpenHashTable(int size = 10);
    ~OpenHashTable();
    
    void insert(const std::string& key, const std::string& value);
    bool remove(const std::string& key);
    std::string find(const std::string& key) const;
    void display() const;
    void clear();
    void resize(int newSize);
    
    int getSize() const;
    int getCapacity() const;
    double getLoadFactor() const;
    
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void saveToBinaryFile(const std::string& filename) const;
    void loadFromBinaryFile(const std::string& filename);
};

// Вспомогательные функции
int stringHash(const std::string& key, int mod);
void parseCommand(const std::string& line, std::string& cmd, std::string& arg1, std::string& arg2);
void displayMenu();

#endif 