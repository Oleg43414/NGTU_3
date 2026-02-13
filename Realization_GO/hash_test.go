package main

import (
	"os"
	"testing"
)

func TestChainHashTable_BasicOperations(t *testing.T) {
	table := NewChainHashTable(10)

	if table.GetSize() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", table.GetSize())
	}
	if table.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", table.GetCapacity())
	}

	table.Insert("name", "Ivan")
	if table.GetSize() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", table.GetSize())
	}
	if table.Find("name") != "Ivan" {
		t.Errorf("Ожидалось 'Ivan', получено '%s'", table.Find("name"))
	}

	table.Insert("name", "Petr")
	if table.GetSize() != 1 {
		t.Errorf("После обновления размер должен остаться 1, получен %d", table.GetSize())
	}
	if table.Find("name") != "Petr" {
		t.Errorf("Ожидалось 'Petr', получено '%s'", table.Find("name"))
	}

	if !table.Remove("name") {
		t.Errorf("Удаление должно вернуть true")
	}
	if table.GetSize() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", table.GetSize())
	}
	if table.Find("name") != "" {
		t.Errorf("После удаления элемент не должен находиться")
	}
}

func TestChainHashTable_CollisionHandling(t *testing.T) {
	table := NewChainHashTable(3)

	table.Insert("abc", "value1")
	table.Insert("cba", "value2")
	table.Insert("bac", "value3")
	table.Insert("xyz", "value4")
	table.Insert("zyx", "value5")

	if table.Find("abc") != "value1" {
		t.Errorf("Ожидалось 'value1', получено '%s'", table.Find("abc"))
	}
	if table.Find("cba") != "value2" {
		t.Errorf("Ожидалось 'value2', получено '%s'", table.Find("cba"))
	}
	if table.Find("bac") != "value3" {
		t.Errorf("Ожидалось 'value3', получено '%s'", table.Find("bac"))
	}
	if table.Find("xyz") != "value4" {
		t.Errorf("Ожидалось 'value4', получено '%s'", table.Find("xyz"))
	}
	if table.Find("zyx") != "value5" {
		t.Errorf("Ожидалось 'value5', получено '%s'", table.Find("zyx"))
	}
	if table.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", table.GetSize())
	}

	// Тест длинной цепочки
	table = NewChainHashTable(1)
	table.Insert("a", "1")
	table.Insert("b", "2")
	table.Insert("c", "3")
	table.Insert("d", "4")
	table.Insert("e", "5")
	if table.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", table.GetSize())
	}
	if table.Find("a") != "1" || table.Find("e") != "5" {
		t.Errorf("Элементы в длинной цепочке должны быть доступны")
	}
}

func TestChainHashTable_Clear(t *testing.T) {
	table := NewChainHashTable(5)

	// Очистка пустой таблицы
	table.Clear()
	if table.GetSize() != 0 {
		t.Errorf("После очистки пустой таблицы размер должен быть 0, получен %d", table.GetSize())
	}

	// Очистка непустой таблицы
	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	table.Insert("key3", "value3")
	table.Clear()

	if table.GetSize() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", table.GetSize())
	}

	// Проверка работы после очистки
	table.Insert("newkey", "newvalue")
	if table.GetSize() != 1 {
		t.Errorf("После очистки должна работать вставка, получен размер %d", table.GetSize())
	}
	if table.Find("newkey") != "newvalue" {
		t.Errorf("После очистки должен работать поиск")
	}
}

func TestChainHashTable_Resize(t *testing.T) {
	table := NewChainHashTable(5)

	// Увеличение размера
	for i := 0; i < 10; i++ {
		table.Insert("key"+string(rune('0'+i)), "value"+string(rune('0'+i)))
	}

	table.Resize(20)
	if table.GetCapacity() != 20 {
		t.Errorf("Ожидалась емкость 20, получена %d", table.GetCapacity())
	}

	// Проверка сохранения данных
	for i := 0; i < 10; i++ {
		key := "key" + string(rune('0'+i))
		expected := "value" + string(rune('0'+i))
		if table.Find(key) != expected {
			t.Errorf("После ресайза ожидалось '%s', получено '%s'", expected, table.Find(key))
		}
	}

	// Уменьшение размера
	table.Resize(15)
	if table.GetCapacity() != 15 {
		t.Errorf("Ожидалась емкость 15, получена %d", table.GetCapacity())
	}

	// Ресайз с тем же размером
	table.Resize(15)
	if table.GetCapacity() != 15 {
		t.Errorf("Емкость не должна измениться")
	}

	// Некорректные размеры
	table.Resize(0)
	table.Resize(-5)

	// Ресайз таблицы с 0 capacity
	emptyTable := NewChainHashTable(0)
	emptyTable.Resize(10)
	if emptyTable.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", emptyTable.GetCapacity())
	}
}

