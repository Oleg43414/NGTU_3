#include "Queue.h"
#include <gtest/gtest.h>
#include <fstream>

using namespace std;

class QueueTest : public ::testing::Test {
protected:
    Queue* queue;
    
    void SetUp() override {
        queue = new Queue();
    }
    
    void TearDown() override {
        delete queue;
    }
};

TEST_F(QueueTest, EmptyQueue) {
    EXPECT_TRUE(queue->isEmpty());
    EXPECT_EQ(queue->size(), 0);
}

TEST_F(QueueTest, PushPopOperations) {
    queue->push(10);
    queue->push(20);
    queue->push(30);
    
    EXPECT_EQ(queue->size(), 3);
    EXPECT_FALSE(queue->isEmpty());
    
    EXPECT_EQ(queue->front(), 10);
    queue->pop();
    EXPECT_EQ(queue->front(), 20);
    queue->pop();
    EXPECT_EQ(queue->front(), 30);
}

TEST_F(QueueTest, CopyConstructor) {
    queue->push(1);
    queue->push(2);
    queue->push(3);
    
    Queue copyQueue(*queue);
    EXPECT_EQ(copyQueue.size(), 3);
    EXPECT_EQ(copyQueue.front(), 1);
}

TEST_F(QueueTest, AssignmentOperator) {
    queue->push(100);
    queue->push(200);
    
    Queue anotherQueue;
    anotherQueue = *queue;
    
    EXPECT_EQ(anotherQueue.size(), 2);
    EXPECT_EQ(anotherQueue.front(), 100);
}

TEST_F(QueueTest, ClearQueue) {
    queue->push(5);
    queue->push(15);
    
    queue->clear();
    EXPECT_TRUE(queue->isEmpty());
    EXPECT_EQ(queue->size(), 0);
}

TEST_F(QueueTest, DisplayQueue) {
    queue->push(42);
    queue->push(84);
    
    testing::internal::CaptureStdout();
    queue->display();
    string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("42"), string::npos);
    EXPECT_NE(output.find("84"), string::npos);
}

TEST_F(QueueTest, SaveLoadTextFile) {
    queue->push(111);
    queue->push(222);
    queue->push(333);
    
    const string filename = "test_queue.txt";
    queue->saveToFile(filename);
    
    Queue newQueue;
    newQueue.loadFromFile(filename);
    
    EXPECT_EQ(newQueue.size(), 3);
    EXPECT_EQ(newQueue.front(), 111);
    newQueue.pop();
    EXPECT_EQ(newQueue.front(), 222);
    
    remove(filename.c_str());
}

TEST_F(QueueTest, SaveLoadBinaryFile) {
    queue->push(999);
    queue->push(888);
    
    const string filename = "test_queue.bin";
    queue->saveToBinaryFile(filename);
    
    Queue newQueue;
    newQueue.loadFromBinaryFile(filename);
    
    EXPECT_EQ(newQueue.size(), 2);
    EXPECT_EQ(newQueue.front(), 999);
    
    remove(filename.c_str());
}