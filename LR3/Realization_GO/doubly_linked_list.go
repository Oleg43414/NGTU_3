package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
)

// DoublyNode - узел двусвязного списка
type DoublyNode struct {
	Key  int
	Next *DoublyNode
	Prev *DoublyNode
}

// DoublyLinkedList - двусвязный список
type DoublyLinkedList struct {
	Head *DoublyNode
	Tail *DoublyNode
}

// NewDoublyLinkedList - конструктор
func NewDoublyLinkedList() *DoublyLinkedList {
	return &DoublyLinkedList{
		Head: nil,
		Tail: nil,
	}
}

// createNode - создание нового узла
func (d *DoublyLinkedList) createNode(value int) *DoublyNode {
	return &DoublyNode{
		Key:  value,
		Next: nil,
		Prev: nil,
	}
}

// AddToHead - добавление в начало
func (d *DoublyLinkedList) AddToHead(value int) {
	newNode := d.createNode(value)

	if d.Head == nil {
		d.Head = newNode
		d.Tail = newNode
	} else {
		newNode.Next = d.Head
		d.Head.Prev = newNode
		d.Head = newNode
	}
	fmt.Printf("Добавлен в начало списка: %d\n", value)
}

// AddToTail - добавление в конец
func (d *DoublyLinkedList) AddToTail(value int) {
	newNode := d.createNode(value)

	if d.Tail == nil {
		d.Head = newNode
		d.Tail = newNode
	} else {
		newNode.Prev = d.Tail
		d.Tail.Next = newNode
		d.Tail = newNode
	}
	fmt.Printf("Добавлен в конец списка: %d\n", value)
}

// AddAfterValue - добавление после значения
func (d *DoublyLinkedList) AddAfterValue(targetValue, newValue int) {
	if d.Head == nil {
		fmt.Println("Список пуст! Невозможно добавить после значения.")
		return
	}

	current := d.Head
	for current != nil && current.Key != targetValue {
		current = current.Next
	}

	if current == nil {
		fmt.Printf("Элемент %d не найден в списке!\n", targetValue)
		return
	}

	newNode := d.createNode(newValue)
	newNode.Next = current.Next
	newNode.Prev = current

	if current.Next != nil {
		current.Next.Prev = newNode
	} else {
		d.Tail = newNode
	}

	current.Next = newNode
	fmt.Printf("Добавлен элемент %d после элемента %d\n", newValue, targetValue)
}

// AddBeforeValue - добавление перед значением
func (d *DoublyLinkedList) AddBeforeValue(targetValue, newValue int) {
	if d.Head == nil {
		fmt.Println("Список пуст! Невозможно добавить перед значением.")
		return
	}

	current := d.Head
	for current != nil && current.Key != targetValue {
		current = current.Next
	}

	if current == nil {
		fmt.Printf("Элемент %d не найден в списке!\n", targetValue)
		return
	}

	newNode := d.createNode(newValue)
	newNode.Prev = current.Prev
	newNode.Next = current

	if current.Prev != nil {
		current.Prev.Next = newNode
	} else {
		d.Head = newNode
	}

	current.Prev = newNode
	fmt.Printf("Добавлен элемент %d перед элементом %d\n", newValue, targetValue)
}

// DeleteFromHead - удаление из начала
func (d *DoublyLinkedList) DeleteFromHead() {
	if d.Head == nil {
		fmt.Println("Список пуст!")
		return
	}

	temp := d.Head
	d.Head = d.Head.Next

	if d.Head != nil {
		d.Head.Prev = nil
	} else {
		d.Tail = nil
	}

	fmt.Printf("Удален из начала: %d\n", temp.Key)
}

// DeleteFromTail - удаление из конца
func (d *DoublyLinkedList) DeleteFromTail() {
	if d.Tail == nil {
		fmt.Println("Список пуст!")
		return
	}

	temp := d.Tail
	d.Tail = d.Tail.Prev

	if d.Tail != nil {
		d.Tail.Next = nil
	} else {
		d.Head = nil
	}

	fmt.Printf("Удален из конца: %d\n", temp.Key)
}

// DeleteByValue - удаление по значению
func (d *DoublyLinkedList) DeleteByValue(value int) {
	if d.Head == nil {
		fmt.Println("Список пуст!")
		return
	}

	current := d.Head
	for current != nil && current.Key != value {
		current = current.Next
	}

	if current == nil {
		fmt.Printf("Элемент %d не найден!\n", value)
		return
	}

	if current.Prev != nil {
		current.Prev.Next = current.Next
	} else {
		d.Head = current.Next
	}

	if current.Next != nil {
		current.Next.Prev = current.Prev
	} else {
		d.Tail = current.Prev
	}

	fmt.Printf("Удален элемент: %d\n", current.Key)
}