func TestChainHashTable_Display(t *testing.T) {
	table := NewChainHashTable(3)

	// Пустая таблица
	table.Display()

	// Таблица с элементами
	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	table.Display()

	// Таблица с коллизиями
	table.Insert("abc", "collision1")
	table.Insert("cba", "collision2")
	table.Display()

	// После удаления
	table.Remove("key1")
	table.Display()
}

func TestChainHashTable_TextFileOperations(t *testing.T) {
	table := NewChainHashTable(10)

	table.Insert("user1", "Alice")
	table.Insert("user2", "Bob")
	table.Insert("user3", "Charlie")

	txtFile := "test_chain.txt"
	err := table.SaveToFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	table2 := NewChainHashTable(10)
	err = table2.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if table2.Find("user1") != "Alice" {
		t.Errorf("Ожидалось 'Alice', получено '%s'", table2.Find("user1"))
	}
	if table2.Find("user2") != "Bob" {
		t.Errorf("Ожидалось 'Bob', получено '%s'", table2.Find("user2"))
	}
	if table2.Find("user3") != "Charlie" {
		t.Errorf("Ожидалось 'Charlie', получено '%s'", table2.Find("user3"))
	}

	// Загрузка с изменением capacity
	table3 := NewChainHashTable(5)
	err = table3.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки с изменением capacity: %v", err)
	}
	if table3.GetCapacity() != 10 {
		t.Errorf("Capacity должно стать 10, получено %d", table3.GetCapacity())
	}

	// Ошибки загрузки
	err = table.LoadFromFile("nonexistent_file.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	os.Remove(txtFile)
}

func TestChainHashTable_BinaryFileOperations(t *testing.T) {
	table := NewChainHashTable(5)

	table.Insert("user1", "Alice")
	table.Insert("user2", "Bob")
	table.Insert("user3", "Charlie")
	table.Insert("user4", "David")
	table.Insert("user5", "Eve")

	binFile := "test_chain.bin"
	err := table.SaveToBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	table2 := NewChainHashTable(5)
	err = table2.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if table2.Find("user1") != "Alice" {
		t.Errorf("Ожидалось 'Alice', получено '%s'", table2.Find("user1"))
	}
	if table2.Find("user2") != "Bob" {
		t.Errorf("Ожидалось 'Bob', получено '%s'", table2.Find("user2"))
	}
	if table2.Find("user3") != "Charlie" {
		t.Errorf("Ожидалось 'Charlie', получено '%s'", table2.Find("user3"))
	}
	if table2.Find("user4") != "David" {
		t.Errorf("Ожидалось 'David', получено '%s'", table2.Find("user4"))
	}
	if table2.Find("user5") != "Eve" {
		t.Errorf("Ожидалось 'Eve', получено '%s'", table2.Find("user5"))
	}

	// Загрузка с изменением capacity
	table3 := NewChainHashTable(10)
	err = table3.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки с изменением capacity: %v", err)
	}
	if table3.GetCapacity() != 5 {
		t.Errorf("Capacity должно стать 5, получено %d", table3.GetCapacity())
	}

	os.Remove(binFile)
}

func TestChainHashTable_BinaryFileErrors(t *testing.T) {
	table := NewChainHashTable(5)

	// 1. Несуществующий файл
	err := table.LoadFromBinaryFile("nonexistent_chain.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	// 2. Неправильная сигнатура
	badFile := "bad_chain_sig.bin"
	f, _ := os.Create(badFile)
	f.Write([]byte("WRONG_SIGNATURE_16"))
	f.Close()

	err = table.LoadFromBinaryFile(badFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при неправильной сигнатуре")
	}
	os.Remove(badFile)

	// 3. Пустой файл
	emptyFile := "empty_chain.bin"
	f2, _ := os.Create(emptyFile)
	f2.Close()

	err = table.LoadFromBinaryFile(emptyFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при пустом файле")
	}
	os.Remove(emptyFile)

	// 4. Файл с правильной сигнатурой но недостаточными данными
	corruptFile := "corrupt_chain.bin"
	f3, _ := os.Create(corruptFile)
	f3.Write([]byte("CHAIN_HASH_BIN_16"))
	f3.Close()

	err = table.LoadFromBinaryFile(corruptFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при поврежденном файле")
	}
	os.Remove(corruptFile)
}

func TestChainHashTable_SaveToBinaryFileError(t *testing.T) {
	table := NewChainHashTable(5)
	table.Insert("key", "value")

	// Попытка сохранения в недоступную директорию
	err := table.SaveToBinaryFile("/nonexistent/path/file.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при сохранении в недоступную директорию")
	}
}

