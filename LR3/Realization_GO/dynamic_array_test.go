package main

import (
	"os"
	"testing"
)

func TestDynamicArray_Constructor(t *testing.T) {
	// Конструктор с положительной емкостью
	arr := NewDynamicArray(5)
	if arr.GetSize() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 5 {
		t.Errorf("Ожидалась емкость 5, получена %d", arr.GetCapacity())
	}

	// Конструктор с нулевой емкостью (должен стать 10)
	arr2 := NewDynamicArray(0)
	if arr2.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", arr2.GetCapacity())
	}

	// Конструктор с отрицательной емкостью (должен стать 10)
	arr3 := NewDynamicArray(-5)
	if arr3.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", arr3.GetCapacity())
	}

	// Конструктор с емкостью 1
	arr4 := NewDynamicArray(1)
	if arr4.GetCapacity() != 1 {
		t.Errorf("Ожидалась емкость 1, получена %d", arr4.GetCapacity())
	}
}

func TestDynamicArray_PushBack(t *testing.T) {
	arr := NewDynamicArray(5)

	arr.PushBack("Hello")
	arr.PushBack("World")
	arr.PushBack("Go")
	arr.PushBack("Programming")
	arr.PushBack("Data")

	if arr.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 5 {
		t.Errorf("Ожидалась емкость 5, получена %d", arr.GetCapacity())
	}

	// Проверка порядка
	expected := []string{"Hello", "World", "Go", "Programming", "Data"}
	for i, val := range expected {
		if arr.Get(i) != val {
			t.Errorf("На позиции %d ожидался '%s', получен '%s'", i, val, arr.Get(i))
		}
	}

	// Добавление сверх capacity - должен сработать resize
	arr.PushBack("Structures")

	if arr.GetSize() != 6 {
		t.Errorf("Ожидался размер 6, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", arr.GetCapacity())
	}
	if arr.Get(5) != "Structures" {
		t.Errorf("Ожидался 'Structures', получен '%s'", arr.Get(5))
	}
}

func TestDynamicArray_Insert(t *testing.T) {
	arr := NewDynamicArray(5)

	// Вставка в пустой массив
	arr.Insert(0, "First")
	if arr.GetSize() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "First" {
		t.Errorf("Ожидался 'First', получен '%s'", arr.Get(0))
	}

	// Вставка в конец
	arr.Insert(1, "Last")
	if arr.GetSize() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", arr.GetSize())
	}
	if arr.Get(1) != "Last" {
		t.Errorf("Ожидался 'Last', получен '%s'", arr.Get(1))
	}

	// Вставка в середину
	arr.Insert(1, "Middle")
	if arr.GetSize() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "First" {
		t.Errorf("Ожидался 'First', получен '%s'", arr.Get(0))
	}
	if arr.Get(1) != "Middle" {
		t.Errorf("Ожидался 'Middle', получен '%s'", arr.Get(1))
	}
	if arr.Get(2) != "Last" {
		t.Errorf("Ожидался 'Last', получен '%s'", arr.Get(2))
	}

	// Вставка с resize (заполняем до capacity)
	arr = NewDynamicArray(2)
	arr.PushBack("A")
	arr.PushBack("B")
	arr.Insert(1, "C") // Должен сработать resize

	if arr.GetCapacity() != 4 {
		t.Errorf("Ожидалась емкость 4, получена %d", arr.GetCapacity())
	}
	if arr.GetSize() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "A" || arr.Get(1) != "C" || arr.Get(2) != "B" {
		t.Errorf("Порядок элементов нарушен")
	}
}

