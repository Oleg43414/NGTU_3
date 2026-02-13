package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
)

// Node - узел односвязного списка
type Node struct {
	Key  int
	Next *Node
}

// SinglyLinkedList - односвязный список
type SinglyLinkedList struct {
	Head *Node
}

// NewSinglyLinkedList - конструктор
func NewSinglyLinkedList() *SinglyLinkedList {
	return &SinglyLinkedList{
		Head: nil,
	}
}

// createNode - создание нового узла
func (s *SinglyLinkedList) createNode(value int) *Node {
	return &Node{
		Key:  value,
		Next: nil,
	}
}

// AddToHead - добавление в начало
func (s *SinglyLinkedList) AddToHead(value int) {
	newNode := s.createNode(value)
	newNode.Next = s.Head
	s.Head = newNode
	fmt.Printf("Добавлен в начало: %d\n", value)
}

// AddToTail - добавление в конец
func (s *SinglyLinkedList) AddToTail(value int) {
	newNode := s.createNode(value)

	if s.Head == nil {
		s.Head = newNode
		fmt.Printf("Добавлен в конец: %d\n", value)
		return
	}

	current := s.Head
	for current.Next != nil {
		current = current.Next
	}
	current.Next = newNode
	fmt.Printf("Добавлен в конец: %d\n", value)
}

// AddAfter - добавление после значения
func (s *SinglyLinkedList) AddAfter(targetValue, newValue int) {
	if s.Head == nil {
		fmt.Println("Список пуст")
		return
	}

	current := s.Head
	for current != nil && current.Key != targetValue {
		current = current.Next
	}

	if current == nil {
		fmt.Printf("%d не найден!\n", targetValue)
		return
	}

	newNode := s.createNode(newValue)
	newNode.Next = current.Next
	current.Next = newNode
	fmt.Printf("Добавлен после %d: %d\n", targetValue, newValue)
}

// AddBefore - добавление перед значением
func (s *SinglyLinkedList) AddBefore(targetValue, newValue int) {
	if s.Head == nil {
		fmt.Println("Список пуст")
		return
	}

	if s.Head.Key == targetValue {
		s.AddToHead(newValue)
		return
	}

	current := s.Head
	for current.Next != nil && current.Next.Key != targetValue {
		current = current.Next
	}

	if current.Next == nil {
		fmt.Printf("%d не найден!\n", targetValue)
		return
	}

	newNode := s.createNode(newValue)
	newNode.Next = current.Next
	current.Next = newNode
	fmt.Printf("Добавлен перед %d: %d\n", targetValue, newValue)
}

// DeleteBefore - удаление перед значением
func (s *SinglyLinkedList) DeleteBefore(targetValue int) {
	if s.Head == nil || s.Head.Next == nil {
		fmt.Println("Недостаточно элементов для удаления!")
		return
	}

	if s.Head.Key == targetValue {
		fmt.Println("Невозможно удалить элемент перед первым элементом!")
		return
	}

	if s.Head.Next.Key == targetValue {
		temp := s.Head
		s.Head = s.Head.Next
		fmt.Printf("Удален элемент: %d перед элементом %d\n", temp.Key, targetValue)
		return
	}

	current := s.Head
	for current.Next.Next != nil && current.Next.Next.Key != targetValue {
		current = current.Next
	}

	if current.Next.Next == nil {
		fmt.Printf("Элемент %d не найден!\n", targetValue)
		return
	}

	nodeToDelete := current.Next
	current.Next = current.Next.Next
	fmt.Printf("Удален элемент: %d перед элементом %d\n", nodeToDelete.Key, targetValue)
}

// DeleteAfter - удаление после значения
func (s *SinglyLinkedList) DeleteAfter(targetValue int) {
	if s.Head == nil {
		fmt.Println("Список пуст! Невозможно удалить элемент.")
		return
	}

	current := s.Head
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
	current.Next = current.Next.Next
	fmt.Printf("Удален элемент: %d после элемента %d\n", nodeToDelete.Key, targetValue)
}

