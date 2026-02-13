#include "DoublyLinkedList.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

using namespace std;

class DoublyLinkedListTest : public ::testing::Test {
protected:
    DoublyLinkedList list;
    
    void SetUp() override {
        filesystem::remove("test_doubly.txt");
        filesystem::remove("test_doubly.bin");
    }
    
    void TearDown() override {
        filesystem::remove("test_doubly.txt");
        filesystem::remove("test_doubly.bin");
    }
};

TEST_F(DoublyLinkedListTest, BasicOperations) {
    list.addToHead(10);
    list.addToTail(20);
    
    // Поиск
    EXPECT_TRUE(list.findByValue(10));
    EXPECT_TRUE(list.findByValue(20));
    EXPECT_FALSE(list.findByValue(30));
}

TEST_F(DoublyLinkedListTest, AddAfterValue) {
    list.addToTail(1);
    list.addToTail(2);
    list.addToTail(3);
    
    list.addAfterValue(2, 99);
    
    EXPECT_TRUE(list.findByValue(99));
}

// Тест добавления перед значением
TEST_F(DoublyLinkedListTest, AddBeforeValue) {
    list.addToTail(1);
    list.addToTail(2);
    list.addToTail(3);
    
    list.addBeforeValue(2, 99);
    
    EXPECT_TRUE(list.findByValue(99));
}

// Тест удаления из начала и конца
TEST_F(DoublyLinkedListTest, DeleteFromHeadAndTail) {
    list.addToTail(10);
    list.addToTail(20);
    list.addToTail(30);
    
    list.deleteFromHead();
    EXPECT_FALSE(list.findByValue(10));
    EXPECT_TRUE(list.findByValue(20));
    EXPECT_TRUE(list.findByValue(30));
    
    list.deleteFromTail();
    EXPECT_FALSE(list.findByValue(30));
    EXPECT_TRUE(list.findByValue(20));
}

// Тест удаления по значению
TEST_F(DoublyLinkedListTest, DeleteByValue) {
    list.addToTail(1);
    list.addToTail(2);
    list.addToTail(3);

    list.deleteByValue(2);
    EXPECT_FALSE(list.findByValue(2));
    EXPECT_TRUE(list.findByValue(1));
    EXPECT_TRUE(list.findByValue(3));
    
    EXPECT_NO_THROW(list.deleteByValue(99));
}

// Тест удаления после значения
TEST_F(DoublyLinkedListTest, DeleteAfterValue) {
    list.addToTail(10);
    list.addToTail(20);
    list.addToTail(30);
    
    list.deleteAfterValue(20);
    EXPECT_FALSE(list.findByValue(30));
    EXPECT_TRUE(list.findByValue(10));
    EXPECT_TRUE(list.findByValue(20));
    
    EXPECT_NO_THROW(list.deleteAfterValue(20));
}

// Тест удаления перед значением
TEST_F(DoublyLinkedListTest, DeleteBeforeValue) {
    list.addToTail(10);
    list.addToTail(20);
    list.addToTail(30);
    
    list.deleteBeforeValue(20);
    EXPECT_FALSE(list.findByValue(10));
    EXPECT_TRUE(list.findByValue(20));
    EXPECT_TRUE(list.findByValue(30));
}

TEST_F(DoublyLinkedListTest, PrintOperations) {
    list.addToTail(1);
    list.addToTail(2);
    list.addToTail(3);
    
    EXPECT_NO_THROW(list.printForward());
    EXPECT_NO_THROW(list.printBackward());
}

TEST_F(DoublyLinkedListTest, ClearList) {
    list.addToTail(100);
    list.addToTail(200);
    list.addToTail(300);
    list.clear();
    
    EXPECT_FALSE(list.findByValue(100));
    EXPECT_FALSE(list.findByValue(200));
    EXPECT_FALSE(list.findByValue(300));
}

TEST_F(DoublyLinkedListTest, FileOperations) {
    list.addToTail(111);
    list.addToTail(222);
    list.addToTail(333);
    
    const string txtFile = "test_doubly.txt";
    
    EXPECT_NO_THROW(list.saveToFile(txtFile));
    
    DoublyLinkedList listFromTxt;
    EXPECT_NO_THROW(listFromTxt.loadFromFile(txtFile));
    
    EXPECT_TRUE(listFromTxt.findByValue(111));
    EXPECT_TRUE(listFromTxt.findByValue(222));
    EXPECT_TRUE(listFromTxt.findByValue(333));
    const string binFile = "test_doubly.bin";
    
    EXPECT_NO_THROW(list.saveToBinaryFile(binFile));
    
    DoublyLinkedList listFromBin;
    EXPECT_NO_THROW(listFromBin.loadFromBinaryFile(binFile));
    
    EXPECT_TRUE(listFromBin.findByValue(111));
    EXPECT_TRUE(listFromBin.findByValue(222));
    EXPECT_TRUE(listFromBin.findByValue(333));
}