func TestDynamicArray_InsertInvalidIndex(t *testing.T) {
	arr := NewDynamicArray(5)

	// Вставка с отрицательным индексом
	arr.Insert(-1, "Invalid") // Должно вывести "Неверный индекс!"
	if arr.GetSize() != 0 {
		t.Errorf("Размер не должен измениться, получен %d", arr.GetSize())
	}

	// Вставка с индексом больше размера
	arr.Insert(5, "Invalid") // Должно вывести "Неверный индекс!"
	if arr.GetSize() != 0 {
		t.Errorf("Размер не должен измениться, получен %d", arr.GetSize())
	}

	// Вставка в непустой массив с неверным индексом
	arr.PushBack("A")
	arr.PushBack("B")
	arr.Insert(5, "Invalid") // Должно вывести "Неверный индекс!"
	if arr.GetSize() != 2 {
		t.Errorf("Размер не должен измениться, получен %d", arr.GetSize())
	}
}

func TestDynamicArray_Remove(t *testing.T) {
	arr := NewDynamicArray(10)

	// Удаление из пустого массива
	arr.Remove(0) // Должно вывести "Неверный индекс"
	if arr.GetSize() != 0 {
		t.Errorf("Размер не должен измениться, получен %d", arr.GetSize())
	}

	// Подготовка данных
	arr.PushBack("A")
	arr.PushBack("B")
	arr.PushBack("C")
	arr.PushBack("D")
	arr.PushBack("E")

	// Удаление из начала
	arr.Remove(0)
	if arr.GetSize() != 4 {
		t.Errorf("Ожидался размер 4, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "B" {
		t.Errorf("Ожидался 'B', получен '%s'", arr.Get(0))
	}

	// Удаление из середины
	arr.Remove(1)
	if arr.GetSize() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "B" || arr.Get(1) != "D" || arr.Get(2) != "E" {
		t.Errorf("Порядок элементов нарушен: %v", arr.data[:arr.size])
	}

	// Удаление из конца
	arr.Remove(2)
	if arr.GetSize() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "B" || arr.Get(1) != "D" {
		t.Errorf("Ожидались 'B','D', получены '%s','%s'", arr.Get(0), arr.Get(1))
	}
}

func TestDynamicArray_RemoveWithResize(t *testing.T) {
	// Тест уменьшения capacity
	arr := NewDynamicArray(20)

	// Заполняем массив
	for i := 0; i < 20; i++ {
		arr.PushBack("Element" + string(rune('0'+i)))
	}

	if arr.GetCapacity() != 20 {
		t.Errorf("Ожидалась емкость 20, получена %d", arr.GetCapacity())
	}

	// Удаляем элементы, чтобы size < capacity/4
	for i := 0; i < 16; i++ {
		arr.Remove(0)
	}

	// Должен сработать resize: capacity/4 = 5, size=4, capacity>10 -> уменьшаем
	if arr.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость 10, получена %d", arr.GetCapacity())
	}
	if arr.GetSize() != 4 {
		t.Errorf("Ожидался размер 4, получен %d", arr.GetSize())
	}

	// Проверяем что resize не срабатывает при capacity <= 10
	arr = NewDynamicArray(10)
	for i := 0; i < 10; i++ {
		arr.PushBack("X" + string(rune('0'+i)))
	}
	for i := 0; i < 8; i++ {
		arr.Remove(0)
	}
	// size=2, capacity/4=2.5, условие size < capacity/4 не выполняется
	if arr.GetCapacity() != 10 {
		t.Errorf("Емкость не должна измениться, получена %d", arr.GetCapacity())
	}
}

func TestDynamicArray_RemoveInvalidIndex(t *testing.T) {
	arr := NewDynamicArray(5)

	// Удаление с отрицательным индексом
	arr.Remove(-1) // Должно вывести "Неверный индекс"

	// Удаление с индексом >= size
	arr.Remove(0) // Должно вывести "Неверный индекс"

	// Удаление с индексом > size
	arr.PushBack("A")
	arr.Remove(5) // Должно вывести "Неверный индекс"
	if arr.GetSize() != 1 {
		t.Errorf("Размер не должен измениться, получен %d", arr.GetSize())
	}
}

