package main

import (
	"os"
	"testing"
)

func TestStack_PushPop(t *testing.T) {
	stack := NewStack()

	// Проверка пустого стека
	if !stack.IsEmpty() {
		t.Errorf("Новый стек должен быть пустым")
	}
	if stack.Size() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", stack.Size())
	}

	// Push
	stack.Push(10)
	stack.Push(20)
	stack.Push(30)

	if stack.IsEmpty() {
		t.Errorf("Стек не должен быть пустым")
	}
	if stack.Size() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", stack.Size())
	}
	if stack.Peek() != 30 {
		t.Errorf("Ожидался верхний элемент 30, получен %d", stack.Peek())
	}

	// Pop
	val := stack.Pop()
	if val != 30 {
		t.Errorf("Ожидалось извлечь 30, получено %d", val)
	}
	val = stack.Pop()
	if val != 20 {
		t.Errorf("Ожидалось извлечь 20, получено %d", val)
	}
	val = stack.Pop()
	if val != 10 {
		t.Errorf("Ожидалось извлечь 10, получено %d", val)
	}

	// Pop из пустого стека
	val = stack.Pop()
	if val != -1 {
		t.Errorf("Pop из пустого стека должен вернуть -1, получен %d", val)
	}
	if !stack.IsEmpty() {
		t.Errorf("После извлечения всех элементов стек должен быть пустым")
	}
}

func TestStack_Clear(t *testing.T) {
	stack := NewStack()

	// Очистка пустого стека
	stack.Clear()
	if !stack.IsEmpty() {
		t.Errorf("Пустой стек после очистки должен быть пустым")
	}
	if stack.Size() != 0 {
		t.Errorf("Размер должен быть 0, получен %d", stack.Size())
	}

	// Очистка непустого стека
	stack.Push(1)
	stack.Push(2)
	stack.Push(3)
	stack.Push(4)
	stack.Push(5)

	if stack.Size() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", stack.Size())
	}

	stack.Clear()

	if !stack.IsEmpty() {
		t.Errorf("После очистки стек должен быть пустым")
	}
	if stack.Size() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", stack.Size())
	}

	// Проверка что после очистки можно добавлять элементы
	stack.Push(100)
	stack.Push(200)
	if stack.Peek() != 200 {
		t.Errorf("После очистки должен работать Push")
	}
	if stack.Size() != 2 {
		t.Errorf("Размер должен быть 2, получен %d", stack.Size())
	}
}

func TestStack_Print(t *testing.T) {
	stack := NewStack()

	// Печать пустого стека
	stack.Print() // Должно вывести "Стек пуст!"

	// Печать непустого стека
	stack.Push(5)
	stack.Push(4)
	stack.Push(3)
	stack.Push(2)
	stack.Push(1)
	stack.Print() // Должно вывести "Стек (сверху вниз): 1 2 3 4 5"

	// Печать после удаления
	stack.Pop()
	stack.Pop()
	stack.Print() // Должно вывести "Стек (сверху вниз): 3 4 5"

	// Печать с одним элементом
	stack.Clear()
	stack.Push(42)
	stack.Print()
}

func TestStack_SaveLoadFile(t *testing.T) {
	stack := NewStack()

	// 1. Сохранение пустого стека
	err := stack.SaveToFile("empty_stack.txt")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого стека: %v", err)
	}

	// 2. Загрузка пустого стека
	emptyStack := NewStack()
	err = emptyStack.LoadFromFile("empty_stack.txt")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого стека: %v", err)
	}
	if !emptyStack.IsEmpty() {
		t.Errorf("Загруженный пустой стек должен быть пустым")
	}
	if emptyStack.Size() != 0 {
		t.Errorf("Размер должен быть 0, получен %d", emptyStack.Size())
	}

	// 3. Сохранение непустого стека
	stack.Push(100)
	stack.Push(200)
	stack.Push(300)
	stack.Push(400)

	filename := "test_stack.txt"
	err = stack.SaveToFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	// 4. Загрузка из файла
	newStack := NewStack()
	err = newStack.LoadFromFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if newStack.Size() != 4 {
		t.Errorf("Ожидался размер 4, получен %d", newStack.Size())
	}
	if newStack.Peek() != 400 {
		t.Errorf("Ожидался верхний элемент 400, получен %d", newStack.Peek())
	}

	// 5. Проверка порядка элементов
	val := newStack.Pop()
	if val != 400 {
		t.Errorf("Ожидалось 400, получено %d", val)
	}
	val = newStack.Pop()
	if val != 300 {
		t.Errorf("Ожидалось 300, получено %d", val)
	}
	val = newStack.Pop()
	if val != 200 {
		t.Errorf("Ожидалось 200, получено %d", val)
	}
	val = newStack.Pop()
	if val != 100 {
		t.Errorf("Ожидалось 100, получено %d", val)
	}

	// 6. Ошибка загрузки из несуществующего файла
	err = stack.LoadFromFile("nonexistent_file_12345.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	os.Remove(filename)
	os.Remove("empty_stack.txt")
}