// Тест граничных случаев
TEST_F(DoublyLinkedListTest, EdgeCases) {
    EXPECT_NO_THROW(list.deleteFromHead());
    EXPECT_NO_THROW(list.deleteFromTail());
    EXPECT_NO_THROW(list.deleteByValue(99));
    EXPECT_NO_THROW(list.deleteAfterValue(10));
    EXPECT_NO_THROW(list.deleteBeforeValue(10));
    EXPECT_NO_THROW(list.printForward());
    EXPECT_NO_THROW(list.printBackward());

    list.addToHead(1);
    EXPECT_TRUE(list.findByValue(1));
    
    list.clear();

    list.addToTail(1);
    EXPECT_TRUE(list.findByValue(1));

    list.clear();
    EXPECT_NO_THROW(list.addAfterValue(1, 2));
    EXPECT_NO_THROW(list.addBeforeValue(1, 2));

    list.clear();
    list.addToHead(42);
    EXPECT_NO_THROW(list.deleteFromHead());
    EXPECT_TRUE(list.findByValue(42) == false);
    
    list.addToTail(42);
    EXPECT_NO_THROW(list.deleteFromTail());
    EXPECT_TRUE(list.findByValue(42) == false);
}

// Тест с большим количеством элементов
TEST_F(DoublyLinkedListTest, LargeListOperations) {
    const int ELEMENT_COUNT = 100;

    for (int i = 0; i < ELEMENT_COUNT; i++) {
        list.addToTail(i);
    }
    for (int i = 0; i < ELEMENT_COUNT; i++) {
        EXPECT_TRUE(list.findByValue(i));
    }
    for (int i = 0; i < ELEMENT_COUNT / 2; i++) {
        list.deleteByValue(i);
    }
    for (int i = 0; i < ELEMENT_COUNT / 2; i++) {
        EXPECT_FALSE(list.findByValue(i));
    }
    for (int i = ELEMENT_COUNT / 2; i < ELEMENT_COUNT; i++) {
        EXPECT_TRUE(list.findByValue(i));
    }
    
    EXPECT_NO_THROW(list.clear());
    EXPECT_EQ(list.findByValue(50), false);
}

// Тест корректности структуры списка
TEST_F(DoublyLinkedListTest, ListStructure) {
    list.addToTail(1);
    list.addToTail(2);
    list.addToTail(3);
    list.addAfterValue(1, 99);
    EXPECT_TRUE(list.findByValue(99));
    
    list.addBeforeValue(3, 88);
    EXPECT_TRUE(list.findByValue(88));
    
    list.deleteByValue(2);
    EXPECT_FALSE(list.findByValue(2));
    
    EXPECT_TRUE(list.findByValue(1));
    EXPECT_TRUE(list.findByValue(99));
    EXPECT_TRUE(list.findByValue(88));
    EXPECT_TRUE(list.findByValue(3));
}

// Тест повторных операций
TEST_F(DoublyLinkedListTest, RepeatedOperations) {
    for (int i = 0; i < 10; i++) {
        list.addToTail(i);
    }
    
    for (int i = 0; i < 10; i++) {
        EXPECT_TRUE(list.findByValue(i));
        list.deleteByValue(i);
        EXPECT_FALSE(list.findByValue(i));
    }
    
    EXPECT_TRUE(list.findByValue(0) == false);
    EXPECT_TRUE(list.findByValue(9) == false);
}

// Тест файловых операций с пустым списком
TEST_F(DoublyLinkedListTest, EmptyListFileOperations) {
    const string txtFile = "test_doubly_empty.txt";
    const string binFile = "test_doubly_empty.bin";

    EXPECT_NO_THROW(list.saveToFile(txtFile));
    EXPECT_NO_THROW(list.saveToBinaryFile(binFile));
    
    DoublyLinkedList emptyList;
    EXPECT_NO_THROW(emptyList.loadFromFile(txtFile));
    EXPECT_TRUE(emptyList.findByValue(1) == false);
    
    EXPECT_NO_THROW(emptyList.loadFromBinaryFile(binFile));
    EXPECT_TRUE(emptyList.findByValue(1) == false);
    
    filesystem::remove(txtFile);
    filesystem::remove(binFile);
}

// Тест множественных файловых операций
TEST_F(DoublyLinkedListTest, MultipleFileOperations) {
    DoublyLinkedList list1, list2, list3;
    
    list1.addToTail(1);
    list1.addToTail(2);
    list1.addToTail(3);
    
    list2.addToTail(100);
    list2.addToTail(200);
    
    list3.addToTail(999);
    
    list1.saveToFile("list1.txt");
    list2.saveToFile("list2.txt");
    list3.saveToFile("list3.txt");
    
    DoublyLinkedList loaded1, loaded2, loaded3;
    loaded1.loadFromFile("list1.txt");
    loaded2.loadFromFile("list2.txt");
    loaded3.loadFromFile("list3.txt");
    
    EXPECT_TRUE(loaded1.findByValue(1));
    EXPECT_TRUE(loaded1.findByValue(2));
    EXPECT_TRUE(loaded1.findByValue(3));
    
    EXPECT_TRUE(loaded2.findByValue(100));
    EXPECT_TRUE(loaded2.findByValue(200));
    
    EXPECT_TRUE(loaded3.findByValue(999));
    
    filesystem::remove("list1.txt");
    filesystem::remove("list2.txt");
    filesystem::remove("list3.txt");
}