func TestDynamicArray_GetSet(t *testing.T) {
	arr := NewDynamicArray(5)

	// Get из пустого массива
	value := arr.Get(0)
	if value != "" {
		t.Errorf("Ожидалась пустая строка, получен '%s'", value)
	}

	// Set в пустом массиве
	arr.Set(0, "Test") // Должно вывести "Неверный индекс"
	if arr.GetSize() != 0 {
		t.Errorf("Размер не должен измениться")
	}

	// Нормальная работа
	arr.PushBack("Initial")
	arr.PushBack("Second")
	arr.PushBack("Third")

	// Get существующего элемента
	if arr.Get(0) != "Initial" {
		t.Errorf("Ожидался 'Initial', получен '%s'", arr.Get(0))
	}

	// Get с неверным индексом
	value = arr.Get(10)
	if value != "" {
		t.Errorf("Ожидалась пустая строка, получен '%s'", value)
	}

	// Set существующего элемента
	arr.Set(1, "Changed")
	if arr.Get(1) != "Changed" {
		t.Errorf("Ожидался 'Changed', получен '%s'", arr.Get(1))
	}

	// Set с неверным индексом
	arr.Set(10, "Invalid") // Должно вывести "Неверный индекс"
	if arr.GetSize() != 3 {
		t.Errorf("Размер не должен измениться")
	}
}

func TestDynamicArray_Resize(t *testing.T) {
	// Тест увеличения capacity
	arr := NewDynamicArray(2)

	// Проверка начального состояния
	if arr.GetCapacity() != 2 {
		t.Errorf("Ожидалась емкость 2, получена %d", arr.GetCapacity())
	}

	// Добавляем элементы до превышения capacity
	for i := 0; i < 5; i++ {
		arr.PushBack("Element" + string(rune('0'+i)))
	}

	// Проверка что capacity увеличилось
	if arr.GetCapacity() != 8 {
		t.Errorf("Ожидалась емкость 8, получена %d", arr.GetCapacity())
	}
	if arr.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", arr.GetSize())
	}

	// Проверка что все элементы на месте
	for i := 0; i < 5; i++ {
		expected := "Element" + string(rune('0'+i))
		if arr.Get(i) != expected {
			t.Errorf("На позиции %d ожидался '%s', получен '%s'", i, expected, arr.Get(i))
		}
	}

	// Тест уменьшения capacity (уже в TestDynamicArray_RemoveWithResize)
}

func TestDynamicArray_Clear(t *testing.T) {
	arr := NewDynamicArray(5)

	// Очистка пустого массива
	arr.Clear()
	if arr.GetSize() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 5 {
		t.Errorf("Емкость не должна измениться, получена %d", arr.GetCapacity())
	}

	// Очистка непустого массива
	arr.PushBack("One")
	arr.PushBack("Two")
	arr.PushBack("Three")
	arr.PushBack("Four")
	arr.PushBack("Five")

	if arr.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", arr.GetSize())
	}

	arr.Clear()

	if arr.GetSize() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 5 {
		t.Errorf("Емкость не должна измениться, получена %d", arr.GetCapacity())
	}

	// Проверка что после очистки можно добавлять элементы
	arr.PushBack("New")
	if arr.GetSize() != 1 {
		t.Errorf("После очистки должен работать PushBack, размер %d", arr.GetSize())
	}
	if arr.Get(0) != "New" {
		t.Errorf("Ожидался 'New', получен '%s'", arr.Get(0))
	}
}

func TestDynamicArray_Print(t *testing.T) {
	arr := NewDynamicArray(5)

	// Print пустого массива
	arr.Print() // Должно вывести "Массив[0/5]: "

	// Print непустого массива
	arr.PushBack("Hello")
	arr.PushBack("World")
	arr.Print() // Должно вывести "Массив[2/5]: \"Hello\" \"World\" "

	// Print после удаления
	arr.Remove(0)
	arr.Print() // Должно вывести "Массив[1/5]: \"World\" "

	// Print после очистки
	arr.Clear()
	arr.Print() // Должно вывести "Массив[0/5]: "
}

