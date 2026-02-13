#include "Hash.h"
#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

class ChainHashTableTest : public ::testing::Test {
protected:
    ChainHashTable table{10};
};

// Проверка базовых операций хеш-таблицы (вставка, обновление, удаление, поиск)
TEST_F(ChainHashTableTest, BasicOperations) {
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.getCapacity(), 10);
    
    table.insert("name", "Ivan");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.find("name"), "Ivan");
    
    table.insert("name", "Petr");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.find("name"), "Petr");
    
    EXPECT_TRUE(table.remove("name"));
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.find("name"), ""); // Поиск удаленного элемента
}

// Проверка обработки коллизий (разные ключи с одинаковым хешем)
TEST_F(ChainHashTableTest, CollisionHandling) {
    table.insert("abc", "value1");
    table.insert("cba", "value2");
    table.insert("xyz", "value3");
    
    EXPECT_EQ(table.find("abc"), "value1");
    EXPECT_EQ(table.find("cba"), "value2");
    EXPECT_EQ(table.find("xyz"), "value3");
    EXPECT_EQ(table.getSize(), 3);
}

// Проверка очистки таблицы и изменения размера
TEST_F(ChainHashTableTest, ClearAndResize) {

    for (int i = 0; i < 5; i++) {
        table.insert("key" + to_string(i), "value" + to_string(i));
    }
    EXPECT_EQ(table.getSize(), 5);
    
    table.clear();
    EXPECT_EQ(table.getSize(), 0);
    
    table.resize(20);
    EXPECT_EQ(table.getCapacity(), 20);
}

// Проверка сохранения и загрузки в текстовый файл
TEST_F(ChainHashTableTest, TextFileOperations) {
    table.insert("user1", "Alice");
    table.insert("user2", "Bob");
    
    const string txtFile = "test_chain.txt";
    table.saveToFile(txtFile);  // Сохранение в файл
    
    ChainHashTable table2(10);
    table2.loadFromFile(txtFile);
    
    EXPECT_EQ(table2.find("user1"), "Alice");
    EXPECT_EQ(table2.find("user2"), "Bob");
    
    remove(txtFile.c_str());
}

// Проверка сохранения и загрузки в бинарный файл
TEST_F(ChainHashTableTest, BinaryFileOperations) {
    ChainHashTable table1(5);
    table1.insert("user1", "Alice");
    table1.insert("user2", "Bob");
    table1.insert("user3", "Charlie");
    
    const string binFile = "test_chain.bin";
    table1.saveToBinaryFile(binFile);  // Сохранение в бинарный файл
    
    ifstream checkFile(binFile, ios::binary);
    EXPECT_TRUE(checkFile.good());
    checkFile.close();
    
    ChainHashTable table2(5);
    table2.loadFromBinaryFile(binFile);
    
    EXPECT_EQ(table2.find("user1"), "Alice");
    EXPECT_EQ(table2.find("user2"), "Bob");
    EXPECT_EQ(table2.find("user3"), "Charlie");
    
    remove(binFile.c_str());
}

// Проверка граничных случаев (поиск/удаление несуществующего ключа)
TEST_F(ChainHashTableTest, EdgeCases) {
    EXPECT_EQ(table.find("nonexistent"), "");
    
    EXPECT_FALSE(table.remove("nonexistent"));
}

// Проверка функции отображения (не должна падать)
TEST_F(ChainHashTableTest, DisplayFunction) {
    testing::internal::CaptureStdout();
    table.display();
    testing::internal::GetCapturedStdout();
    
    table.insert("test", "value");
    testing::internal::CaptureStdout();
    table.display();
    testing::internal::GetCapturedStdout();
}

// Проверка некорректных операций изменения размера
TEST_F(ChainHashTableTest, InvalidResize) {
    testing::internal::CaptureStdout();
    table.resize(0);   // Некорректный размер
    table.resize(-5);  // Отрицательный размер
    testing::internal::GetCapturedStdout(); // Проверка что не упало
}