func TestChainHashTable_EdgeCases(t *testing.T) {
	table := NewChainHashTable(10)

	// Поиск несуществующего элемента
	if table.Find("nonexistent") != "" {
		t.Errorf("Поиск несуществующего ключа должен вернуть пустую строку")
	}

	// Удаление несуществующего элемента
	if table.Remove("nonexistent") {
		t.Errorf("Удаление несуществующего ключа должно вернуть false")
	}

	// Пустые строки
	table.Insert("", "empty_key")
	table.Insert("empty_value", "")
	if table.Find("") != "empty_key" || table.Find("empty_value") != "" {
		t.Errorf("Пустые строки должны корректно обрабатываться")
	}

	// Длинные строки
	longKey := string(make([]byte, 1000))
	longValue := string(make([]byte, 1000))
	table.Insert(longKey, longValue)
	if table.Find(longKey) != longValue {
		t.Errorf("Длинные строки должны корректно обрабатываться")
	}
}

func TestChainHashTable_EmptyTableFileOperations(t *testing.T) {
	table := NewChainHashTable(10)

	txtFile := "empty_chain.txt"
	binFile := "empty_chain.bin"

	err := table.SaveToFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустой таблицы: %v", err)
	}
	err = table.SaveToBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустой таблицы в бинарный файл: %v", err)
	}

	table2 := NewChainHashTable(10)
	err = table2.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустой таблицы: %v", err)
	}
	if table2.GetSize() != 0 {
		t.Errorf("Загруженная таблица должна быть пустой, получен размер %d", table2.GetSize())
	}

	table3 := NewChainHashTable(10)
	err = table3.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустой таблицы из бинарного файла: %v", err)
	}
	if table3.GetSize() != 0 {
		t.Errorf("Загруженная таблица должна быть пустой, получен размер %d", table3.GetSize())
	}

	os.Remove(txtFile)
	os.Remove(binFile)
}

func TestChainHashTable_GetSize(t *testing.T) {
	table := NewChainHashTable(10)

	if table.GetSize() != 0 {
		t.Errorf("Размер пустой таблицы должен быть 0, получен %d", table.GetSize())
	}

	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	table.Insert("key3", "value3")
	if table.GetSize() != 3 {
		t.Errorf("Размер должен быть 3, получен %d", table.GetSize())
	}

	table.Insert("key1", "newvalue") // обновление
	if table.GetSize() != 3 {
		t.Errorf("После обновления размер должен остаться 3, получен %d", table.GetSize())
	}

	table.Remove("key2")
	if table.GetSize() != 2 {
		t.Errorf("После удаления размер должен быть 2, получен %d", table.GetSize())
	}

	table.Clear()
	if table.GetSize() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", table.GetSize())
	}
}

// ==================== OpenHashTable Tests ====================

func TestOpenHashTable_BasicOperations(t *testing.T) {
	table := NewOpenHashTable(10)

	if table.GetSize() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", table.GetSize())
	}
	if table.GetLoadFactor() != 0.0 {
		t.Errorf("Ожидался load factor 0.0, получен %f", table.GetLoadFactor())
	}

	table.Insert("key1", "value1")
	if table.GetSize() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", table.GetSize())
	}
	if table.GetLoadFactor() <= 0.0 {
		t.Errorf("Load factor должен быть > 0.0, получен %f", table.GetLoadFactor())
	}
	if table.Find("key1") != "value1" {
		t.Errorf("Ожидалось 'value1', получено '%s'", table.Find("key1"))
	}

	table.Insert("key1", "value2")
	if table.GetSize() != 1 {
		t.Errorf("После обновления размер должен остаться 1, получен %d", table.GetSize())
	}
	if table.Find("key1") != "value2" {
		t.Errorf("Ожидалось 'value2', получено '%s'", table.Find("key1"))
	}

	if !table.Remove("key1") {
		t.Errorf("Удаление должно вернуть true")
	}
	if table.GetSize() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", table.GetSize())
	}
	if table.Find("key1") != "" {
		t.Errorf("После удаления элемент не должен находиться")
	}
}

