package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// DynamicArray - динамический массив строк
type DynamicArray struct {
	data     []string
	size     int
	capacity int
}

// NewDynamicArray - конструктор
func NewDynamicArray(initialCapacity int) *DynamicArray {
	if initialCapacity < 1 {
		initialCapacity = 10
	}
	return &DynamicArray{
		data:     make([]string, initialCapacity),
		size:     0,
		capacity: initialCapacity,
	}
}

// resize - изменение размера массива
func (d *DynamicArray) resize(newCapacity int) {
	newData := make([]string, newCapacity)
	for i := 0; i < d.size; i++ {
		newData[i] = d.data[i]
	}
	d.data = newData
	d.capacity = newCapacity
}

// PushBack - добавление в конец
func (d *DynamicArray) PushBack(value string) {
	if d.size >= d.capacity {
		newCap := d.capacity * 2
		if newCap == 0 {
			newCap = 4
		}
		d.resize(newCap)
	}
	d.data[d.size] = value
	d.size++
}

// Insert - вставка по индексу
func (d *DynamicArray) Insert(index int, value string) {
	if index < 0 || index > d.size {
		fmt.Println("Неверный индекс!")
		return
	}

	if d.size >= d.capacity {
		d.resize(d.capacity * 2)
	}

	for i := d.size; i > index; i-- {
		d.data[i] = d.data[i-1]
	}

	d.data[index] = value
	d.size++
}

// Remove - удаление по индексу
func (d *DynamicArray) Remove(index int) {
	if index < 0 || index >= d.size {
		fmt.Println("Неверный индекс")
		return
	}

	for i := index; i < d.size-1; i++ {
		d.data[i] = d.data[i+1]
	}
	d.size--

	if d.size < d.capacity/4 && d.capacity > 10 {
		d.resize(d.capacity / 2)
	}
}

// Get - получение элемента по индексу
func (d *DynamicArray) Get(index int) string {
	if index < 0 || index >= d.size {
		fmt.Println("Неверный индекс")
		return ""
	}
	return d.data[index]
}

// Set - установка значения по индексу
func (d *DynamicArray) Set(index int, value string) {
	if index < 0 || index >= d.size {
		fmt.Println("Неверный индекс")
		return
	}
	d.data[index] = value
}

// GetSize - получение размера
func (d *DynamicArray) GetSize() int {
	return d.size
}

// GetCapacity - получение емкости
func (d *DynamicArray) GetCapacity() int {
	return d.capacity
}

// Print - печать массива
func (d *DynamicArray) Print() {
	fmt.Printf("Массив[%d/%d]: ", d.size, d.capacity)
	for i := 0; i < d.size; i++ {
		fmt.Printf("\"%s\" ", d.data[i])
	}
	fmt.Println()
}

// Clear - очистка массива
func (d *DynamicArray) Clear() {
	d.size = 0
}

// SaveToFile - сохранение в текстовый файл
func (d *DynamicArray) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка создания файла: %s\n", filename)
		return err
	}
	defer file.Close()

	_, err = fmt.Fprintf(file, "%d\n", d.size)
	if err != nil {
		return err
	}

	for i := 0; i < d.size; i++ {
		_, err = fmt.Fprintf(file, "%s\n", d.data[i])
		if err != nil {
			return err
		}
	}

	fmt.Printf("Массив сохранён в файл: %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (d *DynamicArray) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла: %s\n", filename)
		return err
	}
	defer file.Close()

	d.Clear()

	scanner := bufio.NewScanner(file)

	// Читаем размер
	if !scanner.Scan() {
		return fmt.Errorf("файл пуст")
	}
	n, err := strconv.Atoi(scanner.Text())
	if err != nil {
		return err
	}

	// Читаем строки
	for i := 0; i < n && scanner.Scan(); i++ {
		value := scanner.Text()
		value = strings.TrimRight(value, "\n\r")
		d.PushBack(value)
	}

	fmt.Printf("Массив загружен из файла: %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл
func (d *DynamicArray) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка создания бинарного файла: %s\n", filename)
		return err
	}
	defer file.Close()

	// Запись размера
	err = binary.Write(file, binary.LittleEndian, int32(d.size))
	if err != nil {
		return err
	}

	// Запись строк
	for i := 0; i < d.size; i++ {
		str := d.data[i]
		strLen := int32(len(str))

		// Запись длины строки
		err = binary.Write(file, binary.LittleEndian, strLen)
		if err != nil {
			return err
		}

		// Запись строки
		_, err = file.Write([]byte(str))
		if err != nil {
			return err
		}
	}

	fmt.Printf("Динамический массив сохранён в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла
func (d *DynamicArray) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла: %s\n", filename)
		return err
	}
	defer file.Close()

	d.Clear()

	var fileSize int32
	err = binary.Read(file, binary.LittleEndian, &fileSize)
	if err != nil {
		return err
	}

	for i := 0; i < int(fileSize); i++ {
		var strLen int32
		err = binary.Read(file, binary.LittleEndian, &strLen)
		if err != nil {
			return err
		}

		buffer := make([]byte, strLen)
		_, err = file.Read(buffer)
		if err != nil {
			return err
		}

		d.PushBack(string(buffer))
	}

	fmt.Printf("Динамический массив загружен из бинарного файла: %s\n", filename)
	return nil
}