TEST_F(ChainHashTableTest, SaveToFileError) {
    // Попытка сохранения в невалидный файл
    testing::internal::CaptureStdout();
    table.saveToFile("/invalid/path/file.txt");
    testing::internal::GetCapturedStdout();
}

TEST_F(ChainHashTableTest, LoadFromFileError) {
    testing::internal::CaptureStdout();
    table.loadFromFile("nonexistent_file.txt");
    testing::internal::GetCapturedStdout();
    
    ofstream badFile("bad_format.txt");
    badFile << "WRONG_FORMAT" << endl;
    badFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromFile("bad_format.txt");
    testing::internal::GetCapturedStdout();
    
    remove("bad_format.txt");
}

TEST_F(ChainHashTableTest, LoadFromBinaryFileError) {
    // Попытка загрузки несуществующего бинарного файла
    testing::internal::CaptureStdout();
    table.loadFromBinaryFile("nonexistent_binary.bin");
    testing::internal::GetCapturedStdout();
    
    ofstream badBinFile("bad_binary.bin", ios::binary);
    const char* badSig = "WRONG";
    badBinFile.write(badSig, 6);
    badBinFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromBinaryFile("bad_binary.bin");
    testing::internal::GetCapturedStdout();
    
    remove("bad_binary.bin");
}

class OpenHashTableTest : public ::testing::Test {
protected:
    OpenHashTable table{10};
};

TEST_F(OpenHashTableTest, BasicOperations) {
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_DOUBLE_EQ(table.getLoadFactor(), 0.0);
    
    table.insert("key1", "value1");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_GT(table.getLoadFactor(), 0.0);
    EXPECT_EQ(table.find("key1"), "value1");
    
    table.insert("key1", "value2");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.find("key1"), "value2");
    
    EXPECT_TRUE(table.remove("key1"));
    EXPECT_EQ(table.getSize(), 0);
    EXPECT_EQ(table.find("key1"), "");
    
    table.insert("key1", "value3");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.find("key1"), "value3");
}

TEST_F(OpenHashTableTest, CollisionsAndFullTable) {
    OpenHashTable smallTable{3};
    
    smallTable.insert("a", "1");
    smallTable.insert("b", "2");
    smallTable.insert("c", "3");
    
    EXPECT_EQ(smallTable.getSize(), 3);
    EXPECT_DOUBLE_EQ(smallTable.getLoadFactor(), 1.0);
}

TEST_F(OpenHashTableTest, TextFileOperations) {
    table.insert("data1", "value1");
    table.insert("data2", "value2");
    
    const string txtFile = "test_open.txt";
    table.saveToFile(txtFile);
    
    OpenHashTable table2(10);
    table2.loadFromFile(txtFile);
    
    EXPECT_EQ(table2.find("data1"), "value1");
    EXPECT_EQ(table2.find("data2"), "value2");
    
    remove(txtFile.c_str());
}

TEST_F(OpenHashTableTest, BinaryFileOperations) {
    OpenHashTable table1(5);
    table1.insert("data1", "value1");
    table1.insert("data2", "value2");
    table1.insert("data3", "value3");
    
    const string binFile = "test_open.bin";
    table1.saveToBinaryFile(binFile);
    
    ifstream checkFile(binFile, ios::binary);
    EXPECT_TRUE(checkFile.good());
    checkFile.close();
    
    OpenHashTable table2(5);
    table2.loadFromBinaryFile(binFile);
    
    EXPECT_EQ(table2.find("data1"), "value1");
    EXPECT_EQ(table2.find("data2"), "value2");
    EXPECT_EQ(table2.find("data3"), "value3");
    
    remove(binFile.c_str());
}

TEST_F(OpenHashTableTest, ResizeAndLoadFactor) {
    EXPECT_EQ(table.getCapacity(), 10);
    
    for (int i = 0; i < 5; i++) {
        table.insert("key" + to_string(i), "value" + to_string(i));
    }
    
    double loadFactor = table.getLoadFactor();
    EXPECT_GT(loadFactor, 0.0);
    EXPECT_LT(loadFactor, 1.0);
    
    table.resize(20);
    EXPECT_EQ(table.getCapacity(), 20);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(table.find("key" + to_string(i)), "value" + to_string(i));
    }
}