func TestStack_EmptyStackOperations(t *testing.T) {
	stack := NewStack()

	// Pop на пустом стеке
	val := stack.Pop()
	if val != -1 {
		t.Errorf("Pop() на пустом стеке должен вернуть -1, получен %d", val)
	}

	// Peek на пустом стеке
	peek := stack.Peek()
	if peek != -1 {
		t.Errorf("Peek() на пустом стеке должен вернуть -1, получен %d", peek)
	}

	// IsEmpty на пустом стеке
	if !stack.IsEmpty() {
		t.Errorf("Стек должен быть пустым")
	}

	// Size на пустом стеке
	if stack.Size() != 0 {
		t.Errorf("Размер должен быть 0, получен %d", stack.Size())
	}

	// Многократный Pop на пустом стеке
	for i := 0; i < 5; i++ {
		val = stack.Pop()
		if val != -1 {
			t.Errorf("Pop() должен возвращать -1, получен %d", val)
		}
	}
}

func TestStack_MaxSize(t *testing.T) {
	stack := NewStack()

	// Заполнение до максимума
	for i := 0; i < 100; i++ {
		stack.Push(i)
		if stack.Peek() != i {
			t.Errorf("Ожидался верхний элемент %d, получен %d", i, stack.Peek())
		}
		if stack.Size() != i+1 {
			t.Errorf("Ожидался размер %d, получен %d", i+1, stack.Size())
		}
	}

	if stack.Size() != 100 {
		t.Errorf("Ожидался размер 100, получен %d", stack.Size())
	}

	// Попытка добавить 101-й элемент - переполнение
	stack.Push(101) // Должно вывести "Стек переполнен!"
	if stack.Size() != 100 {
		t.Errorf("После переполнения размер должен остаться 100, получен %d", stack.Size())
	}

	// Проверка что верхний элемент не изменился
	if stack.Peek() != 99 {
		t.Errorf("После переполнения верхний элемент должен остаться 99, получен %d", stack.Peek())
	}

	// Извлечение всех элементов
	for i := 99; i >= 0; i-- {
		val := stack.Pop()
		if val != i {
			t.Errorf("Ожидалось извлечь %d, получено %d", i, val)
		}
		if stack.Size() != i {
			t.Errorf("Ожидался размер %d, получен %d", i, stack.Size())
		}
	}

	if !stack.IsEmpty() {
		t.Errorf("После извлечения всех элементов стек должен быть пустым")
	}
}

func TestStack_BinarySaveLoad(t *testing.T) {
	stack := NewStack()

	// 1. Сохранение пустого стека
	err := stack.SaveToBinaryFile("empty_stack.bin")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого стека: %v", err)
	}

	// 2. Загрузка пустого стека
	emptyStack := NewStack()
	err = emptyStack.LoadFromBinaryFile("empty_stack.bin")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого стека: %v", err)
	}
	if !emptyStack.IsEmpty() {
		t.Errorf("Загруженный пустой стек должен быть пустым")
	}

	// 3. Сохранение непустого стека
	stack.Push(555)
	stack.Push(666)
	stack.Push(777)
	stack.Push(888)
	stack.Push(999)

	filename := "test_stack.bin"
	err = stack.SaveToBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	// 4. Загрузка из бинарного файла
	newStack := NewStack()
	err = newStack.LoadFromBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if newStack.Size() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", newStack.Size())
	}
	if newStack.Peek() != 999 {
		t.Errorf("Ожидался верхний элемент 999, получен %d", newStack.Peek())
	}

	// 5. Проверка порядка извлечения
	val := newStack.Pop()
	if val != 999 {
		t.Errorf("Ожидалось извлечь 999, получено %d", val)
	}
	val = newStack.Pop()
	if val != 888 {
		t.Errorf("Ожидалось извлечь 888, получено %d", val)
	}
	val = newStack.Pop()
	if val != 777 {
		t.Errorf("Ожидалось извлечь 777, получено %d", val)
	}
	val = newStack.Pop()
	if val != 666 {
		t.Errorf("Ожидалось извлечь 666, получено %d", val)
	}
	val = newStack.Pop()
	if val != 555 {
		t.Errorf("Ожидалось извлечь 555, получено %d", val)
	}

	// 6. Ошибка загрузки из несуществующего файла
	err = stack.LoadFromBinaryFile("nonexistent_file_12345.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего бинарного файла")
	}

	os.Remove(filename)
	os.Remove("empty_stack.bin")
}

