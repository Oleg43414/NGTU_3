#include "SinglyLinkedList.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

class SinglyLinkedListTest : public ::testing::Test {
protected:
    SinglyLinkedList* list;
    
    void SetUp() override {
        list = new SinglyLinkedList();
        remove("test_singly.txt");
        remove("test_singly.bin");
        remove("empty_singly.txt");
        remove("empty_singly.bin");
        remove("bad_format.txt");
    }
    
    void TearDown() override {
        delete list;
        remove("test_singly.txt");
        remove("test_singly.bin");
        remove("empty_singly.txt");
        remove("empty_singly.bin");
        remove("bad_format.txt");
    }
};

TEST_F(SinglyLinkedListTest, DeleteByValue) {
    EXPECT_NO_THROW(list->deleteByValue(99));
    
    list->addToTail(42);
    list->addToTail(24);
    list->addToTail(99);
    list->addToTail(100);
    
    EXPECT_TRUE(list->findByValue(42));
    EXPECT_TRUE(list->findByValue(24));
    EXPECT_TRUE(list->findByValue(99));
    EXPECT_TRUE(list->findByValue(100));
    EXPECT_EQ(list->findByValue(42), true);
    
    list->deleteByValue(42);
    EXPECT_FALSE(list->findByValue(42));
    EXPECT_TRUE(list->findByValue(24));
    EXPECT_TRUE(list->findByValue(99));
    EXPECT_TRUE(list->findByValue(100));
    
    list->deleteByValue(99);
    EXPECT_FALSE(list->findByValue(99));
    EXPECT_TRUE(list->findByValue(24));
    EXPECT_TRUE(list->findByValue(100));
    
    // 6. Удаление хвоста
    list->deleteByValue(100);
    EXPECT_FALSE(list->findByValue(100));
    EXPECT_TRUE(list->findByValue(24));
    
    list->deleteByValue(24);
    EXPECT_FALSE(list->findByValue(24));
    
    EXPECT_NO_THROW(list->deleteByValue(999));
}

TEST_F(SinglyLinkedListTest, AddBefore) {
    // 1. Добавление в пустой список - не должно падать
    EXPECT_NO_THROW(list->addBefore(1, 99));
    
    list->addToTail(10);
    list->addBefore(10, 5);
    
    EXPECT_TRUE(list->findByValue(5));
    EXPECT_TRUE(list->findByValue(10));
    
    list->addToTail(20);
    list->addToTail(30);
    list->addBefore(20, 15);
    
    EXPECT_TRUE(list->findByValue(15));
    EXPECT_TRUE(list->findByValue(20));
    EXPECT_TRUE(list->findByValue(30));
    
    list->addBefore(30, 25);
    EXPECT_TRUE(list->findByValue(25));
    
    EXPECT_NO_THROW(list->addBefore(999, 88));
    EXPECT_FALSE(list->findByValue(88));
}

TEST_F(SinglyLinkedListTest, FileOperationErrors) {
    EXPECT_NO_THROW(list->loadFromFile("nonexistent_file.txt"));
    EXPECT_NO_THROW(list->loadFromBinaryFile("nonexistent_file.bin"));
    
    {
        ofstream badFile("bad_format.txt");
        badFile << "abc def ghi" << endl;
        badFile << "not numbers" << endl;
        badFile.close();
    }
    
    EXPECT_NO_THROW(list->loadFromFile("bad_format.txt"));
    
    EXPECT_FALSE(list->findByValue(1));
    
    SinglyLinkedList emptyList;
    EXPECT_NO_THROW(emptyList.saveToFile("empty.txt"));
    EXPECT_NO_THROW(emptyList.loadFromFile("empty.txt"));
    EXPECT_NO_THROW(emptyList.saveToBinaryFile("empty.bin"));
    EXPECT_NO_THROW(emptyList.loadFromBinaryFile("empty.bin"));
}


TEST_F(SinglyLinkedListTest, AddToHead) {
    list->addToHead(10);
    EXPECT_TRUE(list->findByValue(10));
    
    list->addToHead(20);
    EXPECT_TRUE(list->findByValue(20));
    EXPECT_TRUE(list->findByValue(10));
}

TEST_F(SinglyLinkedListTest, AddToTail) {
    list->addToTail(20);
    EXPECT_TRUE(list->findByValue(20));
    
    list->addToTail(30);
    list->addToTail(40);
    EXPECT_TRUE(list->findByValue(20));
    EXPECT_TRUE(list->findByValue(30));
    EXPECT_TRUE(list->findByValue(40));
}

TEST_F(SinglyLinkedListTest, AddAfter) {
    EXPECT_NO_THROW(list->addAfter(1, 99));
    
    list->addToTail(1);
    list->addToTail(2);
    list->addToTail(3);
    
    list->addAfter(1, 99);
    EXPECT_TRUE(list->findByValue(99));
    
    EXPECT_NO_THROW(list->addAfter(999, 88));
    
    list->addAfter(3, 77);
    EXPECT_TRUE(list->findByValue(77));
}

TEST_F(SinglyLinkedListTest, DeleteFromHead) {
    EXPECT_NO_THROW(list->deleteFromHead());
    
    list->addToTail(1);
    list->deleteFromHead();
    EXPECT_FALSE(list->findByValue(1));
    
    list->addToTail(2);
    list->addToTail(3);
    list->deleteFromHead();
    EXPECT_FALSE(list->findByValue(2));
    EXPECT_TRUE(list->findByValue(3));
}