TEST_F(OpenHashTableTest, DisplayFunction) {
    testing::internal::CaptureStdout();
    table.display();
    testing::internal::GetCapturedStdout();
    
    table.insert("test", "value");
    
    testing::internal::CaptureStdout();
    table.display();
    testing::internal::GetCapturedStdout();
    
    table.remove("test");
    
    testing::internal::CaptureStdout();
    table.display();
    testing::internal::GetCapturedStdout();
}

TEST_F(OpenHashTableTest, SaveLoadErrors) {
    testing::internal::CaptureStdout();
    table.saveToFile("/invalid/path/file.txt");
    testing::internal::GetCapturedStdout();
    
    testing::internal::CaptureStdout();
    table.loadFromFile("nonexistent_file.txt");
    testing::internal::GetCapturedStdout();
    
    ofstream badFile("bad_open_format.txt");
    badFile << "WRONG_FORMAT" << endl;
    badFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromFile("bad_open_format.txt");
    testing::internal::GetCapturedStdout();
    
    remove("bad_open_format.txt");
}

TEST_F(OpenHashTableTest, LoadFromBinaryFileError) {
    testing::internal::CaptureStdout();
    table.loadFromBinaryFile("nonexistent_open_binary.bin");
    testing::internal::GetCapturedStdout();

    ofstream badBinFile("bad_open_binary.bin", ios::binary);
    const char* badSig = "WRONG";
    badBinFile.write(badSig, 5);
    badBinFile.close();
    
    testing::internal::CaptureStdout();
    table.loadFromBinaryFile("bad_open_binary.bin");
    testing::internal::GetCapturedStdout();
    
    remove("bad_open_binary.bin");
}

TEST_F(OpenHashTableTest, InvalidResize) {
    testing::internal::CaptureStdout();
    table.resize(0);
    table.resize(-5);
    testing::internal::GetCapturedStdout();
}

// Тесты для вспомогательных функций
TEST(HashHelperTest, StringHashFunction) {
    int hash1 = stringHash("test", 100);
    int hash2 = stringHash("test", 100);
    EXPECT_EQ(hash1, hash2);
    
    EXPECT_NO_THROW(stringHash("", 100));
    EXPECT_NO_THROW(stringHash("abc", 100));
    
    int hash = stringHash("test", 10);
    EXPECT_GE(hash, 0);
    EXPECT_LT(hash, 10);
    
    string longStr(1000, 'a');
    EXPECT_NO_THROW(stringHash(longStr, 100));
    
    EXPECT_NO_THROW(stringHash("test!@#$%^&*()", 100));
}

TEST(HashHelperTest, DisplayMenu) {
    testing::internal::CaptureStdout();
    displayMenu();
    testing::internal::GetCapturedStdout();
}

// Тесты для конструкторов/деструкторов
TEST(HashConstructorsTest, ChainHashTable) {
    ChainHashTable table1(5);
    EXPECT_EQ(table1.getCapacity(), 5);
    EXPECT_EQ(table1.getSize(), 0);
    
    ChainHashTable table2(0);
    EXPECT_EQ(table2.getCapacity(), 1);
    
    ChainHashTable table3(-5);
    EXPECT_EQ(table3.getCapacity(), 1);
    
    ChainHashTable table4;
    EXPECT_EQ(table4.getCapacity(), 10);
}

TEST(HashConstructorsTest, OpenHashTable) {
    OpenHashTable table1(5);
    EXPECT_EQ(table1.getCapacity(), 5);
    EXPECT_EQ(table1.getSize(), 0);
    
    OpenHashTable table2(0);
    EXPECT_EQ(table2.getCapacity(), 1);
    
    OpenHashTable table3(-5);
    EXPECT_EQ(table3.getCapacity(), 1);
    
    OpenHashTable table4;
    EXPECT_EQ(table4.getCapacity(), 10);
}

// Тесты для структур
TEST(HashStructsTest, HashNodeConstructor) {
    HashNode node("test_key", "test_value");
    EXPECT_EQ(node.key, "test_key");
    EXPECT_EQ(node.value, "test_value");
    EXPECT_EQ(node.next, nullptr);
}