func TestOpenHashTable_CollisionsAndFullTable(t *testing.T) {
	table := NewOpenHashTable(3)

	table.Insert("a", "1")
	table.Insert("b", "2")
	table.Insert("c", "3")

	if table.GetSize() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", table.GetSize())
	}
	if table.GetLoadFactor() != 1.0 {
		t.Errorf("Ожидался load factor 1.0, получен %f", table.GetLoadFactor())
	}

	// Попытка вставки в полную таблицу
	table.Insert("d", "4")
	if table.GetSize() != 3 {
		t.Errorf("После попытки вставки в полную таблицу размер должен остаться 3, получен %d", table.GetSize())
	}

	// Удаление и вставка в освободившееся место
	table.Remove("b")
	table.Insert("d", "4")
	if table.GetSize() != 3 {
		t.Errorf("Размер должен быть 3, получен %d", table.GetSize())
	}
	if table.Find("d") != "4" {
		t.Errorf("Элемент d должен быть добавлен")
	}
}

func TestOpenHashTable_Clear(t *testing.T) {
	table := NewOpenHashTable(10)

	// Очистка пустой таблицы
	table.Clear()
	if table.GetSize() != 0 {
		t.Errorf("После очистки пустой таблицы размер должен быть 0, получен %d", table.GetSize())
	}

	// Очистка непустой таблицы
	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	table.Insert("key3", "value3")
	table.Clear()

	if table.GetSize() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", table.GetSize())
	}

	// Проверка работы после очистки
	table.Insert("newkey", "newvalue")
	if table.GetSize() != 1 {
		t.Errorf("После очистки должна работать вставка, получен размер %d", table.GetSize())
	}
	if table.Find("newkey") != "newvalue" {
		t.Errorf("После очистки должен работать поиск")
	}
}

func TestOpenHashTable_Display(t *testing.T) {
	table := NewOpenHashTable(5)

	table.Display() // пустая таблица

	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	table.Display()

	table.Remove("key1")
	table.Display() // с deleted флагом

	table.Insert("key3", "value3")
	table.Display()
}

func TestOpenHashTable_TextFileOperations(t *testing.T) {
	table := NewOpenHashTable(10)

	table.Insert("data1", "value1")
	table.Insert("data2", "value2")
	table.Insert("data3", "value3")
	table.Insert("data4", "value4")

	txtFile := "test_open.txt"
	err := table.SaveToFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	table2 := NewOpenHashTable(10)
	err = table2.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if table2.Find("data1") != "value1" {
		t.Errorf("Ожидалось 'value1', получено '%s'", table2.Find("data1"))
	}
	if table2.Find("data2") != "value2" {
		t.Errorf("Ожидалось 'value2', получено '%s'", table2.Find("data2"))
	}
	if table2.Find("data3") != "value3" {
		t.Errorf("Ожидалось 'value3', получено '%s'", table2.Find("data3"))
	}
	if table2.Find("data4") != "value4" {
		t.Errorf("Ожидалось 'value4', получено '%s'", table2.Find("data4"))
	}

	// Загрузка с изменением capacity
	table3 := NewOpenHashTable(5)
	err = table3.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки с изменением capacity: %v", err)
	}
	if table3.GetCapacity() != 10 {
		t.Errorf("Capacity должно стать 10, получено %d", table3.GetCapacity())
	}

	// Ошибки загрузки
	err = table.LoadFromFile("nonexistent_file.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	os.Remove(txtFile)
}

func TestOpenHashTable_BinaryFileOperations(t *testing.T) {
	table := NewOpenHashTable(5)

	table.Insert("data1", "value1")
	table.Insert("data2", "value2")
	table.Insert("data3", "value3")
	table.Insert("data4", "value4")
	table.Insert("data5", "value5")

	binFile := "test_open.bin"
	err := table.SaveToBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	table2 := NewOpenHashTable(5)
	err = table2.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if table2.Find("data1") != "value1" {
		t.Errorf("Ожидалось 'value1', получено '%s'", table2.Find("data1"))
	}
	if table2.Find("data2") != "value2" {
		t.Errorf("Ожидалось 'value2', получено '%s'", table2.Find("data2"))
	}
	if table2.Find("data3") != "value3" {
		t.Errorf("Ожидалось 'value3', получено '%s'", table2.Find("data3"))
	}
	if table2.Find("data4") != "value4" {
		t.Errorf("Ожидалось 'value4', получено '%s'", table2.Find("data4"))
	}
	if table2.Find("data5") != "value5" {
		t.Errorf("Ожидалось 'value5', получено '%s'", table2.Find("data5"))
	}

	// Загрузка с изменением capacity
	table3 := NewOpenHashTable(10)
	err = table3.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки с изменением capacity: %v", err)
	}
	if table3.GetCapacity() != 5 {
		t.Errorf("Capacity должно стать 5, получено %d", table3.GetCapacity())
	}

	os.Remove(binFile)
}