// Print - печать списка
func (s *SinglyLinkedList) Print() {
	if s.Head == nil {
		fmt.Println("Список пуст")
		return
	}
	current := s.Head
	for current != nil {
		fmt.Printf("%d -> ", current.Key)
		current = current.Next
	}
	fmt.Println("nullptr")
}

// PrintReverse - печать в обратном порядке
func (s *SinglyLinkedList) PrintReverse() {
	if s.Head == nil {
		fmt.Println("Список пуст")
		return
	}

	stack := []int{}
	current := s.Head

	for current != nil {
		stack = append(stack, current.Key)
		current = current.Next
	}

	fmt.Print("Список в обратном порядке: ")
	for i := len(stack) - 1; i >= 0; i-- {
		fmt.Printf("%d -> ", stack[i])
	}
	fmt.Println("nullptr")
}

// DeleteFromHead - удаление из начала
func (s *SinglyLinkedList) DeleteFromHead() {
	if s.Head == nil {
		fmt.Println("Список пуст!")
		return
	}
	temp := s.Head
	s.Head = s.Head.Next
	fmt.Printf("Удален из начала: %d\n", temp.Key)
}

// DeleteFromTail - удаление из конца
func (s *SinglyLinkedList) DeleteFromTail() {
	if s.Head == nil {
		fmt.Println("Список пуст!")
		return
	}

	if s.Head.Next == nil {
		fmt.Printf("Удален из конца: %d\n", s.Head.Key)
		s.Head = nil
		return
	}

	current := s.Head
	for current.Next.Next != nil {
		current = current.Next
	}

	temp := current.Next
	current.Next = nil
	fmt.Printf("Удален из конца: %d\n", temp.Key)
}

// DeleteByValue - удаление по значению
func (s *SinglyLinkedList) DeleteByValue(value int) {
	if s.Head == nil {
		fmt.Println("Список пуст!")
		return
	}

	// Удаление головы
	if s.Head.Key == value {
		temp := s.Head
		s.Head = s.Head.Next
		fmt.Printf("Удален элемент: %d\n", temp.Key)
		return
	}

	// Удаление не головы
	current := s.Head
	var prev *Node

	for current != nil && current.Key != value {
		prev = current
		current = current.Next
	}

	if current == nil {
		fmt.Printf("Элемент %d не найден!\n", value)
		return
	}

	if prev != nil {
		prev.Next = current.Next
	}

	fmt.Printf("Удален элемент: %d\n", current.Key)
}

// FindByValue - поиск по значению
func (s *SinglyLinkedList) FindByValue(value int) bool {
	current := s.Head
	for current != nil {
		if current.Key == value {
			return true
		}
		current = current.Next
	}
	return false
}

// Clear - очистка списка
func (s *SinglyLinkedList) Clear() {
	s.Head = nil
}

// SaveToFile - сохранение в текстовый файл
func (s *SinglyLinkedList) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка создания файла: %s\n", filename)
		return err
	}
	defer file.Close()

	current := s.Head
	for current != nil {
		_, err := fmt.Fprintf(file, "%d ", current.Key)
		if err != nil {
			return err
		}
		current = current.Next
	}

	fmt.Printf("Односвязный список сохранён в файл: %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (s *SinglyLinkedList) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла: %s\n", filename)
		return err
	}
	defer file.Close()

	s.Clear()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	for scanner.Scan() {
		value, err := strconv.Atoi(scanner.Text())
		if err == nil {
			s.AddToTail(value)
		}
	}

	fmt.Printf("Односвязный список загружен из файла: %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл
func (s *SinglyLinkedList) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	count := 0
	current := s.Head
	for current != nil {
		count++
		current = current.Next
	}

	err = binary.Write(file, binary.LittleEndian, int32(count))
	if err != nil {
		return err
	}

	current = s.Head
	for current != nil {
		err = binary.Write(file, binary.LittleEndian, int32(current.Key))
		if err != nil {
			return err
		}
		current = current.Next
	}

	fmt.Printf("Односвязный список сохранён в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла
func (s *SinglyLinkedList) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	s.Clear()

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
		s.AddToTail(int(value))
	}

	fmt.Printf("Односвязный список загружен из бинарного файла: %s\n", filename)
	return nil
}