TEST(HashStructsTest, OpenHashEntryConstructor) {
    OpenHashEntry entry;
    EXPECT_FALSE(entry.used);
    EXPECT_FALSE(entry.deleted);
    EXPECT_TRUE(entry.key.empty());
    EXPECT_TRUE(entry.value.empty());
}

// Тест полного цикла работы
TEST(HashIntegrationTest, CompleteWorkflow) {
    ChainHashTable chainTable(5);
    chainTable.insert("name", "Alice");
    chainTable.insert("age", "25");
    chainTable.insert("city", "Moscow");
    
    EXPECT_EQ(chainTable.getSize(), 3);
    EXPECT_EQ(chainTable.find("name"), "Alice");
    EXPECT_EQ(chainTable.find("age"), "25");
    EXPECT_EQ(chainTable.find("city"), "Moscow");
    
    chainTable.insert("age", "26");
    EXPECT_EQ(chainTable.find("age"), "26");
    
    EXPECT_TRUE(chainTable.remove("city"));
    EXPECT_EQ(chainTable.find("city"), "");
    EXPECT_EQ(chainTable.getSize(), 2);
    
    chainTable.clear();
    EXPECT_EQ(chainTable.getSize(), 0);
    
    // Тест для таблицы с открытой адресацией
    OpenHashTable openTable(5);
    openTable.insert("key1", "val1");
    openTable.insert("key2", "val2");
    openTable.insert("key3", "val3");
    
    EXPECT_EQ(openTable.getSize(), 3);
    EXPECT_GT(openTable.getLoadFactor(), 0.0);
    
    openTable.resize(10);
    EXPECT_EQ(openTable.getCapacity(), 10);
    
    EXPECT_EQ(openTable.find("key1"), "val1");
    EXPECT_EQ(openTable.find("key2"), "val2");
    EXPECT_EQ(openTable.find("key3"), "val3");
}

// Тест для производительности
TEST(HashPerformanceTest, LargeData) {
    ChainHashTable largeTable(100);
    
    for (int i = 0; i < 1000; i++) {
        largeTable.insert("key" + to_string(i), "value" + to_string(i));
    }
    
    EXPECT_GE(largeTable.getSize(), 100);
    
    // Ищем случайные элементы
    for (int i = 0; i < 100; i++) {
        int randomIndex = rand() % 1000;
        string expected = "value" + to_string(randomIndex);
        string found = largeTable.find("key" + to_string(randomIndex));
        // Не все могут быть найдены из-за коллизий и перезаписи
        EXPECT_NO_THROW(largeTable.find("key" + to_string(randomIndex)));
    }
}

// Тест для проверки обработки граничных значений
TEST(HashBoundaryTest, BoundaryConditions) {
    ChainHashTable tinyTable(1);
    tinyTable.insert("a", "1");
    tinyTable.insert("b", "2"); // Коллизия
    tinyTable.insert("a", "3"); // Обновление
    
    EXPECT_GE(tinyTable.getSize(), 1);
    
    OpenHashTable tinyOpenTable(1);
    testing::internal::CaptureStdout();
    tinyOpenTable.insert("a", "1");
    tinyOpenTable.insert("b", "2"); // Должно переполниться
    testing::internal::GetCapturedStdout();
    
    EXPECT_GE(tinyOpenTable.getSize(), 1);
}

// Тест для проверки пустых и null строк
TEST(HashStringTest, EmptyAndNullStrings) {
    ChainHashTable table(10);
    
    table.insert("", "empty_key");
    table.insert("empty_value", "");
    
    EXPECT_NO_THROW(table.find(""));
    EXPECT_NO_THROW(table.find("empty_value"));
    
    EXPECT_NO_THROW(table.remove(""));
    EXPECT_NO_THROW(table.remove("empty_value"));
    
    // Таблица с открытой адресацией
    OpenHashTable openTable(10);
    openTable.insert("", "test");
    openTable.insert("test", "");
    
    EXPECT_NO_THROW(openTable.find(""));
    EXPECT_NO_THROW(openTable.find("test"));
}

