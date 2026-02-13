package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"math"
	"os"
	"strconv"
)

// TreeNode - узел бинарного дерева
type TreeNode struct {
	Data  int
	Left  *TreeNode
	Right *TreeNode
}

// NewTreeNode - конструктор узла
func NewTreeNode(value int) *TreeNode {
	return &TreeNode{
		Data:  value,
		Left:  nil,
		Right: nil,
	}
}

// FullBinaryTree - полное бинарное дерево
type FullBinaryTree struct {
	Root *TreeNode
}

// NewFullBinaryTree - конструктор дерева
func NewFullBinaryTree() *FullBinaryTree {
	return &FullBinaryTree{
		Root: nil,
	}
}

// clearRecursive - рекурсивная очистка
func (f *FullBinaryTree) clearRecursive(current *TreeNode) {
	if current == nil {
		return
	}
	f.clearRecursive(current.Left)
	f.clearRecursive(current.Right)
	current.Left = nil
	current.Right = nil
}

// searchRecursive - рекурсивный поиск
func (f *FullBinaryTree) searchRecursive(current *TreeNode, value int) bool {
	if current == nil {
		return false
	}
	if current.Data == value {
		return true
	}
	return f.searchRecursive(current.Left, value) || f.searchRecursive(current.Right, value)
}

// countNodes - подсчет узлов
func (f *FullBinaryTree) countNodes(current *TreeNode) int {
	if current == nil {
		return 0
	}
	return 1 + f.countNodes(current.Left) + f.countNodes(current.Right)
}

// getHeight - получение высоты
func (f *FullBinaryTree) getHeight(current *TreeNode) int {
	if current == nil {
		return 0
	}
	leftHeight := f.getHeight(current.Left)
	rightHeight := f.getHeight(current.Right)
	if leftHeight > rightHeight {
		return 1 + leftHeight
	}
	return 1 + rightHeight
}

// printInOrderRecursive - рекурсивный InOrder обход
func (f *FullBinaryTree) printInOrderRecursive(current *TreeNode) {
	if current == nil {
		return
	}
	f.printInOrderRecursive(current.Left)
	fmt.Printf("%d ", current.Data)
	f.printInOrderRecursive(current.Right)
}

// printPreOrderRecursive - рекурсивный PreOrder обход
func (f *FullBinaryTree) printPreOrderRecursive(current *TreeNode) {
	if current == nil {
		return
	}
	fmt.Printf("%d ", current.Data)
	f.printPreOrderRecursive(current.Left)
	f.printPreOrderRecursive(current.Right)
}

// printPostOrderRecursive - рекурсивный PostOrder обход
func (f *FullBinaryTree) printPostOrderRecursive(current *TreeNode) {
	if current == nil {
		return
	}
	f.printPostOrderRecursive(current.Left)
	f.printPostOrderRecursive(current.Right)
	fmt.Printf("%d ", current.Data)
}

// Add - добавление элемента
func (f *FullBinaryTree) Add(value int) {
	if f.Root == nil {
		f.Root = NewTreeNode(value)
		fmt.Printf("Добавлен корень: %d\n", value)
		return
	}

	queue := []*TreeNode{f.Root}

	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		if current.Left == nil {
			current.Left = NewTreeNode(value)
			fmt.Printf("Добавлен элемент: %d\n", value)
			return
		} else {
			queue = append(queue, current.Left)
		}

		if current.Right == nil {
			current.Right = NewTreeNode(value)
			fmt.Printf("Добавлен элемент: %d\n", value)
			return
		} else {
			queue = append(queue, current.Right)
		}
	}
}

// Search - поиск элемента
func (f *FullBinaryTree) Search(value int) bool {
	found := f.searchRecursive(f.Root, value)
	if found {
		fmt.Printf("Элемент %d найден!\n", value)
	} else {
		fmt.Printf("Элемент %d не найден!\n", value)
	}
	return found
}

// IsFull - проверка на полное бинарное дерево
func (f *FullBinaryTree) IsFull() bool {
	if f.Root == nil {
		fmt.Println("Дерево пусто!")
		return true
	}

	nodeCount := f.countNodes(f.Root)
	height := f.getHeight(f.Root)
	expectedNodes := int(math.Pow(2, float64(height))) - 1

	if nodeCount == expectedNodes {
		fmt.Println("Дерево является полным бинарным деревом!")
		return true
	} else {
		fmt.Printf("Дерево НЕ является полным бинарным деревом.\n")
		fmt.Printf("Узлов: %d, Ожидается: %d\n", nodeCount, expectedNodes)
		return false
	}
}

// PrintInOrder - InOrder обход
func (f *FullBinaryTree) PrintInOrder() {
	if f.Root == nil {
		fmt.Println("Дерево пусто!")
		return
	}
	fmt.Print("In-Order обход: ")
	f.printInOrderRecursive(f.Root)
	fmt.Println()
}

// PrintPreOrder - PreOrder обход
func (f *FullBinaryTree) PrintPreOrder() {
	if f.Root == nil {
		fmt.Println("Дерево пусто!")
		return
	}
	fmt.Print("Pre-Order обход: ")
	f.printPreOrderRecursive(f.Root)
	fmt.Println()
}

// PrintPostOrder - PostOrder обход
func (f *FullBinaryTree) PrintPostOrder() {
	if f.Root == nil {
		fmt.Println("Дерево пусто!")
		return
	}
	fmt.Print("Post-Order обход: ")
	f.printPostOrderRecursive(f.Root)
	fmt.Println()
}