func TestDynamicArray_SaveLoadTextFile(t *testing.T) {
	arr := NewDynamicArray(5)

	// Сохранение пустого массива
	err := arr.SaveToFile("empty_array.txt")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого массива: %v", err)
	}

	// Загрузка пустого массива
	emptyArr := NewDynamicArray(5)
	err = emptyArr.LoadFromFile("empty_array.txt")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого массива: %v", err)
	}
	if emptyArr.GetSize() != 0 {
		t.Errorf("Загруженный пустой массив должен иметь размер 0, получен %d", emptyArr.GetSize())
	}

	// Сохранение непустого массива
	arr.PushBack("Hello")
	arr.PushBack("World")
	arr.PushBack("Go")
	arr.PushBack("Programming")
	arr.PushBack("Data")

	filename := "test_array.txt"
	err = arr.SaveToFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	// Загрузка из файла с той же емкостью
	newArr := NewDynamicArray(5)
	err = newArr.LoadFromFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if newArr.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", newArr.GetSize())
	}

	// Проверка порядка элементов
	expected := []string{"Hello", "World", "Go", "Programming", "Data"}
	for i, val := range expected {
		if newArr.Get(i) != val {
			t.Errorf("На позиции %d ожидался '%s', получен '%s'", i, val, newArr.Get(i))
		}
	}

	// Ошибка загрузки из несуществующего файла
	err = arr.LoadFromFile("nonexistent_file.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	// Загрузка из файла с другой емкостью
	diffArr := NewDynamicArray(10)
	err = diffArr.LoadFromFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла с другой емкостью: %v", err)
	}
	if diffArr.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", diffArr.GetSize())
	}
	if diffArr.GetCapacity() != 10 {
		t.Errorf("Емкость не должна измениться, получена %d", diffArr.GetCapacity())
	}

	// Тест с пустым файлом
	emptyFile := "empty.txt"
	f, _ := os.Create(emptyFile)
	f.Close()

	err = arr.LoadFromFile(emptyFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке пустого файла")
	}
	os.Remove(emptyFile)

	os.Remove(filename)
	os.Remove("empty_array.txt")
}

func TestDynamicArray_SaveLoadBinaryFile(t *testing.T) {
	arr := NewDynamicArray(5)

	// Сохранение пустого массива в бинарный файл
	err := arr.SaveToBinaryFile("empty_array.bin")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого массива в бинарный файл: %v", err)
	}

	// Загрузка пустого массива из бинарного файла
	emptyArr := NewDynamicArray(5)
	err = emptyArr.LoadFromBinaryFile("empty_array.bin")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого массива из бинарного файла: %v", err)
	}
	if emptyArr.GetSize() != 0 {
		t.Errorf("Загруженный пустой массив должен иметь размер 0, получен %d", emptyArr.GetSize())
	}

	// Сохранение непустого массива
	arr.PushBack("Binary")
	arr.PushBack("File")
	arr.PushBack("Test")
	arr.PushBack("Format")
	arr.PushBack("Data")

	filename := "test_array.bin"
	err = arr.SaveToBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	// Загрузка из бинарного файла
	newArr := NewDynamicArray(5)
	err = newArr.LoadFromBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if newArr.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", newArr.GetSize())
	}

	// Проверка порядка элементов
	expected := []string{"Binary", "File", "Test", "Format", "Data"}
	for i, val := range expected {
		if newArr.Get(i) != val {
			t.Errorf("На позиции %d ожидался '%s', получен '%s'", i, val, newArr.Get(i))
		}
	}

	// Ошибка загрузки из несуществующего бинарного файла
	err = arr.LoadFromBinaryFile("nonexistent_file.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего бинарного файла")
	}

	// Тест с поврежденным бинарным файлом (недостаточно данных)
	badFile := "bad_array.bin"
	f, _ := os.Create(badFile)
	f.Write([]byte{0, 1, 2, 3}) // Только размер, нет данных
	f.Close()

	err = arr.LoadFromBinaryFile(badFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке поврежденного бинарного файла")
	}
	os.Remove(badFile)

	os.Remove(filename)
	os.Remove("empty_array.bin")
}