func TestOpenHashTable_BinaryFileErrors(t *testing.T) {
	table := NewOpenHashTable(5)

	// 1. Несуществующий файл
	err := table.LoadFromBinaryFile("nonexistent_open.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	// 2. Неправильная сигнатура
	badFile := "bad_open_sig.bin"
	f, _ := os.Create(badFile)
	f.Write([]byte("WRONG_SIGNATURE_16"))
	f.Close()

	err = table.LoadFromBinaryFile(badFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при неправильной сигнатуре")
	}
	os.Remove(badFile)

	// 3. Пустой файл
	emptyFile := "empty_open.bin"
	f2, _ := os.Create(emptyFile)
	f2.Close()

	err = table.LoadFromBinaryFile(emptyFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при пустом файле")
	}
	os.Remove(emptyFile)

	// 4. Файл с правильной сигнатурой но недостаточными данными
	corruptFile := "corrupt_open.bin"
	f3, _ := os.Create(corruptFile)
	f3.Write([]byte("OPEN_HASH_BIN_16"))
	f3.Close()

	err = table.LoadFromBinaryFile(corruptFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при поврежденном файле")
	}
	os.Remove(corruptFile)
}

func TestOpenHashTable_SaveToBinaryFileError(t *testing.T) {
	table := NewOpenHashTable(5)
	table.Insert("key", "value")

	// Попытка сохранения в недоступную директорию
	err := table.SaveToBinaryFile("/nonexistent/path/file.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при сохранении в недоступную директорию")
	}
}

func TestOpenHashTable_Resize(t *testing.T) {
	table := NewOpenHashTable(5)

	if table.GetCapacity() != 5 {
		t.Errorf("Ожидалась емкость 5, получена %d", table.GetCapacity())
	}

	for i := 0; i < 4; i++ {
		table.Insert("key"+string(rune('0'+i)), "value"+string(rune('0'+i)))
	}

	table.Resize(10)
	if table.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", table.GetCapacity())
	}

	for i := 0; i < 4; i++ {
		key := "key" + string(rune('0'+i))
		expected := "value" + string(rune('0'+i))
		if table.Find(key) != expected {
			t.Errorf("После ресайза ожидалось '%s', получено '%s'", expected, table.Find(key))
		}
	}

	table.Resize(8)
	if table.GetCapacity() != 8 {
		t.Errorf("Ожидалась емкость 8, получена %d", table.GetCapacity())
	}

	// Ресайз с тем же размером
	table.Resize(8)
	if table.GetCapacity() != 8 {
		t.Errorf("Емкость не должна измениться")
	}

	// Некорректные размеры
	table.Resize(0)
	table.Resize(-5)

	// Ресайз таблицы с 0 capacity
	emptyTable := NewOpenHashTable(0)
	emptyTable.Resize(10)
	if emptyTable.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", emptyTable.GetCapacity())
	}
}

func TestOpenHashTable_DeletedEntries(t *testing.T) {
	table := NewOpenHashTable(10)

	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	table.Insert("key3", "value3")

	table.Remove("key1")
	table.Remove("key2")
	table.Insert("key4", "value4")
	table.Insert("key5", "value5")
	table.Insert("key6", "value6")

	if table.Find("key3") != "value3" {
		t.Errorf("Ожидалось 'value3', получено '%s'", table.Find("key3"))
	}
	if table.Find("key4") != "value4" {
		t.Errorf("Ожидалось 'value4', получено '%s'", table.Find("key4"))
	}
	if table.Find("key5") != "value5" {
		t.Errorf("Ожидалось 'value5', получено '%s'", table.Find("key5"))
	}
	if table.Find("key6") != "value6" {
		t.Errorf("Ожидалось 'value6', получено '%s'", table.Find("key6"))
	}
	if table.Find("key1") != "" {
		t.Errorf("Удаленный ключ key1 не должен находиться")
	}
	if table.Find("key2") != "" {
		t.Errorf("Удаленный ключ key2 не должен находиться")
	}

	if table.Remove("key1") {
		t.Errorf("Повторное удаление должно вернуть false")
	}
	if table.Remove("nonexistent") {
		t.Errorf("Удаление несуществующего должно вернуть false")
	}
}