// DeleteAfterValue - удаление после значения
func (d *DoublyLinkedList) DeleteAfterValue(targetValue int) {
	if d.Head == nil {
		fmt.Println("Список пуст! Невозможно удалить элемент.")
		return
	}

	current := d.Head
	for current != nil && current.Key != targetValue {
		current = current.Next
	}

	if current == nil {
		fmt.Printf("Элемент %d не найден в списке!\n", targetValue)
		return
	}

	if current.Next == nil {
		fmt.Printf("После элемента %d нет элементов для удаления!\n", targetValue)
		return
	}

	nodeToDelete := current.Next
	current.Next = nodeToDelete.Next

	if nodeToDelete.Next != nil {
		nodeToDelete.Next.Prev = current
	} else {
		d.Tail = current
	}

	fmt.Printf("Удален элемент: %d после элемента %d\n", nodeToDelete.Key, targetValue)
}

// DeleteBeforeValue - удаление перед значением
func (d *DoublyLinkedList) DeleteBeforeValue(targetValue int) {
	if d.Head == nil {
		fmt.Println("Список пуст! Невозможно удалить элемент.")
		return
	}

	current := d.Head
	for current != nil && current.Key != targetValue {
		current = current.Next
	}

	if current == nil {
		fmt.Printf("Элемент %d не найден в списке!\n", targetValue)
		return
	}

	if current.Prev == nil {
		fmt.Printf("Перед элементом %d нет элементов для удаления!\n", targetValue)
		return
	}

	nodeToDelete := current.Prev
	current.Prev = nodeToDelete.Prev

	if nodeToDelete.Prev != nil {
		nodeToDelete.Prev.Next = current
	} else {
		d.Head = current
	}

	fmt.Printf("Удален элемент: %d перед элементом %d\n", nodeToDelete.Key, targetValue)
}

// FindByValue - поиск по значению
func (d *DoublyLinkedList) FindByValue(value int) bool {
	current := d.Head
	for current != nil {
		if current.Key == value {
			return true
		}
		current = current.Next
	}
	return false
}

// PrintForward - печать с начала в конец
func (d *DoublyLinkedList) PrintForward() {
	fmt.Print("Список (начало -> конец): ")
	current := d.Head
	for current != nil {
		fmt.Printf("%d ", current.Key)
		current = current.Next
	}
	fmt.Println()
}

// PrintBackward - печать с конца в начало
func (d *DoublyLinkedList) PrintBackward() {
	fmt.Print("Список (конец -> начало): ")
	current := d.Tail
	for current != nil {
		fmt.Printf("%d ", current.Key)
		current = current.Prev
	}
	fmt.Println()
}

// Clear - очистка списка
func (d *DoublyLinkedList) Clear() {
	d.Head = nil
	d.Tail = nil
	fmt.Println("Двусвязный список очищен")
}

// SaveToFile - сохранение в текстовый файл
func (d *DoublyLinkedList) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка создания файла: %s\n", filename)
		return err
	}
	defer file.Close()

	current := d.Head
	for current != nil {
		_, err := fmt.Fprintf(file, "%d ", current.Key)
		if err != nil {
			return err
		}
		current = current.Next
	}
	fmt.Printf("Двусвязный список сохранён в файл: %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (d *DoublyLinkedList) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла: %s\n", filename)
		return err
	}
	defer file.Close()

	d.Clear()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	for scanner.Scan() {
		value, err := strconv.Atoi(scanner.Text())
		if err == nil {
			d.AddToTail(value)
		}
	}

	fmt.Printf("Двусвязный список загружен из файла: %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл
func (d *DoublyLinkedList) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка создания бинарного файла: %s\n", filename)
		return err
	}
	defer file.Close()

	// Подсчет количества элементов
	count := 0
	current := d.Head
	for current != nil {
		count++
		current = current.Next
	}

	// Запись количества элементов
	err = binary.Write(file, binary.LittleEndian, int32(count))
	if err != nil {
		return err
	}

	// Запись элементов
	current = d.Head
	for current != nil {
		err = binary.Write(file, binary.LittleEndian, int32(current.Key))
		if err != nil {
			return err
		}
		current = current.Next
	}

	fmt.Printf("Двусвязный список сохранён в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла
func (d *DoublyLinkedList) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла: %s\n", filename)
		return err
	}
	defer file.Close()

	d.Clear()

	var count int32
	err = binary.Read(file, binary.LittleEndian, &count)
	if err != nil {
		return err
	}

	for i := 0; i < int(count); i++ {
		var value int32
		err = binary.Read(file, binary.LittleEndian, &value)
		if err != nil {
			return err
		}
		d.AddToTail(int(value))
	}

	fmt.Printf("Двусвязный список загружен из бинарного файла: %s\n", filename)
	return nil
}