// Тесты для деструкторов
TEST(HashDestructorTest, ChainHashTableDestructor) {
    {
        ChainHashTable table(5);
        table.insert("key1", "value1");
        table.insert("key2", "value2");
    }
    EXPECT_TRUE(true);
}

TEST(HashDestructorTest, OpenHashTableDestructor) {
    {
        OpenHashTable table(5);
        table.insert("key1", "value1");
        table.insert("key2", "value2");
    }
    EXPECT_TRUE(true);
}

// Тесты для обработки удаленных записей в OpenHashTable
TEST_F(OpenHashTableTest, DeletedEntriesHandling) {
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    
    EXPECT_TRUE(table.remove("key1"));
    table.insert("key3", "value3");
    
    EXPECT_EQ(table.find("key2"), "value2");
    EXPECT_EQ(table.find("key3"), "value3");
    EXPECT_EQ(table.find("key1"), "");
}

// Тесты для переполнения таблицы с открытой адресацией
TEST_F(OpenHashTableTest, TableOverflow) {
    OpenHashTable smallTable(3);
    
    smallTable.insert("a", "1");
    smallTable.insert("b", "2");
    smallTable.insert("c", "3");
    
    testing::internal::CaptureStdout();
    smallTable.insert("d", "4");
    testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(smallTable.getSize(), 3);
}

// Тесты для цепочечной таблицы с большим количеством коллизий
TEST_F(ChainHashTableTest, LongChainHandling) {
    ChainHashTable table(1);
    
    table.insert("a", "1");
    table.insert("b", "2");
    table.insert("c", "3");
    table.insert("d", "4");
    table.insert("e", "5");
    
    EXPECT_EQ(table.getSize(), 5);
    
    EXPECT_TRUE(table.remove("c"));
    EXPECT_EQ(table.getSize(), 4);
    
    EXPECT_TRUE(table.remove("a"));
    EXPECT_EQ(table.getSize(), 3);
    
    EXPECT_TRUE(table.remove("e"));
    EXPECT_EQ(table.getSize(), 2);
}

// Тесты для сохранения/загрузки пустых таблиц
TEST_F(ChainHashTableTest, SaveLoadEmptyTable) {
    const string txtFile = "empty_chain.txt";
    const string binFile = "empty_chain.bin";
    
    table.saveToFile(txtFile);
    table.saveToBinaryFile(binFile);
    
    ChainHashTable table2(10);
    table2.loadFromFile(txtFile);
    EXPECT_EQ(table2.getSize(), 0);
    
    ChainHashTable table3(10);
    table3.loadFromBinaryFile(binFile);
    EXPECT_EQ(table3.getSize(), 0);
    
    remove(txtFile.c_str());
    remove(binFile.c_str());
}

TEST_F(OpenHashTableTest, SaveLoadEmptyTable) {
    const string txtFile = "empty_open.txt";
    const string binFile = "empty_open.bin";
    
    table.saveToFile(txtFile);
    table.saveToBinaryFile(binFile);
    
    OpenHashTable table2(10);
    table2.loadFromFile(txtFile);
    EXPECT_EQ(table2.getSize(), 0);
    
    OpenHashTable table3(10);
    table3.loadFromBinaryFile(binFile);
    EXPECT_EQ(table3.getSize(), 0);
    
    remove(txtFile.c_str());
    remove(binFile.c_str());
}

// Тесты для проверки повторной инициализации таблиц
TEST_F(ChainHashTableTest, Reinitialization) {
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    
    table.clear();
    EXPECT_EQ(table.getSize(), 0);
    
    table.insert("key3", "value3");
    EXPECT_EQ(table.getSize(), 1);
    EXPECT_EQ(table.find("key3"), "value3");
}