func TestOpenHashTable_EmptyTableFileOperations(t *testing.T) {
	table := NewOpenHashTable(10)

	txtFile := "empty_open.txt"
	binFile := "empty_open.bin"

	err := table.SaveToFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустой таблицы: %v", err)
	}
	err = table.SaveToBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустой таблицы в бинарный файл: %v", err)
	}

	table2 := NewOpenHashTable(10)
	err = table2.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустой таблицы: %v", err)
	}
	if table2.GetSize() != 0 {
		t.Errorf("Загруженная таблица должна быть пустой, получен размер %d", table2.GetSize())
	}

	table3 := NewOpenHashTable(10)
	err = table3.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустой таблицы из бинарного файла: %v", err)
	}
	if table3.GetSize() != 0 {
		t.Errorf("Загруженная таблица должна быть пустой, получен размер %d", table3.GetSize())
	}

	os.Remove(txtFile)
	os.Remove(binFile)
}

func TestOpenHashTable_GetLoadFactor(t *testing.T) {
	table := NewOpenHashTable(10)

	if table.GetLoadFactor() != 0.0 {
		t.Errorf("Load factor пустой таблицы должен быть 0.0, получен %f", table.GetLoadFactor())
	}

	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	if table.GetLoadFactor() != 0.2 {
		t.Errorf("Load factor должен быть 0.2, получен %f", table.GetLoadFactor())
	}

	table.Remove("key1")
	if table.GetLoadFactor() != 0.1 {
		t.Errorf("Load factor должен быть 0.1, получен %f", table.GetLoadFactor())
	}

	table = NewOpenHashTable(3)
	table.Insert("a", "1")
	table.Insert("b", "2")
	table.Insert("c", "3")
	if table.GetLoadFactor() != 1.0 {
		t.Errorf("Load factor должен быть 1.0, получен %f", table.GetLoadFactor())
	}
}

func TestOpenHashTable_GetSize(t *testing.T) {
	table := NewOpenHashTable(10)

	if table.GetSize() != 0 {
		t.Errorf("Размер пустой таблицы должен быть 0, получен %d", table.GetSize())
	}

	table.Insert("key1", "value1")
	table.Insert("key2", "value2")
	table.Insert("key3", "value3")
	if table.GetSize() != 3 {
		t.Errorf("Размер должен быть 3, получен %d", table.GetSize())
	}

	table.Insert("key1", "newvalue") // обновление
	if table.GetSize() != 3 {
		t.Errorf("После обновления размер должен остаться 3, получен %d", table.GetSize())
	}

	table.Remove("key2")
	if table.GetSize() != 2 {
		t.Errorf("После удаления размер должен быть 2, получен %d", table.GetSize())
	}

	table.Clear()
	if table.GetSize() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", table.GetSize())
	}
}

func TestOpenHashTable_EdgeCases(t *testing.T) {
	table := NewOpenHashTable(10)

	table.Insert("", "empty_key")
	table.Insert("empty_value", "")
	if table.Find("") != "empty_key" || table.Find("empty_value") != "" {
		t.Errorf("Пустые строки должны корректно обрабатываться")
	}

	longKey := string(make([]byte, 1000))
	longValue := string(make([]byte, 1000))
	table.Insert(longKey, longValue)
	if table.Find(longKey) != longValue {
		t.Errorf("Длинные строки должны корректно обрабатываться")
	}
}

// ==================== Helper Functions Tests ====================