TEST_F(SinglyLinkedListTest, DeleteFromTail) {
    EXPECT_NO_THROW(list->deleteFromTail());
    
    list->addToTail(100);
    list->deleteFromTail();
    EXPECT_FALSE(list->findByValue(100));
    
    list->addToTail(200);
    list->addToTail(300);
    list->deleteFromTail();
    EXPECT_FALSE(list->findByValue(300));
    EXPECT_TRUE(list->findByValue(200));
}

TEST_F(SinglyLinkedListTest, FindByValue) {
    EXPECT_FALSE(list->findByValue(999));
    
    list->addToTail(123);
    list->addToTail(456);
    list->addToTail(789);
    
    EXPECT_TRUE(list->findByValue(123));
    EXPECT_TRUE(list->findByValue(456));
    EXPECT_TRUE(list->findByValue(789));
    EXPECT_FALSE(list->findByValue(999));
}

TEST_F(SinglyLinkedListTest, ClearList) {
    EXPECT_NO_THROW(list->clear());
    
    list->addToTail(1);
    list->addToTail(2);
    list->addToTail(3);
    
    list->clear();
    EXPECT_FALSE(list->findByValue(1));
    EXPECT_FALSE(list->findByValue(2));
    EXPECT_FALSE(list->findByValue(3));
}

TEST_F(SinglyLinkedListTest, SaveLoadTextFile) {
    const string filename = "test_singly.txt";
    
    list->addToTail(111);
    list->addToTail(222);
    list->addToTail(333);
    list->saveToFile(filename);
    
    SinglyLinkedList newList;
    newList.loadFromFile(filename);
    
    EXPECT_TRUE(newList.findByValue(111));
    EXPECT_TRUE(newList.findByValue(222));
    EXPECT_TRUE(newList.findByValue(333));
    
    SinglyLinkedList emptyList;
    emptyList.saveToFile("empty_singly.txt");
    SinglyLinkedList loadedEmpty;
    loadedEmpty.loadFromFile("empty_singly.txt");
    EXPECT_FALSE(loadedEmpty.findByValue(1));
}

TEST_F(SinglyLinkedListTest, SaveLoadBinaryFile) {
    const string filename = "test_singly.bin";
    
    list->addToTail(555);
    list->addToTail(666);
    list->addToTail(777);
    list->saveToBinaryFile(filename);
    
    SinglyLinkedList newList;
    newList.loadFromBinaryFile(filename);
    
    EXPECT_TRUE(newList.findByValue(555));
    EXPECT_TRUE(newList.findByValue(666));
    EXPECT_TRUE(newList.findByValue(777));
    
    SinglyLinkedList emptyList;
    emptyList.saveToBinaryFile("empty_singly.bin");
    SinglyLinkedList loadedEmpty;
    loadedEmpty.loadFromBinaryFile("empty_singly.bin");
    EXPECT_FALSE(loadedEmpty.findByValue(1));
}

TEST_F(SinglyLinkedListTest, DeleteAfter) {
    EXPECT_NO_THROW(list->deleteAfter(20));
    
    list->addToTail(10);
    list->addToTail(20);
    list->addToTail(30);
    
    list->deleteAfter(20);
    EXPECT_FALSE(list->findByValue(30));
    EXPECT_TRUE(list->findByValue(10));
    EXPECT_TRUE(list->findByValue(20));
    
    EXPECT_NO_THROW(list->deleteAfter(999));
    
    EXPECT_NO_THROW(list->deleteAfter(20));
}

TEST_F(SinglyLinkedListTest, DeleteBefore) {
    EXPECT_NO_THROW(list->deleteBefore(30));
    
    list->addToTail(10);
    list->addToTail(20);
    list->addToTail(30);
    
    list->deleteBefore(30);
    EXPECT_FALSE(list->findByValue(20));
    EXPECT_TRUE(list->findByValue(10));
    EXPECT_TRUE(list->findByValue(30));
    
    EXPECT_NO_THROW(list->deleteBefore(10));
    
    EXPECT_NO_THROW(list->deleteBefore(999));
    
    list->clear();
    list->addToTail(100);
    list->addToTail(200);
    list->deleteBefore(200);
    EXPECT_FALSE(list->findByValue(100));
    EXPECT_TRUE(list->findByValue(200));
}

TEST_F(SinglyLinkedListTest, EdgeCases) {
    // Список с одним элементом
    list->addToHead(42);
    EXPECT_TRUE(list->findByValue(42));
    list->deleteFromHead();
    EXPECT_FALSE(list->findByValue(42));
    
    list->addToTail(42);
    list->deleteFromTail();
    EXPECT_FALSE(list->findByValue(42));
}

TEST_F(SinglyLinkedListTest, LargeList) {
    const int COUNT = 1000;
    
    for (int i = 0; i < COUNT; i++) {
        list->addToTail(i);
    }
    
    EXPECT_TRUE(list->findByValue(0));
    EXPECT_TRUE(list->findByValue(COUNT/2));
    EXPECT_TRUE(list->findByValue(COUNT-1));
    
    for (int i = 0; i < COUNT; i++) {
        list->deleteByValue(i);
    }
    
    EXPECT_FALSE(list->findByValue(0));
    EXPECT_FALSE(list->findByValue(COUNT-1));
}

TEST_F(SinglyLinkedListTest, PrintFunctions) {
    EXPECT_NO_THROW(list->print());
    EXPECT_NO_THROW(list->printReverse());
    
    list->addToTail(1);
    list->addToTail(2);
    list->addToTail(3);
    
    EXPECT_NO_THROW(list->print());
    EXPECT_NO_THROW(list->printReverse());
}