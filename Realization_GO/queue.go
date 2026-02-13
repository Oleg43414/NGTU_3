package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
)

// Queue - очередь на кольцевом буфере
type Queue struct {
	arr         []int
	capacity    int
	frontIdx    int
	rearIdx     int
	currentSize int
}

// NewQueue - конструктор
func NewQueue() *Queue {
	q := &Queue{
		arr:         make([]int, 4),
		capacity:    4,
		frontIdx:    0,
		rearIdx:     0,
		currentSize: 0,
	}
	return q
}

// resize - изменение размера
func (q *Queue) resize() {
	newCapacity := q.capacity * 2
	newArr := make([]int, newCapacity)

	i := 0
	j := q.frontIdx
	for i < q.currentSize {
		newArr[i] = q.arr[j]
		j = (j + 1) % q.capacity
		i++
	}

	q.arr = newArr
	q.capacity = newCapacity
	q.frontIdx = 0
	q.rearIdx = q.currentSize
}

// Push - добавление элемента
func (q *Queue) Push(value int) {
	if q.currentSize == q.capacity {
		q.resize()
	}

	q.arr[q.rearIdx] = value
	q.rearIdx = (q.rearIdx + 1) % q.capacity
	q.currentSize++

	fmt.Printf("Добавлен элемент: %d\n", value)
}

// Pop - удаление элемента
func (q *Queue) Pop() {
	if q.IsEmpty() {
		fmt.Println("Очередь пуста! Нельзя удалить элемент.")
		return
	}

	frontValue := q.arr[q.frontIdx]
	q.frontIdx = (q.frontIdx + 1) % q.capacity
	q.currentSize--

	fmt.Printf("Удален элемент: %d\n", frontValue)
}

// Front - получение первого элемента
func (q *Queue) Front() int {
	if q.IsEmpty() {
		fmt.Println("Очередь пуста!")
		return -1
	}
	return q.arr[q.frontIdx]
}

// IsEmpty - проверка на пустоту
func (q *Queue) IsEmpty() bool {
	return q.currentSize == 0
}

// Size - получение размера
func (q *Queue) Size() int {
	return q.currentSize
}

// Display - отображение очереди
func (q *Queue) Display() {
	if q.IsEmpty() {
		fmt.Println("Очередь пуста!")
		return
	}

	fmt.Print("Содержимое очереди: ")
	idx := q.frontIdx
	for i := 0; i < q.currentSize; i++ {
		fmt.Printf("%d ", q.arr[idx])
		idx = (idx + 1) % q.capacity
	}
	fmt.Println()
}

// Clear - очистка очереди
func (q *Queue) Clear() {
	q.frontIdx = 0
	q.rearIdx = 0
	q.currentSize = 0
	fmt.Println("Очередь очищена!")
}

// SaveToFile - сохранение в текстовый файл
func (q *Queue) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла: %s\n", filename)
		return err
	}
	defer file.Close()

	idx := q.frontIdx
	for i := 0; i < q.currentSize; i++ {
		_, err := fmt.Fprintf(file, "%d ", q.arr[idx])
		if err != nil {
			return err
		}
		idx = (idx + 1) % q.capacity
	}

	fmt.Printf("Очередь сохранена в файл: %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (q *Queue) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла: %s\n", filename)
		return err
	}
	defer file.Close()

	q.Clear()

	scanner := bufio.NewScanner(file)
	scanner.Split(bufio.ScanWords)

	for scanner.Scan() {
		value, err := strconv.Atoi(scanner.Text())
		if err == nil {
			q.Push(value)
		}
	}

	fmt.Printf("Очередь загружена из файла: %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл
func (q *Queue) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	queueSize := int64(q.currentSize)
	err = binary.Write(file, binary.LittleEndian, queueSize)
	if err != nil {
		return err
	}

	idx := q.frontIdx
	for i := 0; i < q.currentSize; i++ {
		err = binary.Write(file, binary.LittleEndian, int32(q.arr[idx]))
		if err != nil {
			return err
		}
		idx = (idx + 1) % q.capacity
	}

	fmt.Printf("Очередь сохранена в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла
func (q *Queue) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	q.Clear()

	var queueSize int64
	err = binary.Read(file, binary.LittleEndian, &queueSize)
	if err != nil {
		return err
	}

	for i := 0; i < int(queueSize); i++ {
		var value int32
		err = binary.Read(file, binary.LittleEndian, &value)
		if err != nil {
			return err
		}
		q.Push(int(value))
	}

	fmt.Printf("Очередь загружена из бинарного файла: %s\n", filename)
	return nil
}
