package main

import (
	"bufio"
	"encoding/binary"
	"fmt"
	"os"
	"strconv"
	"strings"
)

// ==================== Хеш-функция ====================

// StringHash - экспортируемая хеш-функция
func StringHash(key string, mod int) int {
	if mod <= 0 {
		return 0
	}
	hashVal := 0
	prime := 31

	for _, ch := range key {
		hashVal = (hashVal*prime + int(ch)) % mod
		if hashVal < 0 {
			hashVal = -hashVal
		}
	}
	return hashVal
}

// ==================== HashNode ====================

// HashNode - узел цепочечной хеш-таблицы
type HashNode struct {
	Key   string
	Value string
	Next  *HashNode
}

// NewHashNode - конструктор узла
func NewHashNode(key, value string) *HashNode {
	return &HashNode{
		Key:   key,
		Value: value,
		Next:  nil,
	}
}

// ==================== ChainHashTable ====================

// ChainHashTable - цепочечная хеш-таблица
type ChainHashTable struct {
	table    []*HashNode
	capacity int
}

// NewChainHashTable - конструктор
func NewChainHashTable(size int) *ChainHashTable {
	if size < 1 {
		size = 1
	}
	cht := &ChainHashTable{
		table:    make([]*HashNode, size),
		capacity: size,
	}
	return cht
}

// hashFunction - хеш-функция
func (c *ChainHashTable) hashFunction(key string) int {
	return StringHash(key, c.capacity)
}

// Insert - вставка элемента
func (c *ChainHashTable) Insert(key, value string) {
	index := c.hashFunction(key)

	// Проверка на существование ключа
	current := c.table[index]
	for current != nil {
		if current.Key == key {
			current.Value = value
			fmt.Printf("Ключ '%s' обновлен\n", key)
			return
		}
		current = current.Next
	}

	// Вставка нового узла
	newNode := NewHashNode(key, value)
	newNode.Next = c.table[index]
	c.table[index] = newNode

	fmt.Printf("Добавлено: %s -> %s (цепочка %d)\n", key, value, index)
}

// Remove - удаление элемента
func (c *ChainHashTable) Remove(key string) bool {
	index := c.hashFunction(key)
	current := c.table[index]
	var previous *HashNode

	for current != nil {
		if current.Key == key {
			if previous == nil {
				c.table[index] = current.Next
			} else {
				previous.Next = current.Next
			}
			fmt.Printf("Ключ '%s' удален\n", key)
			return true
		}
		previous = current
		current = current.Next
	}

	fmt.Printf("Ключ '%s' не найден\n", key)
	return false
}

// Find - поиск элемента
func (c *ChainHashTable) Find(key string) string {
	index := c.hashFunction(key)
	current := c.table[index]

	for current != nil {
		if current.Key == key {
			return current.Value
		}
		current = current.Next
	}
	return ""
}

// Display - отображение таблицы
func (c *ChainHashTable) Display() {
	fmt.Printf("\n=== ЦЕПОЧЕЧНАЯ ХЕШ-ТАБЛИЦА (%d ячеек) ===\n", c.capacity)

	isEmpty := true
	for i := 0; i < c.capacity; i++ {
		if c.table[i] != nil {
			isEmpty = false
			fmt.Printf("[%d]: ", i)

			current := c.table[i]
			for current != nil {
				fmt.Printf("%s=%s", current.Key, current.Value)
				if current.Next != nil {
					fmt.Printf(" -> ")
				}
				current = current.Next
			}
			fmt.Println()
		}
	}

	if isEmpty {
		fmt.Println("Таблица пуста")
	}
}

// Clear - очистка таблицы
func (c *ChainHashTable) Clear() {
	for i := 0; i < c.capacity; i++ {
		c.table[i] = nil
	}
	fmt.Println("Цепочечная таблица очищена")
}

// Resize - изменение размера
func (c *ChainHashTable) Resize(newSize int) {
	if newSize < 1 {
		fmt.Println("Некорректный размер")
		return
	}

	oldTable := c.table
	oldCapacity := c.capacity

	c.capacity = newSize
	c.table = make([]*HashNode, newSize)

	// Перехеширование
	for i := 0; i < oldCapacity; i++ {
		current := oldTable[i]
		for current != nil {
			next := current.Next
			newIndex := c.hashFunction(current.Key)
			current.Next = c.table[newIndex]
			c.table[newIndex] = current
			current = next
		}
	}

	fmt.Printf("Таблица изменена до %d ячеек\n", c.capacity)
}

