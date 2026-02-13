package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
)

const MAX_SIZE = 100

// Stack - стек на массиве
type Stack struct {
	data   [MAX_SIZE]int
	topIdx int
}

// NewStack - конструктор
func NewStack() *Stack {
	return &Stack{
		topIdx: -1,
	}
}

// Push - добавление элемента
func (s *Stack) Push(value int) {
	if s.topIdx >= MAX_SIZE-1 {
		fmt.Println("Стек переполнен!")
		return
	}
	s.topIdx++
	s.data[s.topIdx] = value
}

// Pop - удаление элемента
func (s *Stack) Pop() int {
	if s.IsEmpty() {
		fmt.Println("Стек пуст!")
		return -1
	}
	value := s.data[s.topIdx]
	s.topIdx--
	return value
}

// Peek - получение верхнего элемента
func (s *Stack) Peek() int {
	if s.IsEmpty() {
		return -1
	}
	return s.data[s.topIdx]
}

// IsEmpty - проверка на пустоту
func (s *Stack) IsEmpty() bool {
	return s.topIdx == -1
}

// Size - получение размера
func (s *Stack) Size() int {
	return s.topIdx + 1
}

// Print - печать стека
func (s *Stack) Print() {
	if s.IsEmpty() {
		fmt.Println("Стек пуст!")
		return
	}
	fmt.Print("Стек (сверху вниз): ")
	for i := s.topIdx; i >= 0; i-- {
		fmt.Printf("%d ", s.data[i])
	}
	fmt.Println()
}

// Clear - очистка стека
func (s *Stack) Clear() {
	s.topIdx = -1
}

// SaveToFile - сохранение в текстовый файл
func (s *Stack) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла: %s\n", filename)
		return err
	}
	defer file.Close()

	for i := 0; i <= s.topIdx; i++ {
		_, err := fmt.Fprintf(file, "%d ", s.data[i])
		if err != nil {
			return err
		}
	}

	fmt.Printf("Стек сохранен в файл: %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (s *Stack) LoadFromFile(filename string) error {
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
			s.Push(value)
		}
	}

	fmt.Printf("Стек загружен из файла: %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл
func (s *Stack) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла: %s\n", filename)
		return err
	}
	defer file.Close()

	stackSize := int32(s.Size())
	err = binary.Write(file, binary.LittleEndian, stackSize)
	if err != nil {
		return err
	}

	for i := 0; i <= s.topIdx; i++ {
		err = binary.Write(file, binary.LittleEndian, int32(s.data[i]))
		if err != nil {
			return err
		}
	}

	fmt.Printf("Стек сохранен в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла
func (s *Stack) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла: %s\n", filename)
		return err
	}
	defer file.Close()

	s.Clear()

	var stackSize int32
	err = binary.Read(file, binary.LittleEndian, &stackSize)
	if err != nil {
		return err
	}

	for i := 0; i < int(stackSize); i++ {
		var value int32
		err = binary.Read(file, binary.LittleEndian, &value)
		if err != nil {
			return err
		}
		s.Push(int(value))
	}

	fmt.Printf("Стек загружен из бинарного файла: %s\n", filename)
	return nil
}