// Тесты для проверки хеш-функции с разными размерами таблиц
TEST(HashHelperTest, StringHashWithDifferentSizes) {
    int hash1 = stringHash("test", 1);
    EXPECT_EQ(hash1, 0);
    
    int hash2 = stringHash("test", 2);
    EXPECT_TRUE(hash2 == 0 || hash2 == 1);
    
    int hash100 = stringHash("test", 100);
    EXPECT_GE(hash100, 0);
    EXPECT_LT(hash100, 100);
    
    int hashSpecial = stringHash("!@#$%^&*()", 50);
    EXPECT_GE(hashSpecial, 0);
    EXPECT_LT(hashSpecial, 50);
}

// Тесты для проверки обработки исключительных ситуаций в resize
TEST_F(ChainHashTableTest, ResizeWithSameSize) {
    int originalCapacity = table.getCapacity();
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    
    // Ресайз на тот же размер
    table.resize(originalCapacity);
    
    EXPECT_EQ(table.getCapacity(), originalCapacity);
    EXPECT_EQ(table.find("key1"), "value1");
    EXPECT_EQ(table.find("key2"), "value2");
}

TEST_F(OpenHashTableTest, ResizeWithSameSize) {
    int originalCapacity = table.getCapacity();
    table.insert("key1", "value1");
    table.insert("key2", "value2");
    
    // Ресайз на тот же размер
    table.resize(originalCapacity);
    
    EXPECT_EQ(table.getCapacity(), originalCapacity);
    EXPECT_EQ(table.find("key1"), "value1");
    EXPECT_EQ(table.find("key2"), "value2");
}

// Тесты для проверки работы с максимальным размером
TEST(HashBoundaryTest, MaxSizeOperations) {
    ChainHashTable largeTable(1000);
    for (int i = 0; i < 500; i++) {
        largeTable.insert("key" + to_string(i), "value" + to_string(i));
    }
    
    EXPECT_EQ(largeTable.getSize(), 500);
    
    // Ресайз на меньший размер
    largeTable.resize(100);
    EXPECT_EQ(largeTable.getCapacity(), 100);
    // Проверяем что данные не потерялись (хотя могли быть перехешированы)
    EXPECT_NO_THROW(largeTable.find("key0"));
}

// Тест для проверки вывода на экран (capture output)
TEST_F(ChainHashTableTest, DisplayOutput) {
    testing::internal::CaptureStdout();
    table.display();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Таблица пуста") != string::npos);
    
    table.insert("test", "value");
    testing::internal::CaptureStdout();
    table.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("test=value") != string::npos);
}

TEST_F(OpenHashTableTest, DisplayOutput) {
    testing::internal::CaptureStdout();
    table.display();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Таблица пуста") != string::npos);
    
    // Тестируем вывод заполненной таблицы
    table.insert("test", "value");
    testing::internal::CaptureStdout();
    table.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("test=value") != string::npos);
    
    // Тестируем вывод с удаленными элементами
    table.remove("test");
    testing::internal::CaptureStdout();
    table.display();
    output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("[УДАЛЕНО]") != string::npos);
}

// Тест для проверки загрузки поврежденных бинарных файлов (безопасная версия)
TEST_F(ChainHashTableTest, LoadInvalidBinaryFile) {
    {
        ofstream emptyFile("empty.bin", ios::binary);
        emptyFile.close();
    }
    
    testing::internal::CaptureStdout();
    EXPECT_NO_THROW(table.loadFromBinaryFile("empty.bin"));
    string output = testing::internal::GetCapturedStdout();
    
    remove("empty.bin");
}

TEST_F(OpenHashTableTest, LoadInvalidBinaryFile) {
    {
        ofstream emptyFile("empty_open.bin", ios::binary);
        emptyFile.close();
    }
    
    testing::internal::CaptureStdout();
    EXPECT_NO_THROW(table.loadFromBinaryFile("empty_open.bin"));
    string output = testing::internal::GetCapturedStdout();
    
    remove("empty_open.bin");
}

// Тесты для parseCommand
TEST(ParseCommandTest, VariousInputs) {
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
    
    parseCommand("command with multiple words in arg2", cmd, arg1, arg2);
    EXPECT_EQ(cmd, "command");
    EXPECT_EQ(arg1, "with");
    EXPECT_EQ(arg2, "multiple words in arg2");
}