// GetSize - получение количества элементов
func (c *ChainHashTable) GetSize() int {
	count := 0
	for i := 0; i < c.capacity; i++ {
		current := c.table[i]
		for current != nil {
			count++
			current = current.Next
		}
	}
	return count
}

// GetCapacity - получение емкости
func (c *ChainHashTable) GetCapacity() int {
	return c.capacity
}

// SaveToFile - сохранение в текстовый файл
func (c *ChainHashTable) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	_, err = fmt.Fprintf(file, "CHAIN_HASH_TABLE\n")
	if err != nil {
		return err
	}
	_, err = fmt.Fprintf(file, "%d\n", c.capacity)
	if err != nil {
		return err
	}

	for i := 0; i < c.capacity; i++ {
		current := c.table[i]
		for current != nil {
			_, err = fmt.Fprintf(file, "%s %s\n", current.Key, current.Value)
			if err != nil {
				return err
			}
			current = current.Next
		}
	}

	fmt.Printf("Цепочечная таблица сохранена в %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (c *ChainHashTable) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	if !scanner.Scan() {
		return fmt.Errorf("файл пуст")
	}
	typeStr := scanner.Text()
	if typeStr != "CHAIN_HASH_TABLE" {
		fmt.Println("Неверный формат файла")
		return fmt.Errorf("неверный формат файла")
	}

	if !scanner.Scan() {
		return fmt.Errorf("файл поврежден")
	}
	fileCapacity, err := strconv.Atoi(scanner.Text())
	if err != nil {
		return err
	}

	c.Clear()
	if fileCapacity != c.capacity {
		c.table = make([]*HashNode, fileCapacity)
		c.capacity = fileCapacity
	}

	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.SplitN(line, " ", 2)
		if len(parts) == 2 {
			c.Insert(parts[0], parts[1])
		}
	}

	fmt.Printf("Цепочечная таблица загружена из %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл (ChainHashTable)
func (c *ChainHashTable) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	// Сигнатура - ровно 16 байт
	signature := []byte("CHAIN_HASH_BIN_16")
	_, err = file.Write(signature)
	if err != nil {
		return err
	}

	// Запись capacity
	err = binary.Write(file, binary.LittleEndian, int32(c.capacity))
	if err != nil {
		return err
	}

	// Запись количества элементов
	numElements := int32(c.GetSize())
	err = binary.Write(file, binary.LittleEndian, numElements)
	if err != nil {
		return err
	}

	// Запись элементов
	for i := 0; i < c.capacity; i++ {
		current := c.table[i]
		for current != nil {
			keyLen := int32(len(current.Key))
			valueLen := int32(len(current.Value))

			err = binary.Write(file, binary.LittleEndian, keyLen)
			if err != nil {
				return err
			}
			_, err = file.Write([]byte(current.Key))
			if err != nil {
				return err
			}

			err = binary.Write(file, binary.LittleEndian, valueLen)
			if err != nil {
				return err
			}
			_, err = file.Write([]byte(current.Value))
			if err != nil {
				return err
			}

			current = current.Next
		}
	}

	fmt.Printf("Цепочечная таблица сохранена в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла (ChainHashTable)
func (c *ChainHashTable) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	// Чтение сигнатуры
	signature := make([]byte, 16)
	_, err = file.Read(signature)
	if err != nil {
		return err
	}
	if string(signature) != "CHAIN_HASH_BIN_16" {
		fmt.Println("Неверный формат файла")
		return fmt.Errorf("неверный формат файла")
	}

	var fileCapacity int32
	err = binary.Read(file, binary.LittleEndian, &fileCapacity)
	if err != nil {
		return err
	}

	c.Clear()
	c.capacity = int(fileCapacity)
	c.table = make([]*HashNode, c.capacity)

	var numElements int32
	err = binary.Read(file, binary.LittleEndian, &numElements)
	if err != nil {
		return err
	}

	for i := 0; i < int(numElements); i++ {
		var keyLen int32
		err = binary.Read(file, binary.LittleEndian, &keyLen)
		if err != nil {
			return err
		}

		keyBuf := make([]byte, keyLen)
		_, err = file.Read(keyBuf)
		if err != nil {
			return err
		}
		key := string(keyBuf)

		var valueLen int32
		err = binary.Read(file, binary.LittleEndian, &valueLen)
		if err != nil {
			return err
		}

		valueBuf := make([]byte, valueLen)
		_, err = file.Read(valueBuf)
		if err != nil {
			return err
		}
		value := string(valueBuf)

		c.Insert(key, value)
	}

	fmt.Printf("Цепочечная таблица загружена из %s\n", filename)
	return nil
}

// ==================== OpenHashEntry ====================

// OpenHashEntry - запись в открытой хеш-таблице
type OpenHashEntry struct {
	Key     string
	Value   string
	Used    bool
	Deleted bool
}

// NewOpenHashEntry - конструктор
func NewOpenHashEntry() OpenHashEntry {
	return OpenHashEntry{
		Used:    false,
		Deleted: false,
	}
}

// ==================== OpenHashTable ====================

// OpenHashTable - хеш-таблица с открытой адресацией
type OpenHashTable struct {
	table    []OpenHashEntry
	capacity int
}

// NewOpenHashTable - конструктор
func NewOpenHashTable(size int) *OpenHashTable {
	if size < 1 {
		size = 1
	}
	oht := &OpenHashTable{
		table:    make([]OpenHashEntry, size),
		capacity: size,
	}
	for i := 0; i < size; i++ {
		oht.table[i] = NewOpenHashEntry()
	}
	return oht
}

// hashFunction - хеш-функция
func (o *OpenHashTable) hashFunction(key string) int {
	return StringHash(key, o.capacity)
}

// probeFunction - функция пробирования
func (o *OpenHashTable) probeFunction(index, attempt int) int {
	return (index + attempt) % o.capacity
}

// Insert - вставка элемента
func (o *OpenHashTable) Insert(key, value string) {
	index := o.hashFunction(key)

	for attempt := 0; attempt < o.capacity; attempt++ {
		currentIndex := o.probeFunction(index, attempt)

		if !o.table[currentIndex].Used || o.table[currentIndex].Deleted {
			o.table[currentIndex].Key = key
			o.table[currentIndex].Value = value
			o.table[currentIndex].Used = true
			o.table[currentIndex].Deleted = false
			fmt.Printf("Добавлено: %s -> %s (ячейка %d)\n", key, value, currentIndex)
			return
		}

		if o.table[currentIndex].Used && o.table[currentIndex].Key == key {
			o.table[currentIndex].Value = value
			fmt.Printf("Ключ '%s' обновлен\n", key)
			return
		}
	}

	fmt.Printf("Таблица переполнена, невозможно добавить '%s'\n", key)
}

// Remove - удаление элемента
func (o *OpenHashTable) Remove(key string) bool {
	index := o.hashFunction(key)

	for attempt := 0; attempt < o.capacity; attempt++ {
		currentIndex := o.probeFunction(index, attempt)

		if o.table[currentIndex].Used && o.table[currentIndex].Key == key {
			o.table[currentIndex].Used = false
			o.table[currentIndex].Deleted = true
			fmt.Printf("Ключ '%s' удален\n", key)
			return true
		}

		if !o.table[currentIndex].Used && !o.table[currentIndex].Deleted {
			break
		}
	}

	fmt.Printf("Ключ '%s' не найден\n", key)
	return false
}

// Find - поиск элемента
func (o *OpenHashTable) Find(key string) string {
	index := o.hashFunction(key)

	for attempt := 0; attempt < o.capacity; attempt++ {
		currentIndex := o.probeFunction(index, attempt)

		if o.table[currentIndex].Used && o.table[currentIndex].Key == key {
			return o.table[currentIndex].Value
		}

		if !o.table[currentIndex].Used && !o.table[currentIndex].Deleted {
			break
		}
	}

	return ""
}

// Display - отображение таблицы
func (o *OpenHashTable) Display() {
	fmt.Printf("\n=== ХЕШ-ТАБЛИЦА С ОТКРЫТОЙ АДРЕСАЦИЕЙ (%d ячеек) ===\n", o.capacity)

	isEmpty := true
	for i := 0; i < o.capacity; i++ {
		if o.table[i].Used {
			isEmpty = false
			fmt.Printf("[%d]: %s=%s", i, o.table[i].Key, o.table[i].Value)
			if o.table[i].Deleted {
				fmt.Printf(" [УДАЛЕНО]")
			}
			fmt.Println()
		} else if o.table[i].Deleted {
			isEmpty = false
			fmt.Printf("[%d]: [УДАЛЕНО]\n", i)
		}
	}

	if isEmpty {
		fmt.Println("Таблица пуста")
	}
}

// Clear - очистка таблицы
func (o *OpenHashTable) Clear() {
	for i := 0; i < o.capacity; i++ {
		o.table[i].Used = false
		o.table[i].Deleted = false
		o.table[i].Key = ""
		o.table[i].Value = ""
	}
	fmt.Println("Таблица с открытой адресацией очищена")
}

// Resize - изменение размера
func (o *OpenHashTable) Resize(newSize int) {
	if newSize < 1 {
		fmt.Println("Некорректный размер")
		return
	}

	oldTable := o.table
	oldCapacity := o.capacity

	o.capacity = newSize
	o.table = make([]OpenHashEntry, newSize)
	for i := 0; i < newSize; i++ {
		o.table[i] = NewOpenHashEntry()
	}

	// Перехеширование
	for i := 0; i < oldCapacity; i++ {
		if oldTable[i].Used && !oldTable[i].Deleted {
			key := oldTable[i].Key
			value := oldTable[i].Value
			index := o.hashFunction(key)

			for attempt := 0; attempt < o.capacity; attempt++ {
				currentIndex := o.probeFunction(index, attempt)
				if !o.table[currentIndex].Used || o.table[currentIndex].Deleted {
					o.table[currentIndex].Key = key
					o.table[currentIndex].Value = value
					o.table[currentIndex].Used = true
					o.table[currentIndex].Deleted = false
					break
				}
			}
		}
	}

	fmt.Printf("Таблица изменена до %d ячеек\n", o.capacity)
}

// GetSize - получение количества элементов
func (o *OpenHashTable) GetSize() int {
	count := 0
	for i := 0; i < o.capacity; i++ {
		if o.table[i].Used {
			count++
		}
	}
	return count
}

// GetCapacity - получение емкости
func (o *OpenHashTable) GetCapacity() int {
	return o.capacity
}

// GetLoadFactor - получение коэффициента заполнения
func (o *OpenHashTable) GetLoadFactor() float64 {
	size := o.GetSize()
	if size == 0 {
		return 0.0
	}
	return float64(size) / float64(o.capacity)
}

// SaveToFile - сохранение в текстовый файл
func (o *OpenHashTable) SaveToFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	_, err = fmt.Fprintf(file, "OPEN_HASH_TABLE\n")
	if err != nil {
		return err
	}
	_, err = fmt.Fprintf(file, "%d\n", o.capacity)
	if err != nil {
		return err
	}

	for i := 0; i < o.capacity; i++ {
		if o.table[i].Used {
			_, err = fmt.Fprintf(file, "%s %s\n", o.table[i].Key, o.table[i].Value)
			if err != nil {
				return err
			}
		}
	}

	fmt.Printf("Таблица с открытой адресацией сохранена в %s\n", filename)
	return nil
}