func TestDynamicArray_EdgeCases(t *testing.T) {
	arr := NewDynamicArray(1)

	// PushBack с capacity = 0
	arr = NewDynamicArray(0)
	arr.PushBack("Zero")
	if arr.GetCapacity() != 4 {
		t.Errorf("Ожидалась емкость 4, получена %d", arr.GetCapacity())
	}
	if arr.GetSize() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "Zero" {
		t.Errorf("Ожидался 'Zero', получен '%s'", arr.Get(0))
	}

	// PushBack с capacity = 1
	arr = NewDynamicArray(1)
	arr.PushBack("One")
	arr.PushBack("Two") // Должен сработать resize
	if arr.GetCapacity() != 2 {
		t.Errorf("Ожидалась емкость 2, получена %d", arr.GetCapacity())
	}
	if arr.GetSize() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", arr.GetSize())
	}

	// Многократный PushBack и Remove
	arr = NewDynamicArray(2)
	for i := 0; i < 10; i++ {
		arr.PushBack("X" + string(rune('0'+i)))
	}
	for i := 0; i < 8; i++ {
		arr.Remove(0)
	}
	// Проверка что capacity уменьшилась
	if arr.GetCapacity() > 4 {
		t.Errorf("Емкость должна уменьшиться, получена %d", arr.GetCapacity())
	}
}

func TestDynamicArray_GetSizeCapacity(t *testing.T) {
	arr := NewDynamicArray(5)

	// Начальные значения
	if arr.GetSize() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 5 {
		t.Errorf("Ожидалась емкость 5, получена %d", arr.GetCapacity())
	}

	// После PushBack
	arr.PushBack("A")
	if arr.GetSize() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 5 {
		t.Errorf("Емкость не должна измениться, получена %d", arr.GetCapacity())
	}

	// После Remove
	arr.Remove(0)
	if arr.GetSize() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", arr.GetSize())
	}
	if arr.GetCapacity() != 5 {
		t.Errorf("Емкость не должна измениться, получена %d", arr.GetCapacity())
	}
}

func TestDynamicArray_DefaultConstructor(t *testing.T) {
	arr := NewDynamicArray(0)

	if arr.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость по умолчанию 10, получена %d", arr.GetCapacity())
	}

	arr2 := NewDynamicArray(-5)
	if arr2.GetCapacity() != 10 {
		t.Errorf("Ожидалась емкость по умолчанию 10, получена %d", arr2.GetCapacity())
	}
}

func TestDynamicArray_MultipleOperations(t *testing.T) {
	arr := NewDynamicArray(3)

	// Комбинация различных операций
	arr.PushBack("A")
	arr.PushBack("B")
	arr.PushBack("C")
	arr.Insert(1, "X")
	arr.Remove(2)
	arr.Set(0, "Z")

	if arr.GetSize() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", arr.GetSize())
	}

	expected := []string{"Z", "X", "C"}
	for i, val := range expected {
		if arr.Get(i) != val {
			t.Errorf("На позиции %d ожидался '%s', получен '%s'", i, val, arr.Get(i))
		}
	}

	// Очистка и повторное заполнение
	arr.Clear()
	arr.PushBack("New1")
	arr.PushBack("New2")

	if arr.GetSize() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", arr.GetSize())
	}
	if arr.Get(0) != "New1" || arr.Get(1) != "New2" {
		t.Errorf("Ожидались 'New1','New2', получены '%s','%s'", arr.Get(0), arr.Get(1))
	}
}