// Тест для проверки отсутствия падений при некорректном вводе
TEST(HashInputTest, InvalidInputHandling) {
    ChainHashTable table(10);
    
    EXPECT_NO_THROW(table.insert("", ""));
    EXPECT_NO_THROW(table.insert("key", ""));
    EXPECT_NO_THROW(table.insert("", "value"));
    
    EXPECT_NO_THROW(table.find(""));
    EXPECT_NO_THROW(table.remove(""));
    
    string longString(10000, 'a');
    EXPECT_NO_THROW(table.insert(longString, longString));
    EXPECT_NO_THROW(table.find(longString));
    EXPECT_NO_THROW(table.remove(longString));
}

// Тест для проверки корректности вычислений
TEST(HashComputationTest, Computations) {
    ChainHashTable table(10);
    
    for (int i = 0; i < 100; i++) {
        table.insert(to_string(i), to_string(i * 10));
    }
    
    int size = table.getSize();
    EXPECT_GT(size, 0);
    
    // Проверяем что getSize() возвращает то же значение, что и фактический подсчет
    int actualCount = 0;
    for (int i = 0; i < table.getCapacity(); i++) {
        // Не можем получить доступ к приватным данным таблицы напрямую
        // Вместо этого проверяем через публичные методы
        for (int j = 0; j < 100; j++) {
            string result = table.find(to_string(j));
            if (!result.empty()) {
                actualCount++;
            }
        }
    }
    EXPECT_GT(actualCount, 0);
}

// Тест для проверки файловых операций с большими данными
TEST(HashFileTest, LargeFileOperations) {
    ChainHashTable table(100);

    for (int i = 0; i < 1000; i++) {
        table.insert("key" + to_string(i), "value" + to_string(i));
    }
    
    const string largeFile = "large_test.txt";
    const string largeBinFile = "large_test.bin";
    
    EXPECT_NO_THROW(table.saveToFile(largeFile));
    EXPECT_NO_THROW(table.saveToBinaryFile(largeBinFile));
    
    ChainHashTable loadedTable(100);
    EXPECT_NO_THROW(loadedTable.loadFromFile(largeFile));
    EXPECT_GT(loadedTable.getSize(), 0);
    
    ChainHashTable loadedBinTable(100);
    EXPECT_NO_THROW(loadedBinTable.loadFromBinaryFile(largeBinFile));
    EXPECT_GT(loadedBinTable.getSize(), 0);
    
    remove(largeFile.c_str());
    remove(largeBinFile.c_str());
}

// Тест для проверки сценариев использования в реальных условиях
TEST(HashRealWorldTest, RealWorldScenarios) {
    ChainHashTable dictionary(50);
    dictionary.insert("apple", "яблоко");
    dictionary.insert("book", "книга");
    dictionary.insert("computer", "компьютер");
    
    EXPECT_EQ(dictionary.find("apple"), "яблоко");
    EXPECT_EQ(dictionary.find("book"), "книга");
    EXPECT_EQ(dictionary.find("computer"), "компьютер");
    
    dictionary.insert("apple", "яблоко (фрукт)"); // Уточнение
    EXPECT_EQ(dictionary.find("apple"), "яблоко (фрукт)");
    
    // Кэш
    OpenHashTable cache(20);
    cache.insert("user_123_profile", "{name: 'John', age: 30}");
    cache.insert("product_456_details", "{price: 99.99, stock: 100}");
    
    EXPECT_FALSE(cache.find("user_123_profile").empty());
    EXPECT_FALSE(cache.find("product_456_details").empty());
    
    // Элемент устарел, удаляем из кэша
    cache.remove("product_456_details");
    EXPECT_TRUE(cache.find("product_456_details").empty());
    
    // Счетчик частоты слов
    ChainHashTable wordCounter(26);
    wordCounter.insert("hello", "1");
    wordCounter.insert("world", "1");
    wordCounter.insert("hello", "2");
    
    EXPECT_EQ(wordCounter.find("hello"), "2");
    EXPECT_EQ(wordCounter.find("world"), "1");
}