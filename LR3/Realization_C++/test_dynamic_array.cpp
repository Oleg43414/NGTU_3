#include "DynamicArray.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

class DynamicArrayTest : public ::testing::Test {
protected:
    DynamicArray* array;
    
    void SetUp() override {
        array = new DynamicArray(5);
    }
    
    void TearDown() override {
        delete array;
    }
};

TEST_F(DynamicArrayTest, Constructor) {
    EXPECT_EQ(array->getSize(), 0);
    EXPECT_EQ(array->getCapacity(), 5);
}

TEST_F(DynamicArrayTest, PushBack) {
    array->pushBack("Hello");
    array->pushBack("World");
    
    EXPECT_EQ(array->getSize(), 2);
    EXPECT_EQ(array->get(0), "Hello");
    EXPECT_EQ(array->get(1), "World");
}

TEST_F(DynamicArrayTest, Insert) {
    array->pushBack("A");
    array->pushBack("C");
    
    testing::internal::CaptureStdout();
    array->insert(1, "B");
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(array->getSize(), 3);
    EXPECT_EQ(array->get(0), "A");
    EXPECT_EQ(array->get(1), "B");
    EXPECT_EQ(array->get(2), "C");
}

TEST_F(DynamicArrayTest, InsertInvalidIndex) {
    testing::internal::CaptureStdout();
    array->insert(5, "Invalid");
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Неверный индекс"), string::npos);
}

TEST_F(DynamicArrayTest, Remove) {
    array->pushBack("A");
    array->pushBack("B");
    array->pushBack("C");
    
    testing::internal::CaptureStdout();
    array->remove(1);
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(array->getSize(), 2);
    EXPECT_EQ(array->get(0), "A");
    EXPECT_EQ(array->get(1), "C");
}

TEST_F(DynamicArrayTest, RemoveInvalidIndex) {
    testing::internal::CaptureStdout();
    array->remove(0);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Неверный индекс"), string::npos);
}

TEST_F(DynamicArrayTest, GetSet) {
    array->pushBack("Initial");
    
    testing::internal::CaptureStdout();
    array->set(0, "Changed");
    string setOutput = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(array->get(0), "Changed");
    
    testing::internal::CaptureStdout();
    string value = array->get(10);
    string getOutput = testing::internal::GetCapturedStdout();
    EXPECT_NE(getOutput.find("Неверный индекс"), string::npos);
    EXPECT_EQ(value, "");
}

TEST_F(DynamicArrayTest, Resize) {
    for (int i = 0; i < 10; i++) {
        array->pushBack("Element" + to_string(i));
    }
    
    EXPECT_GE(array->getCapacity(), 10);
    EXPECT_EQ(array->getSize(), 10);
    
    for (int i = 0; i < 5; i++) {
        array->remove(0);
    }
    
    EXPECT_EQ(array->getSize(), 5);
}

TEST_F(DynamicArrayTest, Clear) {
    array->pushBack("One");
    array->pushBack("Two");
    array->pushBack("Three");
    
    array->clear();
    EXPECT_EQ(array->getSize(), 0);
    EXPECT_EQ(array->getCapacity(), 5);
}

TEST_F(DynamicArrayTest, Print) {
    array->pushBack("Test1");
    array->pushBack("Test2");
    
    testing::internal::CaptureStdout();
    array->print();
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("Test1"), string::npos);
    EXPECT_NE(output.find("Test2"), string::npos);
    EXPECT_NE(output.find("Массив["), string::npos);
}

TEST_F(DynamicArrayTest, SaveLoadTextFile) {
    array->pushBack("Hello");
    array->pushBack("World");
    array->pushBack("C++");
    
    const string filename = "test_array.txt";
    array->saveToFile(filename);
    
    DynamicArray newArray;
    newArray.loadFromFile(filename);
    
    EXPECT_EQ(newArray.getSize(), 3);
    EXPECT_EQ(newArray.get(0), "Hello");
    EXPECT_EQ(newArray.get(1), "World");
    EXPECT_EQ(newArray.get(2), "C++");
    
    remove(filename.c_str());
}

TEST_F(DynamicArrayTest, SaveLoadBinaryFile) {
    array->pushBack("Binary");
    array->pushBack("File");
    array->pushBack("Test");
    
    const string filename = "test_array.bin";
    array->saveToBinaryFile(filename);
    
    DynamicArray newArray;
    newArray.loadFromBinaryFile(filename);
    
    EXPECT_EQ(newArray.getSize(), 3);
    EXPECT_EQ(newArray.get(0), "Binary");
    EXPECT_EQ(newArray.get(1), "File");
    EXPECT_EQ(newArray.get(2), "Test");
    
    remove(filename.c_str());
}

TEST_F(DynamicArrayTest, EdgeCases) {
    EXPECT_EQ(array->getSize(), 0);
    
    testing::internal::CaptureStdout();
    string emptyValue = array->get(0);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Неверный индекс"), string::npos);
    
    testing::internal::CaptureStdout();
    array->set(0, "Test");
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Неверный индекс"), string::npos);
}