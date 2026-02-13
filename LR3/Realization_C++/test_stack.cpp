#include "Stack.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

class StackTest : public ::testing::Test {
protected:
    Stack stack;
    
    void SetUp() override {
        remove("test_stack.txt");
        remove("test_stack.bin");
    }
    
    void TearDown() override {
        remove("test_stack.txt");
        remove("test_stack.bin");
    }
};

// Тест базовых операций 
TEST_F(StackTest, PushPop) {
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
    
    stack.push(10);
    stack.push(20);
    
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.peek(), 20);
    
    EXPECT_EQ(stack.pop(), 20);
    EXPECT_EQ(stack.pop(), 10);
    EXPECT_TRUE(stack.isEmpty());
}

// Тест очистки стека
TEST_F(StackTest, Clear) {
    // Пустой стек
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
    
    stack.push(1);
    stack.push(2);
    stack.push(3);
    EXPECT_FALSE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 3);
    
    stack.clear();
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
    
    EXPECT_EQ(stack.pop(), -1);
    EXPECT_EQ(stack.peek(), -1);
}

// Тест метода print
TEST_F(StackTest, PrintFunction) {
    testing::internal::CaptureStdout();
    stack.print();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек пуст!"), string::npos);
    
    stack.push(5);
    stack.push(4);
    stack.push(3);
    
    testing::internal::CaptureStdout();
    stack.print();
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек (сверху вниз):"), string::npos);
    EXPECT_NE(output.find("3 4 5"), string::npos);
}

// Тест переполнения стека
TEST_F(StackTest, Overflow) {
    for (int i = 0; i < 100; i++) {
        stack.push(i);
    }
    EXPECT_EQ(stack.size(), 100);
    
    testing::internal::CaptureStdout();
    stack.push(101);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек переполнен!"), string::npos);
    EXPECT_EQ(stack.size(), 100);
    EXPECT_EQ(stack.peek(), 99);
}

// Тест множественных операций
TEST_F(StackTest, MultipleOperations) {
    for (int i = 0; i < 10; i++) {
        stack.push(i);
        EXPECT_EQ(stack.peek(), i);
        EXPECT_EQ(stack.size(), i + 1);
    }
    
    for (int i = 9; i >= 0; i--) {
        EXPECT_EQ(stack.pop(), i);
        EXPECT_EQ(stack.size(), i);
    }
    stack.push(999);
    EXPECT_EQ(stack.peek(), 999);
    EXPECT_EQ(stack.size(), 1);
}

// Тест сохранения/загрузки
TEST_F(StackTest, SaveLoadFile) {
    stack.push(100);
    stack.push(200);
    stack.push(300);
    
    const string filename = "test_stack.txt";
    
    testing::internal::CaptureStdout();
    stack.saveToFile(filename);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек сохранен в файл"), string::npos);
    
    Stack newStack;
    testing::internal::CaptureStdout();
    newStack.loadFromFile(filename);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек загружен из файла"), string::npos);
    
    EXPECT_EQ(newStack.size(), 3);
    EXPECT_EQ(newStack.peek(), 300);
    EXPECT_EQ(newStack.pop(), 300);
    EXPECT_EQ(newStack.pop(), 200);
    EXPECT_EQ(newStack.pop(), 100);
    
    Stack emptyStack;
    testing::internal::CaptureStdout();
    emptyStack.saveToFile("empty_stack.txt");
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек сохранен в файл"), string::npos);
    
    Stack loadedEmpty;
    loadedEmpty.loadFromFile("empty_stack.txt");
    EXPECT_TRUE(loadedEmpty.isEmpty());
    
    remove("empty_stack.txt");
}

// Тест пустого стека 
TEST_F(StackTest, EmptyStackOperations) {
    EXPECT_EQ(stack.pop(), -1);
    EXPECT_EQ(stack.peek(), -1);
    EXPECT_TRUE(stack.isEmpty());
    EXPECT_EQ(stack.size(), 0);
    
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(stack.pop(), -1);
    }
    
    testing::internal::CaptureStdout();
    stack.print();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек пуст!"), string::npos);
}

// Тест максимального размера стека
TEST_F(StackTest, MaxSizeStack) {
    for (int i = 0; i < 100; i++) {
        stack.push(i);
        EXPECT_EQ(stack.peek(), i);
        EXPECT_EQ(stack.size(), i + 1);
    }
    EXPECT_EQ(stack.size(), 100);
    
    for (int i = 0; i < 100; i++) {
    }
    
    for (int i = 99; i >= 0; i--) {
        EXPECT_EQ(stack.pop(), i);
        EXPECT_EQ(stack.size(), i);
    }
    EXPECT_TRUE(stack.isEmpty());
}

// Тест бинарного сохранения/загрузки
TEST_F(StackTest, BinarySaveLoad) {
    stack.push(555);
    stack.push(666);
    stack.push(777);
    
    const string filename = "test_stack.bin";
    
    testing::internal::CaptureStdout();
    stack.saveToBinaryFile(filename);
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек сохранен в бинарный файл"), string::npos);
    
    Stack newStack;
    testing::internal::CaptureStdout();
    newStack.loadFromBinaryFile(filename);
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Стек загружен из бинарного файла"), string::npos);
    
    EXPECT_EQ(newStack.size(), 3);
    EXPECT_EQ(newStack.peek(), 777);
    EXPECT_EQ(newStack.pop(), 777);
    EXPECT_EQ(newStack.pop(), 666);
    EXPECT_EQ(newStack.pop(), 555);
    
    Stack emptyStack;
    emptyStack.saveToBinaryFile("empty_stack.bin");
    
    Stack loadedEmpty;
    loadedEmpty.loadFromBinaryFile("empty_stack.bin");
    EXPECT_TRUE(loadedEmpty.isEmpty());
    
    remove("empty_stack.bin");
}

// Тест обработки ошибок файловых операций
TEST_F(StackTest, FileOperationErrors) {
    testing::internal::CaptureStdout();
    stack.loadFromFile("nonexistent_file.txt");
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Ошибка открытия файла"), string::npos);
    
    testing::internal::CaptureStdout();
    stack.loadFromBinaryFile("nonexistent_file.bin");
    output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Ошибка открытия бинарного файла"), string::npos);
}

// Тест производительности
TEST_F(StackTest, DISABLED_Performance) {
    const int ITERATIONS = 10000;
    
    for (int i = 0; i < ITERATIONS; i++) {
        stack.push(i);
    }
    
    for (int i = ITERATIONS - 1; i >= 0; i--) {
        EXPECT_EQ(stack.pop(), i);
    }
}