// PrintLevelOrder - LevelOrder обход
func (f *FullBinaryTree) PrintLevelOrder() {
	if f.Root == nil {
		fmt.Println("Дерево пусто!")
		return
	}

	fmt.Print("Level-Order обход: ")
	queue := []*TreeNode{f.Root}

	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]
		fmt.Printf("%d ", current.Data)

		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}
	fmt.Println()
}

// GetNodeCount - получение количества узлов
func (f *FullBinaryTree) GetNodeCount() int {
	return f.countNodes(f.Root)
}

// GetTreeHeight - получение высоты дерева
func (f *FullBinaryTree) GetTreeHeight() int {
	return f.getHeight(f.Root)
}

// Clear - очистка дерева
func (f *FullBinaryTree) Clear() {
	f.clearRecursive(f.Root)
	f.Root = nil
	fmt.Println("Дерево очищено!")
}

// SaveToFile - сохранение в текстовый файл
func (f *FullBinaryTree) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	if f.Root == nil {
		fmt.Println("Дерево пусто, файл пуст.")
		return nil
	}

	queue := []*TreeNode{f.Root}
	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		_, err := fmt.Fprintf(file, "%d ", current.Data)
		if err != nil {
			return err
		}

		if current.Left != nil {
			queue = append(queue, current.Left)
		}
		if current.Right != nil {
			queue = append(queue, current.Right)
		}
	}

	fmt.Printf("Дерево сохранено в файл: %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (f *FullBinaryTree) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	f.Clear()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	for scanner.Scan() {
		value, err := strconv.Atoi(scanner.Text())
		if err == nil {
			f.Add(value)
		}
	}

	fmt.Printf("Дерево загружено из файла: %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл
func (f *FullBinaryTree) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	if f.Root == nil {
		empty := int32(0)
		err := binary.Write(file, binary.LittleEndian, empty)
		if err != nil {
			return err
		}
		fmt.Printf("Пустое дерево сохранено в бинарный файл: %s\n", filename)
		return nil
	}

	nodeCount := int32(f.GetNodeCount())
	err = binary.Write(file, binary.LittleEndian, nodeCount)
	if err != nil {
		return err
	}

	queue := []*TreeNode{f.Root}
	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		// Запись значения узла
		err = binary.Write(file, binary.LittleEndian, int32(current.Data))
		if err != nil {
			return err
		}

		// Флаги наличия потомков
		hasLeft := current.Left != nil
		hasRight := current.Right != nil

		err = binary.Write(file, binary.LittleEndian, hasLeft)
		if err != nil {
			return err
		}
		err = binary.Write(file, binary.LittleEndian, hasRight)
		if err != nil {
			return err
		}

		if hasLeft {
			queue = append(queue, current.Left)
		}
		if hasRight {
			queue = append(queue, current.Right)
		}
	}

	fmt.Printf("Дерево сохранено в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла
func (f *FullBinaryTree) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	f.Clear()

	var nodeCount int32
	err = binary.Read(file, binary.LittleEndian, &nodeCount)
	if err != nil {
		return err
	}

	if nodeCount == 0 {
		fmt.Printf("Пустое дерево загружено из бинарного файла: %s\n", filename)
		return nil
	}

	// Чтение корня
	var rootValue int32
	var hasLeft, hasRight bool

	err = binary.Read(file, binary.LittleEndian, &rootValue)
	if err != nil {
		return err
	}
	err = binary.Read(file, binary.LittleEndian, &hasLeft)
	if err != nil {
		return err
	}
	err = binary.Read(file, binary.LittleEndian, &hasRight)
	if err != nil {
		return err
	}

	f.Root = NewTreeNode(int(rootValue))
	queue := []*TreeNode{f.Root}
	nodeCount--

	for len(queue) > 0 && nodeCount > 0 {
		current := queue[0]
		queue = queue[1:]

		// Чтение левого потомка
		if hasLeft && nodeCount > 0 {
			var leftValue int32
			var leftHasLeft, leftHasRight bool

			err = binary.Read(file, binary.LittleEndian, &leftValue)
			if err != nil {
				return err
			}
			err = binary.Read(file, binary.LittleEndian, &leftHasLeft)
			if err != nil {
				return err
			}
			err = binary.Read(file, binary.LittleEndian, &leftHasRight)
			if err != nil {
				return err
			}

			current.Left = NewTreeNode(int(leftValue))
			queue = append(queue, current.Left)
			nodeCount--
		}

		// Чтение правого потомка
		if hasRight && nodeCount > 0 {
			var rightValue int32
			var rightHasLeft, rightHasRight bool

			err = binary.Read(file, binary.LittleEndian, &rightValue)
			if err != nil {
				return err
			}
			err = binary.Read(file, binary.LittleEndian, &rightHasLeft)
			if err != nil {
				return err
			}
			err = binary.Read(file, binary.LittleEndian, &rightHasRight)
			if err != nil {
				return err
			}

			current.Right = NewTreeNode(int(rightValue))
			queue = append(queue, current.Right)
			nodeCount--
		}

		// Чтение флагов для следующего узла
		if len(queue) > 0 && nodeCount > 0 {
			err = binary.Read(file, binary.LittleEndian, &hasLeft)
			if err != nil {
				return err
			}
			err = binary.Read(file, binary.LittleEndian, &hasRight)
			if err != nil {
				return err
			}
		}
	}

	fmt.Printf("Дерево загружено из бинарного файла: %s\n", filename)
	return nil
}