// LoadFromFile - загрузка из текстового файла
func (o *OpenHashTable) LoadFromFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)

	if !scanner.Scan() {
		return fmt.Errorf("файл пуст")
	}
	typeStr := scanner.Text()
	if typeStr != "OPEN_HASH_TABLE" {
		fmt.Println("Неверный формат файла")
		return fmt.Errorf("неверный формат файла")
	}

	if !scanner.Scan() {
		return fmt.Errorf("файл поврежден")
	}
	fileCapacity, err := strconv.Atoi(scanner.Text())
	if err != nil {
		return err
	}

	o.Clear()
	if fileCapacity != o.capacity {
		o.table = make([]OpenHashEntry, fileCapacity)
		o.capacity = fileCapacity
		for i := 0; i < o.capacity; i++ {
			o.table[i] = NewOpenHashEntry()
		}
	}

	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.SplitN(line, " ", 2)
		if len(parts) == 2 {
			o.Insert(parts[0], parts[1])
		}
	}

	fmt.Printf("Таблица с открытой адресацией загружена из %s\n", filename)
	return scanner.Err()
}

// SaveToBinaryFile - сохранение в бинарный файл (OpenHashTable)
func (o *OpenHashTable) SaveToBinaryFile(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для записи: %s\n", filename)
		return err
	}
	defer file.Close()

	// Сигнатура - ровно 16 байт
	signature := []byte("OPEN_HASH_BIN_16")
	_, err = file.Write(signature)
	if err != nil {
		return err
	}

	// Запись capacity
	err = binary.Write(file, binary.LittleEndian, int32(o.capacity))
	if err != nil {
		return err
	}

	// Количество активных элементов
	numElements := int32(0)
	for i := 0; i < o.capacity; i++ {
		if o.table[i].Used && !o.table[i].Deleted {
			numElements++
		}
	}
	err = binary.Write(file, binary.LittleEndian, numElements)
	if err != nil {
		return err
	}

	// Запись элементов
	for i := 0; i < o.capacity; i++ {
		if o.table[i].Used && !o.table[i].Deleted {
			keyLen := int32(len(o.table[i].Key))
			valueLen := int32(len(o.table[i].Value))

			err = binary.Write(file, binary.LittleEndian, keyLen)
			if err != nil {
				return err
			}
			_, err = file.Write([]byte(o.table[i].Key))
			if err != nil {
				return err
			}

			err = binary.Write(file, binary.LittleEndian, valueLen)
			if err != nil {
				return err
			}
			_, err = file.Write([]byte(o.table[i].Value))
			if err != nil {
				return err
			}
		}
	}

	fmt.Printf("Таблица с открытой адресацией сохранена в бинарный файл: %s\n", filename)
	return nil
}

