#include "Hash.h"
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>

using namespace std;

TEST(CoverageTest, ChainHashTableDisplay) {
    ChainHashTable table(5);
    
    testing::internal::CaptureStdout();
    table.display();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Таблица пуста") != string::npos);
    
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    table.insert("key3", "value3");
    
    testing::internal::CaptureStdout();
    table.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("key1=value1") != string::npos);
    EXPECT_TRUE(output.find("key2=value2") != string::npos);
    EXPECT_TRUE(output.find("key3=value3") != string::npos);
    
    ChainHashTable smallTable(1); // Все в одну ячейку
    smallTable.insert("a", "1");
    smallTable.insert("b", "2");
    
    testing::internal::CaptureStdout();
    smallTable.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a=1") != string::npos);
    EXPECT_TRUE(output.find("b=2") != string::npos);
    EXPECT_TRUE(output.find("->") != string::npos); // Проверяем цепочку
}

TEST(CoverageTest, ChainHashTableResize) {
    ChainHashTable table(5);
    
    for (int i = 0; i < 10; i++) {
        table.insert("key" + to_string(i), "value" + to_string(i));
    }
    
    int initialSize = table.getSize();
    
    testing::internal::CaptureStdout();
    table.resize(20);
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getCapacity(), 20);
    EXPECT_TRUE(output.find("Таблица изменена до 20 ячеек") != string::npos);
    
    for (int i = 0; i < 10; i++) {
        EXPECT_EQ(table.find("key" + to_string(i)), "value" + to_string(i));
    }
    
    testing::internal::CaptureStdout();
    table.resize(10);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getCapacity(), 10);
    
    testing::internal::CaptureStdout();
    table.resize(0);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Некорректный размер") != string::npos);
    EXPECT_GT(table.getCapacity(), 0);
    
    testing::internal::CaptureStdout();
    table.resize(-5);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Некорректный размер") != string::npos);
}

TEST(CoverageTest, OpenHashTableClear) {
    OpenHashTable table(10);
    
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    table.insert("key3", "value3");
    
    EXPECT_EQ(table.getSize(), 3);
    
    testing::internal::CaptureStdout();
    table.clear();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_TRUE(output.find("очищена") != string::npos);
    
    EXPECT_EQ(table.find("key1"), "");
    EXPECT_EQ(table.find("key2"), "");
    EXPECT_EQ(table.find("key3"), "");
    
    table.insert("newkey", "newvalue");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.find("newkey"), "newvalue");
}

TEST(CoverageTest, OpenHashTableDisplay) {
    OpenHashTable table(5);
    
    testing::internal::CaptureStdout();
    table.display();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Таблица пуста") != string::npos);
    
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    
    testing::internal::CaptureStdout();
    table.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("key1=value1") != string::npos);
    EXPECT_TRUE(output.find("key2=value2") != string::npos);
    
    table.remove("key1");
    
    testing::internal::CaptureStdout();
    table.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("[УДАЛЕНО]") != string::npos);
    
    OpenHashTable smallTable(3);
    smallTable.insert("a", "1");
    smallTable.insert("b", "2");
    smallTable.insert("c", "3");
    
    testing::internal::CaptureStdout();
    smallTable.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("a=1") != string::npos);
    EXPECT_TRUE(output.find("b=2") != string::npos);
    EXPECT_TRUE(output.find("c=3") != string::npos);
}

TEST(CoverageTest, OpenHashTableFileOperations) {
    OpenHashTable table(10);
    
    table.insert("data1", "value1");
    table.insert("data2", "value2");
    table.insert("data3", "value3");
    
    const string txtFile = "test_open_coverage.txt";
    const string binFile = "test_open_coverage.bin";
    
    testing::internal::CaptureStdout();
    table.saveToFile(txtFile);
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("сохранена") != string::npos);
    
    ifstream checkFile(txtFile);
    EXPECT_TRUE(checkFile.good());
    
    string firstLine;
    getline(checkFile, firstLine);
    EXPECT_EQ(firstLine, "OPEN_HASH_TABLE");
    
    string secondLine;
    getline(checkFile, secondLine);
    EXPECT_EQ(secondLine, "10");
    
    checkFile.close();
    
    OpenHashTable loadedTable(10);
    
    testing::internal::CaptureStdout();
    loadedTable.loadFromFile(txtFile);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("загружена") != string::npos);
    EXPECT_EQ(loadedTable.find("data1"), "value1");
    EXPECT_EQ(loadedTable.find("data2"), "value2");
    EXPECT_EQ(loadedTable.find("data3"), "value3");
    
    testing::internal::CaptureStdout();
    table.loadFromFile("nonexistent_file_12345.txt");
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Ошибка открытия") != string::npos);
    
    ofstream badFile("bad_format_open.txt");
    badFile << "WRONG_FORMAT\n10\n";
    badFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromFile("bad_format_open.txt");
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Неверный формат файла") != string::npos);
    
    ofstream diffCapFile("diff_capacity.txt");
    diffCapFile << "OPEN_HASH_TABLE\n15\nkey1 val1\nkey2 val2\n";
    diffCapFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromFile("diff_capacity.txt");
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getCapacity(), 15);
    
    remove(txtFile.c_str());
    remove("bad_format_open.txt");
    remove("diff_capacity.txt");
}

