package main

import (
	"os"
	"testing"
)

func TestFullBinaryTree_BasicOperations(t *testing.T) {
	tree := NewFullBinaryTree()

	tree.Add(10)
	tree.Add(5)
	tree.Add(15)

	if tree.GetNodeCount() != 3 {
		t.Errorf("Ожидалось 3 узла, получено %d", tree.GetNodeCount())
	}

	if !tree.Search(10) {
		t.Errorf("Элемент 10 должен существовать")
	}
	if !tree.Search(5) {
		t.Errorf("Элемент 5 должен существовать")
	}
	if !tree.Search(15) {
		t.Errorf("Элемент 15 должен существовать")
	}
	if tree.Search(99) {
		t.Errorf("Элемент 99 не должен существовать")
	}
}

func TestFullBinaryTree_EmptyTree(t *testing.T) {
	tree := NewFullBinaryTree()

	if tree.GetNodeCount() != 0 {
		t.Errorf("Ожидалось 0 узлов, получено %d", tree.GetNodeCount())
	}
	if tree.GetTreeHeight() != 0 {
		t.Errorf("Ожидалась высота 0, получена %d", tree.GetTreeHeight())
	}
	if !tree.IsFull() {
		t.Errorf("Пустое дерево должно считаться полным")
	}
}

func TestFullBinaryTree_TreeHeight(t *testing.T) {
	tree := NewFullBinaryTree()

	if tree.GetTreeHeight() != 0 {
		t.Errorf("Ожидалась высота 0, получена %d", tree.GetTreeHeight())
	}

	tree.Add(10)
	if tree.GetTreeHeight() != 1 {
		t.Errorf("Ожидалась высота 1, получена %d", tree.GetTreeHeight())
	}

	tree.Add(5)
	tree.Add(15)
	if tree.GetTreeHeight() != 2 {
		t.Errorf("Ожидалась высота 2, получена %d", tree.GetTreeHeight())
	}

	tree.Add(3)
	tree.Add(7)
	if tree.GetTreeHeight() != 3 {
		t.Errorf("Ожидалась высота 3, получена %d", tree.GetTreeHeight())
	}
}

func TestFullBinaryTree_ClearTree(t *testing.T) {
	tree := NewFullBinaryTree()

	tree.Add(10)
	tree.Add(20)
	tree.Add(30)

	if tree.GetNodeCount() != 3 {
		t.Errorf("Ожидалось 3 узла, получено %d", tree.GetNodeCount())
	}

	tree.Clear()

	if tree.GetNodeCount() != 0 {
		t.Errorf("После очистки ожидалось 0 узлов, получено %d", tree.GetNodeCount())
	}
	if tree.GetTreeHeight() != 0 {
		t.Errorf("После очистки ожидалась высота 0, получена %d", tree.GetTreeHeight())
	}
}

func TestFullBinaryTree_FileOperations(t *testing.T) {
	tree := NewFullBinaryTree()

	tree.Add(25)
	tree.Add(75)
	tree.Add(125)

	// Текстовый файл
	txtFile := "test_tree.txt"
	err := tree.SaveToFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	tree2 := NewFullBinaryTree()
	err = tree2.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if tree2.GetNodeCount() != 3 {
		t.Errorf("Ожидалось 3 узла, получено %d", tree2.GetNodeCount())
	}
	if !tree2.Search(25) {
		t.Errorf("Элемент 25 должен существовать")
	}
	if !tree2.Search(75) {
		t.Errorf("Элемент 75 должен существовать")
	}
	if !tree2.Search(125) {
		t.Errorf("Элемент 125 должен существовать")
	}

	os.Remove(txtFile)

	// Бинарный файл
	tree.Clear()
	tree.Add(55)
	tree.Add(66)
	tree.Add(77)

	binFile := "test_tree.bin"
	err = tree.SaveToBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	tree3 := NewFullBinaryTree()
	err = tree3.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if tree3.GetNodeCount() != 3 {
		t.Errorf("Ожидалось 3 узла, получено %d", tree3.GetNodeCount())
	}
	if !tree3.Search(55) {
		t.Errorf("Элемент 55 должен существовать")
	}
	if !tree3.Search(66) {
		t.Errorf("Элемент 66 должен существовать")
	}
	if !tree3.Search(77) {
		t.Errorf("Элемент 77 должен существовать")
	}

	os.Remove(binFile)
}

func TestFullBinaryTree_IsFullTree(t *testing.T) {
	tree := NewFullBinaryTree()

	// Пустое дерево
	if !tree.IsFull() {
		t.Errorf("Пустое дерево должно быть полным")
	}

	// Один элемент
	tree.Add(1)
	if !tree.IsFull() {
		t.Errorf("Дерево с одним узлом должно быть полным")
	}

	// Полное дерево высотой 2 (3 узла)
	tree.Add(2)
	tree.Add(3)
	if !tree.IsFull() {
		t.Errorf("Дерево с 3 узлами должно быть полным")
	}

	// Добавляем 4-й элемент - дерево перестает быть полным
	tree.Add(4)
	if tree.IsFull() {
		t.Errorf("Дерево с 4 узлами не должно быть полным")
	}
}

func TestFullBinaryTree_Traversals(t *testing.T) {
	tree := NewFullBinaryTree()

	tree.Add(1)
	tree.Add(2)
	tree.Add(3)
	tree.Add(4)
	tree.Add(5)

	if tree.GetNodeCount() != 5 {
		t.Errorf("Ожидалось 5 узлов, получено %d", tree.GetNodeCount())
	}

	// Проверяем что обходы не падают
	tree.PrintInOrder()
	tree.PrintPreOrder()
	tree.PrintPostOrder()
	tree.PrintLevelOrder()
}

func TestFullBinaryTree_EmptyTreeFile(t *testing.T) {
	tree := NewFullBinaryTree()

	// Сохранение пустого дерева
	txtFile := "empty_tree.txt"
	err := tree.SaveToFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустого дерева: %v", err)
	}

	binFile := "empty_tree.bin"
	err = tree.SaveToBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустого дерева в бинарный файл: %v", err)
	}

	// Загрузка пустого дерева
	emptyTree := NewFullBinaryTree()
	err = emptyTree.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустого дерева: %v", err)
	}
	if emptyTree.GetNodeCount() != 0 {
		t.Errorf("Загруженное дерево должно быть пустым")
	}

	err = emptyTree.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустого дерева из бинарного файла: %v", err)
	}
	if emptyTree.GetNodeCount() != 0 {
		t.Errorf("Загруженное дерево должно быть пустым")
	}

	os.Remove(txtFile)
	os.Remove(binFile)
}