func TestStack_FileOperationErrors(t *testing.T) {
	stack := NewStack()

	// Загрузка из несуществующего текстового файла
	err := stack.LoadFromFile("nonexistent_file_12345.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	// Загрузка из несуществующего бинарного файла
	err = stack.LoadFromBinaryFile("nonexistent_file_12345.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего бинарного файла")
	}
}

func TestStack_MultipleOperations(t *testing.T) {
	stack := NewStack()

	// Чередование push/pop
	for i := 0; i < 10; i++ {
		stack.Push(i)
		if stack.Peek() != i {
			t.Errorf("Ожидался верхний элемент %d, получен %d", i, stack.Peek())
		}
		if stack.Size() != i+1 {
			t.Errorf("Ожидался размер %d, получен %d", i+1, stack.Size())
		}
	}

	for i := 9; i >= 0; i-- {
		if stack.Pop() != i {
			t.Errorf("Ожидалось извлечь %d", i)
		}
		if stack.Size() != i {
			t.Errorf("Ожидался размер %d, получен %d", i, stack.Size())
		}
	}

	// Push после полной очистки
	stack.Push(999)
	if stack.Peek() != 999 {
		t.Errorf("Ожидался верхний элемент 999, получен %d", stack.Peek())
	}
	if stack.Size() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", stack.Size())
	}

	// Pop и снова Push
	stack.Pop()
	stack.Push(888)
	if stack.Peek() != 888 {
		t.Errorf("Ожидался верхний элемент 888, получен %d", stack.Peek())
	}

	// Push после частичного опустошения
	stack.Pop()
	stack.Push(777)
	stack.Push(666)
	if stack.Size() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", stack.Size())
	}
}

func TestStack_Peek(t *testing.T) {
	stack := NewStack()

	// Peek на пустом стеке
	if stack.Peek() != -1 {
		t.Errorf("Peek на пустом стеке должен вернуть -1, получен %d", stack.Peek())
	}

	// Peek после Push
	stack.Push(42)
	if stack.Peek() != 42 {
		t.Errorf("Peek должен вернуть 42, получен %d", stack.Peek())
	}

	// Peek не должен удалять элемент
	if stack.Size() != 1 {
		t.Errorf("Размер должен быть 1, получен %d", stack.Size())
	}

	// Peek после Pop
	stack.Pop()
	if stack.Peek() != -1 {
		t.Errorf("После Pop Peek должен вернуть -1, получен %d", stack.Peek())
	}

	// Многократный Peek
	stack.Push(100)
	stack.Push(200)
	if stack.Peek() != 200 {
		t.Errorf("Peek должен вернуть 200")
	}
	if stack.Peek() != 200 {
		t.Errorf("Peek не должен изменять стек, все еще 200")
	}
}

func TestStack_Size(t *testing.T) {
	stack := NewStack()

	// Size на пустом стеке
	if stack.Size() != 0 {
		t.Errorf("Размер должен быть 0, получен %d", stack.Size())
	}

	// Size после Push
	for i := 1; i <= 5; i++ {
		stack.Push(i)
		if stack.Size() != i {
			t.Errorf("Размер должен быть %d, получен %d", i, stack.Size())
		}
	}

	// Size после Pop
	for i := 4; i >= 0; i-- {
		stack.Pop()
		if stack.Size() != i {
			t.Errorf("Размер должен быть %d, получен %d", i, stack.Size())
		}
	}
}

func TestStack_IsEmpty(t *testing.T) {
	stack := NewStack()

	// Пустой стек
	if !stack.IsEmpty() {
		t.Errorf("Новый стек должен быть пустым")
	}

	// После Push
	stack.Push(1)
	if stack.IsEmpty() {
		t.Errorf("Стек с элементами не должен быть пустым")
	}

	// После Pop всех элементов
	stack.Pop()
	if !stack.IsEmpty() {
		t.Errorf("После удаления всех элементов стек должен быть пустым")
	}

	// После Clear
	stack.Push(1)
	stack.Push(2)
	stack.Clear()
	if !stack.IsEmpty() {
		t.Errorf("После Clear стек должен быть пустым")
	}
}