TEST(CoverageTest, DisplayMenuTest) {
    testing::internal::CaptureStdout();
    displayMenu();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("МЕНЮ ХЕШ-ТАБЛИЦ") != string::npos);
    EXPECT_TRUE(output.find("Добавить элемент") != string::npos);
    EXPECT_TRUE(output.find("Найти элемент") != string::npos);
    EXPECT_TRUE(output.find("Удалить элемент") != string::npos);
    EXPECT_TRUE(output.find("Показать таблицы") != string::npos);
    EXPECT_TRUE(output.find("Очистить таблицы") != string::npos);
    EXPECT_TRUE(output.find("Изменить размер таблиц") != string::npos);
    EXPECT_TRUE(output.find("Сохранить в файл") != string::npos);
    EXPECT_TRUE(output.find("Загрузить из файла") != string::npos);
    EXPECT_TRUE(output.find("Статистика") != string::npos);
    EXPECT_TRUE(output.find("Выход") != string::npos);
    EXPECT_TRUE(output.find("Выберите действие") != string::npos);
}

TEST(CoverageTest, ParseCommandTest) {
    string cmd, arg1, arg2;
    
    parseCommand("insert key value", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "insert");
    EXPECT_EQ(arg1, "key");
    EXPECT_EQ(arg2, "value");
    
    parseCommand("find mykey", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "find");
    EXPECT_EQ(arg1, "mykey");
    EXPECT_EQ(arg2, "");
    
    parseCommand("exit", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "exit");
    EXPECT_EQ(arg1, "");
    EXPECT_EQ(arg2, "");
    
    parseCommand("", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "");
    EXPECT_EQ(arg1, "");
    EXPECT_EQ(arg2, "");
    
    parseCommand("  command   arg1   arg2 with spaces  ", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "command");
    EXPECT_EQ(arg1, "arg1");
    EXPECT_EQ(arg2, "arg2 with spaces");
    
    parseCommand("   ", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "");
    EXPECT_EQ(arg1, "");
    EXPECT_EQ(arg2, "");
    
    parseCommand("add \"file name.txt\" \"C:\\Program Files\\\"", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "add");
    EXPECT_EQ(arg1, "\"file");
    EXPECT_EQ(arg2, "name.txt\" \"C:\\Program Files\\\"");
}

TEST(CoverageTest, ChainHashTableBinaryFileOperations) {
    ChainHashTable table(5);
    
    table.insert("user1", "Alice");
    table.insert("user2", "Bob");
    table.insert("user3", "Charlie");
    
    const string binFile = "test_chain_coverage.bin";
    
    testing::internal::CaptureStdout();
    table.saveToBinaryFile(binFile);
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("сохранена") != string::npos);
    
    ifstream checkFile(binFile, ios::binary);
    EXPECT_TRUE(checkFile.good());
    
    char signature[16];
    checkFile.read(signature, 16);
    string sigStr(signature, 16);
    EXPECT_TRUE(sigStr.find("CHAIN_HASH_BIN") != string::npos);
    
    checkFile.close();
    
    ChainHashTable loadedTable(5);
    
    testing::internal::CaptureStdout();
    loadedTable.loadFromBinaryFile(binFile);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("загружена") != string::npos);
    EXPECT_EQ(loadedTable.find("user1"), "Alice");
    EXPECT_EQ(loadedTable.find("user2"), "Bob");
    EXPECT_EQ(loadedTable.find("user3"), "Charlie");
    
    ChainHashTable diffTable(10);
    
    testing::internal::CaptureStdout();
    diffTable.loadFromBinaryFile(binFile);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(diffTable.getCapacity(), 5);
    
    testing::internal::CaptureStdout();
    table.loadFromBinaryFile("nonexistent_binary_12345.bin");
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Ошибка открытия") != string::npos);
    
    ofstream badBinFile("bad_binary_coverage.bin", ios::binary);
    const char* badSig = "WRONG_SIGNATURE123";
    badBinFile.write(badSig, 16);
    badBinFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromBinaryFile("bad_binary_coverage.bin");
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Неверный формат файла") != string::npos);
    
    ofstream emptyFile("empty_binary.bin", ios::binary);
    emptyFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromBinaryFile("empty_binary.bin");
    output = testing::internal::GetCapturedStdout();
    
    remove(binFile.c_str());
    remove("bad_binary_coverage.bin");
    remove("empty_binary.bin");
}