// LoadFromBinaryFile - загрузка из бинарного файла (OpenHashTable)
func (o *OpenHashTable) LoadFromBinaryFile(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		fmt.Printf("Ошибка открытия бинарного файла для чтения: %s\n", filename)
		return err
	}
	defer file.Close()

	// Чтение сигнатуры
	signature := make([]byte, 16)
	_, err = file.Read(signature)
	if err != nil {
		return err
	}
	if string(signature) != "OPEN_HASH_BIN_16" {
		fmt.Println("Неверный формат файла")
		return fmt.Errorf("неверный формат файла")
	}

	var fileCapacity int32
	err = binary.Read(file, binary.LittleEndian, &fileCapacity)
	if err != nil {
		return err
	}

	o.Clear()
	o.capacity = int(fileCapacity)
	o.table = make([]OpenHashEntry, o.capacity)
	for i := 0; i < o.capacity; i++ {
		o.table[i] = NewOpenHashEntry()
	}

	var numElements int32
	err = binary.Read(file, binary.LittleEndian, &numElements)
	if err != nil {
		return err
	}

	for i := 0; i < int(numElements); i++ {
		var keyLen int32
		err = binary.Read(file, binary.LittleEndian, &keyLen)
		if err != nil {
			return err
		}

		keyBuf := make([]byte, keyLen)
		_, err = file.Read(keyBuf)
		if err != nil {
			return err
		}
		key := string(keyBuf)

		var valueLen int32
		err = binary.Read(file, binary.LittleEndian, &valueLen)
		if err != nil {
			return err
		}

		valueBuf := make([]byte, valueLen)
		_, err = file.Read(valueBuf)
		if err != nil {
			return err
		}
		value := string(valueBuf)

		inserted := false
		for attempt := 0; attempt < o.capacity; attempt++ {
			index := o.probeFunction(o.hashFunction(key), attempt)
			if !o.table[index].Used || o.table[index].Deleted {
				o.table[index].Key = key
				o.table[index].Value = value
				o.table[index].Used = true
				o.table[index].Deleted = false
				inserted = true
				break
			}
		}

		if !inserted {
			fmt.Printf("Предупреждение: не удалось вставить элемент %s при загрузке\n", key)
		}
	}

	fmt.Printf("Таблица с открытой адресацией загружена из бинарного файла: %s\n", filename)
	return nil
}

// ==================== Вспомогательные функции ====================

// ParseCommand - парсинг команд
func ParseCommand(line string) (cmd, arg1, arg2 string) {
	parts := strings.SplitN(line, " ", 3)
	if len(parts) > 0 {
		cmd = parts[0]
	}
	if len(parts) > 1 {
		arg1 = parts[1]
	}
	if len(parts) > 2 {
		arg2 = parts[2]
	}
	return
}

// DisplayMenu - отображение меню
func DisplayMenu() {
	fmt.Println("\n=== МЕНЮ ХЕШ-ТАБЛИЦ ===")
	fmt.Println("1. Добавить элемент")
	fmt.Println("2. Найти элемент")
	fmt.Println("3. Удалить элемент")
	fmt.Println("4. Показать таблицы")
	fmt.Println("5. Очистить таблицы")
	fmt.Println("6. Изменить размер таблиц")
	fmt.Println("7. Сохранить в файл")
	fmt.Println("8. Загрузить из файла")
	fmt.Println("9. Статистика")
	fmt.Println("0. Выход")
	fmt.Print("Выберите действие: ")
}