func TestStringHashFunction(t *testing.T) {
	hash1 := StringHash("test", 100)
	hash2 := StringHash("test", 100)
	if hash1 != hash2 {
		t.Errorf("Одинаковые ключи должны давать одинаковый хеш")
	}

	hash3 := StringHash("test", 10)
	if hash3 < 0 || hash3 >= 10 {
		t.Errorf("Хеш должен быть в диапазоне [0, 10), получен %d", hash3)
	}

	hash4 := StringHash("", 100)
	if hash4 < 0 || hash4 >= 100 {
		t.Errorf("Хеш пустой строки должен быть в диапазоне [0, 100), получен %d", hash4)
	}

	longStr := make([]byte, 1000)
	for i := 0; i < 1000; i++ {
		longStr[i] = 'a'
	}
	hash5 := StringHash(string(longStr), 100)
	if hash5 < 0 || hash5 >= 100 {
		t.Errorf("Хеш длинной строки должен быть в диапазоне [0, 100), получен %d", hash5)
	}

	hash6 := StringHash("test", 0)
	if hash6 != 0 {
		t.Errorf("При mod=0 должен возвращаться 0, получен %d", hash6)
	}

	hash7 := StringHash("!@#$%^&*()", 50)
	if hash7 < 0 || hash7 >= 50 {
		t.Errorf("Хеш спецсимволов должен быть в диапазоне [0, 50), получен %d", hash7)
	}

	hash8 := StringHash("привет мир", 100)
	if hash8 < 0 || hash8 >= 100 {
		t.Errorf("Хеш Unicode строки должен быть в диапазоне [0, 100), получен %d", hash8)
	}
}

func TestHashTable_Constructors(t *testing.T) {
	table1 := NewChainHashTable(5)
	if table1.GetCapacity() != 5 {
		t.Errorf("Ожидалась емкость 5, получена %d", table1.GetCapacity())
	}
	if table1.GetSize() != 0 {
		t.Errorf("Новая таблица должна быть пустой, получен размер %d", table1.GetSize())
	}

	table2 := NewChainHashTable(0)
	if table2.GetCapacity() != 1 {
		t.Errorf("При capacity=0 ожидалась емкость 1, получена %d", table2.GetCapacity())
	}

	table3 := NewChainHashTable(-5)
	if table3.GetCapacity() != 1 {
		t.Errorf("При отрицательном capacity ожидалась емкость 1, получена %d", table3.GetCapacity())
	}

	openTable1 := NewOpenHashTable(5)
	if openTable1.GetCapacity() != 5 {
		t.Errorf("Ожидалась емкость 5, получена %d", openTable1.GetCapacity())
	}
	if openTable1.GetSize() != 0 {
		t.Errorf("Новая таблица должна быть пустой, получен размер %d", openTable1.GetSize())
	}
	if openTable1.GetLoadFactor() != 0.0 {
		t.Errorf("Load factor новой таблицы должен быть 0.0, получен %f", openTable1.GetLoadFactor())
	}

	openTable2 := NewOpenHashTable(0)
	if openTable2.GetCapacity() != 1 {
		t.Errorf("При capacity=0 ожидалась емкость 1, получена %d", openTable2.GetCapacity())
	}

	openTable3 := NewOpenHashTable(-5)
	if openTable3.GetCapacity() != 1 {
		t.Errorf("При отрицательном capacity ожидалась емкость 1, получена %d", openTable3.GetCapacity())
	}
}