TEST(CoverageTest, OpenHashTableBinaryFileOperations) {
    OpenHashTable table(5);
    
    table.insert("data1", "value1");
    table.insert("data2", "value2");
    
    const string binFile = "test_open_coverage.bin";
    
    testing::internal::CaptureStdout();
    table.saveToBinaryFile(binFile);
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("сохранена") != string::npos);
    
    ifstream checkFile(binFile, ios::binary);
    EXPECT_TRUE(checkFile.good());
    
    char signature[16];
    checkFile.read(signature, 16);
    string sigStr(signature, 16);
    EXPECT_TRUE(sigStr.find("OPEN_HASH_BIN") != string::npos);
    
    checkFile.close();
    
    OpenHashTable loadedTable(5);
    
    testing::internal::CaptureStdout();
    loadedTable.loadFromBinaryFile(binFile);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("загружена") != string::npos);
    EXPECT_EQ(loadedTable.find("data1"), "value1");
    EXPECT_EQ(loadedTable.find("data2"), "value2");
    
    remove(binFile.c_str());
}

// Тест для улучшения покрытия loadFromFile
TEST(CoverageTest, ChainHashTableLoadFromFileEdgeCases) {
    ChainHashTable table(10);
    
    ofstream file("diff_cap_chain.txt");
    file << "CHAIN_HASH_TABLE\n15\nkey1 val1\nkey2 val2\nkey3 val3\n";
    file.close();
    
    testing::internal::CaptureStdout();
    table.loadFromFile("diff_cap_chain.txt");
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getCapacity(), 15);
    EXPECT_EQ(table.find("key1"), "val1");
    EXPECT_EQ(table.find("key2"), "val2");
    EXPECT_EQ(table.find("key3"), "val3");
    
    ofstream emptyFile("empty_chain.txt");
    emptyFile << "CHAIN_HASH_TABLE\n10\n";
    emptyFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromFile("empty_chain.txt");
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getSize(), 0);
    
    remove("diff_cap_chain.txt");
    remove("empty_chain.txt");
}

TEST(CoverageTest, OpenHashTableResizeEdgeCases) {
    OpenHashTable table(5);
    
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    table.insert("key3", "value3");
    
    testing::internal::CaptureStdout();
    table.resize(10);
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getCapacity(), 10);
    EXPECT_TRUE(output.find("Таблица изменена") != string::npos);
    
    EXPECT_EQ(table.find("key1"), "value1");
    EXPECT_EQ(table.find("key2"), "value2");
    EXPECT_EQ(table.find("key3"), "value3");
    
    testing::internal::CaptureStdout();
    table.resize(8);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(table.getCapacity(), 8);
    
    testing::internal::CaptureStdout();
    table.resize(0);
    output = testing::internal::GetCapturedStdout();
    
    EXPECT_TRUE(output.find("Некорректный размер") != string::npos);
    EXPECT_EQ(table.getCapacity(), 8); 
}

// Тест для getLoadFactor
TEST(CoverageTest, OpenHashTableGetLoadFactor) {
    OpenHashTable table(10);
    
    EXPECT_DOUBLE_EQ(table.getLoadFactor(), 0.0);
    
    table.insert("key1", "value1");
    EXPECT_DOUBLE_EQ(table.getLoadFactor(), 0.1); 
    
    table.insert("key2", "value2");
    EXPECT_DOUBLE_EQ(table.getLoadFactor(), 0.2); 
    
    table.insert("key3", "value3");
    EXPECT_DOUBLE_EQ(table.getLoadFactor(), 0.3); 
    
    table.remove("key2");
    EXPECT_DOUBLE_EQ(table.getLoadFactor(), 0.2);
    
    OpenHashTable fullTable(3);
    fullTable.insert("a", "1");
    fullTable.insert("b", "2");
    fullTable.insert("c", "3");
    
    EXPECT_DOUBLE_EQ(fullTable.getLoadFactor(), 1.0);
}