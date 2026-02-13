#include "FullBinaryTree.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace std;

class FullBinaryTreeTest : public ::testing::Test {
protected:
    FullBinaryTree tree;
    
    bool searchHelper(int value) {
        testing::internal::CaptureStdout();
        bool result = tree.search(value);
        testing::internal::GetCapturedStdout();
        return result;
    }
};

TEST_F(FullBinaryTreeTest, BasicOperations) {
    tree.add(10);
    tree.add(5);
    tree.add(15);
    
    EXPECT_EQ(tree.getNodeCount(), 3);
    EXPECT_TRUE(searchHelper(10));
    EXPECT_TRUE(searchHelper(5));
    EXPECT_TRUE(searchHelper(15));
    EXPECT_FALSE(searchHelper(99));
}

TEST_F(FullBinaryTreeTest, EmptyTree) {
    EXPECT_EQ(tree.getNodeCount(), 0);
    EXPECT_EQ(tree.getTreeHeight(), 0);
    EXPECT_TRUE(tree.isFull());
}

TEST_F(FullBinaryTreeTest, TreeTraversals) {
    tree.add(1);
    tree.add(2);
    tree.add(3);
    tree.add(4);
    tree.add(5);
    
    EXPECT_EQ(tree.getNodeCount(), 5);
    
    EXPECT_NO_THROW(tree.printInOrder());
    EXPECT_NO_THROW(tree.printPreOrder());
    EXPECT_NO_THROW(tree.printPostOrder());
    EXPECT_NO_THROW(tree.printLevelOrder());
}

TEST_F(FullBinaryTreeTest, TreeHeight) {
    EXPECT_EQ(tree.getTreeHeight(), 0);
    
    tree.add(10);
    EXPECT_EQ(tree.getTreeHeight(), 1);
    
    tree.add(5);
    tree.add(15);
    EXPECT_EQ(tree.getTreeHeight(), 2);
    
    tree.add(3);
    tree.add(7);
    EXPECT_EQ(tree.getTreeHeight(), 3);
}

TEST_F(FullBinaryTreeTest, ClearTree) {
    tree.add(10);
    tree.add(20);
    tree.add(30);
    
    EXPECT_EQ(tree.getNodeCount(), 3);
    
    tree.clear();
    EXPECT_EQ(tree.getNodeCount(), 0);
    EXPECT_EQ(tree.getTreeHeight(), 0);
}

TEST_F(FullBinaryTreeTest, FileOperations) {
    tree.add(25);
    tree.add(75);
    tree.add(125);
    
    const std::string txtFile = "test_tree.txt";
    tree.saveToFile(txtFile);
    
    FullBinaryTree tree2;
    tree2.loadFromFile(txtFile);
    
    EXPECT_EQ(tree2.getNodeCount(), 3);
    EXPECT_TRUE(searchHelper(25));
    EXPECT_TRUE(searchHelper(75));
    EXPECT_TRUE(searchHelper(125));
    
    std::remove(txtFile.c_str());
    
    tree.clear();
    tree.add(55);
    tree.add(66);
    tree.add(77);
    
    const std::string binFile = "test_tree.bin";
    tree.saveToBinaryFile(binFile);
    
    FullBinaryTree tree3;
    tree3.loadFromBinaryFile(binFile);
    
    EXPECT_EQ(tree3.getNodeCount(), 3);
    EXPECT_TRUE(searchHelper(55));
    EXPECT_TRUE(searchHelper(66));
    EXPECT_TRUE(searchHelper(77));
    
    std::remove(binFile.c_str());
}

TEST_F(FullBinaryTreeTest, IsFullTree) {
    EXPECT_TRUE(tree.isFull());
    
    tree.add(1);
    EXPECT_TRUE(tree.isFull());
    
    tree.add(2);
    tree.add(3);
    EXPECT_TRUE(tree.isFull());
    
    tree.add(4);
    EXPECT_FALSE(tree.isFull());
}