func TestHashTable_RealWorldScenarios(t *testing.T) {
	// Тест словаря
	dictionary := NewChainHashTable(50)
	dictionary.Insert("apple", "яблоко")
	dictionary.Insert("book", "книга")
	dictionary.Insert("computer", "компьютер")
	dictionary.Insert("house", "дом")
	dictionary.Insert("car", "машина")

	if dictionary.Find("apple") != "яблоко" {
		t.Errorf("Ожидалось 'яблоко', получено '%s'", dictionary.Find("apple"))
	}
	if dictionary.Find("book") != "книга" {
		t.Errorf("Ожидалось 'книга', получено '%s'", dictionary.Find("book"))
	}
	if dictionary.Find("computer") != "компьютер" {
		t.Errorf("Ожидалось 'компьютер', получено '%s'", dictionary.Find("computer"))
	}
	if dictionary.Find("house") != "дом" {
		t.Errorf("Ожидалось 'дом', получено '%s'", dictionary.Find("house"))
	}
	if dictionary.Find("car") != "машина" {
		t.Errorf("Ожидалось 'машина', получено '%s'", dictionary.Find("car"))
	}

	dictionary.Insert("apple", "яблоко (фрукт)")
	if dictionary.Find("apple") != "яблоко (фрукт)" {
		t.Errorf("После обновления ожидалось 'яблоко (фрукт)', получено '%s'", dictionary.Find("apple"))
	}

	dictionary.Remove("book")
	if dictionary.Find("book") != "" {
		t.Errorf("После удаления книга не должна быть в словаре")
	}

	dictionary.Display()

	// Тест кэша
	cache := NewOpenHashTable(20)
	cache.Insert("user_123_profile", "{name: 'John', age: 30}")
	cache.Insert("user_456_profile", "{name: 'Jane', age: 25}")
	cache.Insert("product_789_details", "{price: 99.99, stock: 100}")
	cache.Insert("product_101_details", "{price: 49.99, stock: 50}")

	if cache.Find("user_123_profile") == "" {
		t.Errorf("Профиль пользователя user_123 должен быть в кэше")
	}
	if cache.Find("user_456_profile") == "" {
		t.Errorf("Профиль пользователя user_456 должен быть в кэше")
	}
	if cache.Find("product_789_details") == "" {
		t.Errorf("Детали продукта 789 должны быть в кэше")
	}
	if cache.Find("product_101_details") == "" {
		t.Errorf("Детали продукта 101 должны быть в кэше")
	}

	cache.Remove("product_789_details")
	if cache.Find("product_789_details") != "" {
		t.Errorf("После удаления детали продукта 789 не должно быть в кэше")
	}

	cache.Remove("user_123_profile")
	if cache.Find("user_123_profile") != "" {
		t.Errorf("После удаления профиль пользователя не должен быть в кэше")
	}

	cache.Display()

	// Тест счетчика частоты слов
	counter := NewChainHashTable(26)
	counter.Insert("hello", "1")
	counter.Insert("world", "1")
	counter.Insert("hello", "2")
	counter.Insert("go", "1")
	counter.Insert("programming", "1")
	counter.Insert("test", "1")

	if counter.Find("hello") != "2" {
		t.Errorf("Ожидалось '2', получено '%s'", counter.Find("hello"))
	}
	if counter.Find("world") != "1" {
		t.Errorf("Ожидалось '1', получено '%s'", counter.Find("world"))
	}
	if counter.Find("go") != "1" {
		t.Errorf("Ожидалось '1', получено '%s'", counter.Find("go"))
	}
	if counter.Find("programming") != "1" {
		t.Errorf("Ожидалось '1', получено '%s'", counter.Find("programming"))
	}
	if counter.Find("test") != "1" {
		t.Errorf("Ожидалось '1', получено '%s'", counter.Find("test"))
	}
}

func TestParseCommand(t *testing.T) {
	cmd, arg1, arg2 := ParseCommand("insert key value")
	if cmd != "insert" || arg1 != "key" || arg2 != "value" {
		t.Errorf("Ожидалось insert key value, получено %s %s %s", cmd, arg1, arg2)
	}

	cmd, arg1, arg2 = ParseCommand("find mykey")
	if cmd != "find" || arg1 != "mykey" || arg2 != "" {
		t.Errorf("Ожидалось find mykey, получено %s %s %s", cmd, arg1, arg2)
	}

	cmd, arg1, arg2 = ParseCommand("delete key")
	if cmd != "delete" || arg1 != "key" || arg2 != "" {
		t.Errorf("Ожидалось delete key, получено %s %s %s", cmd, arg1, arg2)
	}

	cmd, arg1, arg2 = ParseCommand("update key newvalue")
	if cmd != "update" || arg1 != "key" || arg2 != "newvalue" {
		t.Errorf("Ожидалось update key newvalue, получено %s %s %s", cmd, arg1, arg2)
	}

	cmd, arg1, arg2 = ParseCommand("exit")
	if cmd != "exit" || arg1 != "" || arg2 != "" {
		t.Errorf("Ожидалось exit, получено %s %s %s", cmd, arg1, arg2)
	}

	cmd, arg1, arg2 = ParseCommand("")
	if cmd != "" || arg1 != "" || arg2 != "" {
		t.Errorf("Пустая строка должна вернуть пустые значения")
	}

	cmd, arg1, arg2 = ParseCommand("   ")
	if cmd != "" || arg1 != "" || arg2 != "" {
		t.Errorf("Строка с пробелами должна вернуть пустые значения")
	}

	cmd, arg1, arg2 = ParseCommand("single")
	if cmd != "single" || arg1 != "" || arg2 != "" {
		t.Errorf("Ожидалось single, получено %s %s %s", cmd, arg1, arg2)
	}

	cmd, arg1, arg2 = ParseCommand("command with multiple words in arg2 that are very long")
	if cmd != "command" || arg1 != "with" || arg2 != "multiple words in arg2 that are very long" {
		t.Errorf("Ошибка парсинга длинной команды")
	}
}

func TestDisplayMenu(t *testing.T) {
	DisplayMenu()
	DisplayMenu()
	DisplayMenu()
}
