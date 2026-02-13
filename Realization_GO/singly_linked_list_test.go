package main

import (
	"os"
	"testing"
)

func TestSinglyLinkedList_NewList(t *testing.T) {
	list := NewSinglyLinkedList()

	if list.Head != nil {
		t.Errorf("Новый список должен иметь Head = nil")
	}
}

func TestSinglyLinkedList_AddToHead(t *testing.T) {
	list := NewSinglyLinkedList()

	// Добавление в пустой список
	list.AddToHead(10)
	if !list.FindByValue(10) {
		t.Errorf("Элемент 10 должен существовать")
	}
	if list.Head.Key != 10 {
		t.Errorf("Head должен быть 10, получен %d", list.Head.Key)
	}

	// Добавление в непустой список
	list.AddToHead(20)
	if !list.FindByValue(20) {
		t.Errorf("Элемент 20 должен существовать")
	}
	if !list.FindByValue(10) {
		t.Errorf("Элемент 10 должен существовать")
	}
	if list.Head.Key != 20 {
		t.Errorf("Head должен быть 20, получен %d", list.Head.Key)
	}

	// Проверка порядка (20 -> 10)
	if list.Head.Next.Key != 10 {
		t.Errorf("Второй элемент должен быть 10, получен %d", list.Head.Next.Key)
	}

	// Добавление еще одного
	list.AddToHead(30)
	if list.Head.Key != 30 {
		t.Errorf("Head должен быть 30, получен %d", list.Head.Key)
	}
}

func TestSinglyLinkedList_AddToTail(t *testing.T) {
	list := NewSinglyLinkedList()

	// Добавление в пустой список
	list.AddToTail(20)
	if !list.FindByValue(20) {
		t.Errorf("Элемент 20 должен существовать")
	}
	if list.Head.Key != 20 {
		t.Errorf("Head должен быть 20, получен %d", list.Head.Key)
	}

	// Добавление в непустой список
	list.AddToTail(30)
	list.AddToTail(40)

	if !list.FindByValue(30) || !list.FindByValue(40) {
		t.Errorf("Элементы должны существовать")
	}

	// Проверка порядка (20 -> 30 -> 40)
	if list.Head.Next.Key != 30 {
		t.Errorf("Второй элемент должен быть 30, получен %d", list.Head.Next.Key)
	}
	if list.Head.Next.Next.Key != 40 {
		t.Errorf("Третий элемент должен быть 40, получен %d", list.Head.Next.Next.Key)
	}
}

func TestSinglyLinkedList_AddAfter(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Добавление в пустой список
	list.AddAfter(1, 99) // Должно вывести "Список пуст"
	if list.FindByValue(99) {
		t.Errorf("Элемент 99 не должен быть добавлен в пустой список")
	}

	// 2. Подготовка данных
	list.AddToTail(1)
	list.AddToTail(2)
	list.AddToTail(3)
	list.AddToTail(4)

	// 3. Добавление после существующего элемента
	list.AddAfter(1, 99)
	if !list.FindByValue(99) {
		t.Errorf("Элемент 99 должен существовать")
	}

	// 4. Добавление после несуществующего элемента
	list.AddAfter(999, 88) // Должно вывести "999 не найден!"
	if list.FindByValue(88) {
		t.Errorf("Элемент 88 не должен быть добавлен")
	}

	// 5. Добавление после последнего элемента
	list.AddAfter(4, 77)
	if !list.FindByValue(77) {
		t.Errorf("Элемент 77 должен существовать")
	}

	// Проверка что 77 добавлен в конец
	current := list.Head
	for current.Next != nil {
		current = current.Next
	}
	if current.Key != 77 {
		t.Errorf("Последний элемент должен быть 77, получен %d", current.Key)
	}

	// 6. Добавление после головы
	list.AddAfter(1, 55)
	if !list.FindByValue(55) {
		t.Errorf("Элемент 55 должен существовать")
	}
}

func TestSinglyLinkedList_AddBefore(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Добавление в пустой список
	list.AddBefore(1, 99) // Должно вывести "Список пуст"
	if list.FindByValue(99) {
		t.Errorf("Элемент 99 не должен быть добавлен в пустой список")
	}

	// 2. Добавление перед первым элементом
	list.AddToTail(10)
	list.AddBefore(10, 5)
	if !list.FindByValue(5) {
		t.Errorf("Элемент 5 должен существовать")
	}
	if list.Head.Key != 5 {
		t.Errorf("Head должен быть 5, получен %d", list.Head.Key)
	}

	// 3. Добавление перед средним элементом
	list.AddToTail(20)
	list.AddToTail(30)
	list.AddToTail(40)
	list.AddBefore(20, 15)
	if !list.FindByValue(15) {
		t.Errorf("Элемент 15 должен существовать")
	}

	// 4. Добавление перед последним элементом
	list.AddBefore(40, 35)
	if !list.FindByValue(35) {
		t.Errorf("Элемент 35 должен существовать")
	}

	// 5. Добавление перед несуществующим элементом
	list.AddBefore(999, 88) // Должно вывести "999 не найден!"
	if list.FindByValue(88) {
		t.Errorf("Элемент 88 не должен быть добавлен")
	}

	// 6. Добавление перед в списке с одним элементом
	list2 := NewSinglyLinkedList()
	list2.AddToTail(50)
	list2.AddBefore(50, 45)
	if !list2.FindByValue(45) {
		t.Errorf("Элемент 45 должен существовать")
	}
	if list2.Head.Key != 45 {
		t.Errorf("Head должен быть 45, получен %d", list2.Head.Key)
	}
}

func TestSinglyLinkedList_DeleteFromHead(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Удаление из пустого списка
	list.DeleteFromHead() // Должно вывести "Список пуст!"

	// 2. Удаление из списка с одним элементом
	list.AddToTail(1)
	list.DeleteFromHead()
	if list.FindByValue(1) {
		t.Errorf("Элемент 1 должен быть удален")
	}
	if list.Head != nil {
		t.Errorf("Head должен быть nil")
	}

	// 3. Удаление из списка с несколькими элементами
	list.AddToTail(2)
	list.AddToTail(3)
	list.AddToTail(4)
	list.DeleteFromHead()
	if list.FindByValue(2) {
		t.Errorf("Элемент 2 должен быть удален")
	}
	if !list.FindByValue(3) || !list.FindByValue(4) {
		t.Errorf("Элементы 3,4 должны существовать")
	}
	if list.Head.Key != 3 {
		t.Errorf("Head должен быть 3, получен %d", list.Head.Key)
	}
}

func TestSinglyLinkedList_DeleteFromTail(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Удаление из пустого списка
	list.DeleteFromTail() // Должно вывести "Список пуст!"

	// 2. Удаление из списка с одним элементом
	list.AddToTail(100)
	list.DeleteFromTail()
	if list.FindByValue(100) {
		t.Errorf("Элемент 100 должен быть удален")
	}
	if list.Head != nil {
		t.Errorf("Head должен быть nil")
	}

	// 3. Удаление из списка с несколькими элементами
	list.AddToTail(200)
	list.AddToTail(300)
	list.AddToTail(400)
	list.DeleteFromTail()
	if list.FindByValue(400) {
		t.Errorf("Элемент 400 должен быть удален")
	}
	if !list.FindByValue(200) || !list.FindByValue(300) {
		t.Errorf("Элементы 200,300 должны существовать")
	}

	// Проверка что хвост обновился
	current := list.Head
	for current.Next != nil {
		current = current.Next
	}
	if current.Key != 300 {
		t.Errorf("Последний элемент должен быть 300, получен %d", current.Key)
	}
}

func TestSinglyLinkedList_DeleteByValue(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Удаление из пустого списка
	list.DeleteByValue(99) // Должно вывести "Список пуст!"

	// 2. Подготовка данных
	list.AddToTail(42)
	list.AddToTail(24)
	list.AddToTail(99)
	list.AddToTail(100)
	list.AddToTail(101)

	// 3. Удаление головы
	list.DeleteByValue(42)
	if list.FindByValue(42) {
		t.Errorf("Элемент 42 должен быть удален")
	}
	if list.Head.Key != 24 {
		t.Errorf("Head должен быть 24, получен %d", list.Head.Key)
	}

	// 4. Удаление среднего элемента
	list.DeleteByValue(99)
	if list.FindByValue(99) {
		t.Errorf("Элемент 99 должен быть удален")
	}

	// 5. Удаление хвоста
	list.DeleteByValue(101)
	if list.FindByValue(101) {
		t.Errorf("Элемент 101 должен быть удален")
	}

	// Проверка хвоста
	current := list.Head
	for current.Next != nil {
		current = current.Next
	}
	if current.Key != 100 {
		t.Errorf("Последний элемент должен быть 100, получен %d", current.Key)
	}

	// 6. Удаление последнего элемента
	list.DeleteByValue(24)
	list.DeleteByValue(100)
	if list.Head != nil {
		t.Errorf("Список должен быть пуст")
	}

	// 7. Удаление несуществующего элемента
	list.DeleteByValue(999) // Должно вывести "Элемент 999 не найден!"
}

func TestSinglyLinkedList_DeleteAfter(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Удаление после в пустом списке
	list.DeleteAfter(20) // Должно вывести "Список пуст! Невозможно удалить элемент."

	// 2. Подготовка данных
	list.AddToTail(10)
	list.AddToTail(20)
	list.AddToTail(30)
	list.AddToTail(40)
	list.AddToTail(50)

	// 3. Удаление после существующего элемента
	list.DeleteAfter(20)
	if list.FindByValue(30) {
		t.Errorf("Элемент 30 должен быть удален")
	}

	// 4. Удаление после несуществующего элемента
	list.DeleteAfter(999) // Должно вывести "Элемент 999 не найден в списке!"

	// 5. Удаление после последнего элемента
	list.DeleteAfter(50) // Должно вывести "После элемента 50 нет элементов для удаления!"

	// 6. Удаление после в списке с одним элементом
	list2 := NewSinglyLinkedList()
	list2.AddToTail(60)
	list2.DeleteAfter(60) // Должно вывести "После элемента 60 нет элементов для удаления!"

	// 7. Удаление после головы
	list = NewSinglyLinkedList()
	list.AddToTail(100)
	list.AddToTail(200)
	list.AddToTail(300)
	list.DeleteAfter(100)
	if list.FindByValue(200) {
		t.Errorf("Элемент 200 должен быть удален")
	}
	if list.Head.Next.Key != 300 {
		t.Errorf("Второй элемент должен быть 300, получен %d", list.Head.Next.Key)
	}
}

func TestSinglyLinkedList_DeleteBefore(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Удаление перед в пустом списке
	list.DeleteBefore(30) // Должно вывести "Недостаточно элементов для удаления!"

	// 2. Подготовка данных
	list.AddToTail(10)
	list.AddToTail(20)
	list.AddToTail(30)
	list.AddToTail(40)
	list.AddToTail(50)

	// 3. Удаление перед существующим элементом (не первым)
	list.DeleteBefore(30)
	if list.FindByValue(20) {
		t.Errorf("Элемент 20 должен быть удален")
	}

	// 4. Удаление перед первым элементом
	list.DeleteBefore(10) // Должно вывести "Невозможно удалить элемент перед первым элементом!"

	// 5. Удаление перед несуществующим элементом
	list.DeleteBefore(999) // Должно вывести "Элемент 999 не найден!"

	// 6. Удаление перед вторым элементом
	list2 := NewSinglyLinkedList()
	list2.AddToTail(100)
	list2.AddToTail(200)
	list2.DeleteBefore(200)
	if list2.FindByValue(100) {
		t.Errorf("Элемент 100 должен быть удален")
	}
	if list2.Head.Key != 200 {
		t.Errorf("Head должен быть 200, получен %d", list2.Head.Key)
	}

	// 7. Удаление перед в списке с одним элементом
	list3 := NewSinglyLinkedList()
	list3.AddToTail(500)
	list3.DeleteBefore(500) // Должно вывести "Недостаточно элементов для удаления!"

	// 8. Удаление перед когда targetValue - второй элемент, но список из 2 элементов
	list4 := NewSinglyLinkedList()
	list4.AddToTail(1000)
	list4.AddToTail(2000)
	list4.DeleteBefore(2000)
	if list4.FindByValue(1000) {
		t.Errorf("Элемент 1000 должен быть удален")
	}
}

func TestSinglyLinkedList_FindByValue(t *testing.T) {
	list := NewSinglyLinkedList()

	// Поиск в пустом списке
	if list.FindByValue(999) {
		t.Errorf("В пустом списке не должно быть элементов")
	}

	// Поиск существующих элементов
	list.AddToTail(123)
	list.AddToTail(456)
	list.AddToTail(789)

	if !list.FindByValue(123) {
		t.Errorf("Элемент 123 должен существовать")
	}
	if !list.FindByValue(456) {
		t.Errorf("Элемент 456 должен существовать")
	}
	if !list.FindByValue(789) {
		t.Errorf("Элемент 789 должен существовать")
	}

	// Поиск несуществующего элемента
	if list.FindByValue(999) {
		t.Errorf("Элемент 999 не должен существовать")
	}

	// Поиск после удаления
	list.DeleteByValue(456)
	if list.FindByValue(456) {
		t.Errorf("Элемент 456 не должен существовать после удаления")
	}
}

func TestSinglyLinkedList_ClearList(t *testing.T) {
	list := NewSinglyLinkedList()

	// Очистка пустого списка
	list.Clear()
	if list.Head != nil {
		t.Errorf("После очистки пустого списка Head должен быть nil")
	}

	// Очистка непустого списка
	list.AddToTail(1)
	list.AddToTail(2)
	list.AddToTail(3)
	list.AddToTail(4)
	list.AddToTail(5)

	list.Clear()

	if list.FindByValue(1) || list.FindByValue(2) || list.FindByValue(3) ||
		list.FindByValue(4) || list.FindByValue(5) {
		t.Errorf("После очистки элементы не должны существовать")
	}
	if list.Head != nil {
		t.Errorf("После очистки Head должен быть nil")
	}

	// Проверка что после очистки можно добавлять
	list.AddToTail(100)
	if !list.FindByValue(100) {
		t.Errorf("После очистки должен работать AddToTail")
	}
	if list.Head.Key != 100 {
		t.Errorf("Head должен быть 100, получен %d", list.Head.Key)
	}
}

func TestSinglyLinkedList_PrintFunctions(t *testing.T) {
	list := NewSinglyLinkedList()

	// Печать пустого списка
	list.Print()
	list.PrintReverse()

	// Печать непустого списка
	list.AddToTail(1)
	list.AddToTail(2)
	list.AddToTail(3)
	list.AddToTail(4)
	list.AddToTail(5)

	list.Print()
	list.PrintReverse()

	// Печать после удаления
	list.DeleteFromHead()
	list.DeleteFromTail()
	list.Print()
	list.PrintReverse()

	// Печать списка с одним элементом
	list2 := NewSinglyLinkedList()
	list2.AddToTail(42)
	list2.Print()
	list2.PrintReverse()
}

func TestSinglyLinkedList_SaveLoadTextFile(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Сохранение пустого списка
	err := list.SaveToFile("empty_singly.txt")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого списка: %v", err)
	}

	// 2. Загрузка пустого списка
	emptyList := NewSinglyLinkedList()
	err = emptyList.LoadFromFile("empty_singly.txt")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого списка: %v", err)
	}
	if emptyList.FindByValue(1) {
		t.Errorf("Пустой список не должен содержать элементов")
	}
	if emptyList.Head != nil {
		t.Errorf("Head пустого списка должен быть nil")
	}

	// 3. Сохранение непустого списка
	list.AddToTail(111)
	list.AddToTail(222)
	list.AddToTail(333)
	list.AddToTail(444)
	list.AddToTail(555)

	filename := "test_singly.txt"
	err = list.SaveToFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	// 4. Загрузка из файла
	newList := NewSinglyLinkedList()
	err = newList.LoadFromFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if newList.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", newList.GetSize())
	}

	// Проверка наличия элементов
	if !newList.FindByValue(111) || !newList.FindByValue(222) || !newList.FindByValue(333) ||
		!newList.FindByValue(444) || !newList.FindByValue(555) {
		t.Errorf("Элементы не найдены после загрузки")
	}

	// Проверка порядка
	expected := []int{111, 222, 333, 444, 555}
	current := newList.Head
	for i, val := range expected {
		if current == nil {
			t.Errorf("Недостаточно элементов в списке")
			break
		}
		if current.Key != val {
			t.Errorf("На позиции %d ожидался %d, получен %d", i, val, current.Key)
		}
		current = current.Next
	}

	// 5. Ошибка загрузки из несуществующего файла
	err = list.LoadFromFile("nonexistent_file.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	// 6. Загрузка из файла с некорректными данными
	badFile := "bad_singly.txt"
	f, _ := os.Create(badFile)
	f.WriteString("abc def ghi\n")
	f.Close()

	err = list.LoadFromFile(badFile)
	if err != nil {
		// Ошибка не обязательна, но файл должен загрузиться без добавления элементов
	}
	os.Remove(badFile)

	os.Remove(filename)
	os.Remove("empty_singly.txt")
}

func TestSinglyLinkedList_SaveLoadBinaryFile(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Сохранение пустого списка в бинарный файл
	err := list.SaveToBinaryFile("empty_singly.bin")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого списка в бинарный файл: %v", err)
	}

	// 2. Загрузка пустого списка из бинарного файла
	emptyList := NewSinglyLinkedList()
	err = emptyList.LoadFromBinaryFile("empty_singly.bin")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого списка из бинарного файла: %v", err)
	}
	if emptyList.FindByValue(1) {
		t.Errorf("Пустой список не должен содержать элементов")
	}
	if emptyList.Head != nil {
		t.Errorf("Head пустого списка должен быть nil")
	}

	// 3. Сохранение непустого списка
	list.AddToTail(555)
	list.AddToTail(666)
	list.AddToTail(777)
	list.AddToTail(888)
	list.AddToTail(999)

	filename := "test_singly.bin"
	err = list.SaveToBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	// 4. Загрузка из бинарного файла
	newList := NewSinglyLinkedList()
	err = newList.LoadFromBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if newList.GetSize() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", newList.GetSize())
	}

	// Проверка наличия элементов
	if !newList.FindByValue(555) || !newList.FindByValue(666) || !newList.FindByValue(777) ||
		!newList.FindByValue(888) || !newList.FindByValue(999) {
		t.Errorf("Элементы не найдены после загрузки")
	}

	// Проверка порядка
	expected := []int{555, 666, 777, 888, 999}
	current := newList.Head
	for i, val := range expected {
		if current == nil {
			t.Errorf("Недостаточно элементов в списке")
			break
		}
		if current.Key != val {
			t.Errorf("На позиции %d ожидался %d, получен %d", i, val, current.Key)
		}
		current = current.Next
	}

	// 5. Ошибка загрузки из несуществующего бинарного файла
	err = list.LoadFromBinaryFile("nonexistent_file.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего бинарного файла")
	}

	// 6. Тест с поврежденным бинарным файлом (недостаточно данных)
	badFile := "bad_singly.bin"
	f, _ := os.Create(badFile)
	f.Write([]byte{0, 0, 0, 5}) // Только размер, нет данных
	f.Close()

	err = list.LoadFromBinaryFile(badFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке поврежденного бинарного файла")
	}
	os.Remove(badFile)

	os.Remove(filename)
	os.Remove("empty_singly.bin")
}

func TestSinglyLinkedList_EdgeCases(t *testing.T) {
	list := NewSinglyLinkedList()

	// 1. Список с одним элементом - удаление из начала
	list.AddToHead(42)
	list.DeleteFromHead()
	if list.FindByValue(42) {
		t.Errorf("Элемент 42 должен быть удален")
	}
	if list.Head != nil {
		t.Errorf("Head должен быть nil")
	}

	// 2. Список с одним элементом - удаление из конца
	list.AddToTail(42)
	list.DeleteFromTail()
	if list.FindByValue(42) {
		t.Errorf("Элемент 42 должен быть удален")
	}
	if list.Head != nil {
		t.Errorf("Head должен быть nil")
	}

	// 3. Удаление по значению в списке с одним элементом
	list.AddToHead(100)
	list.DeleteByValue(100)
	if list.FindByValue(100) {
		t.Errorf("Элемент 100 должен быть удален")
	}

	// 4. Добавление после в списке с одним элементом
	list.AddToTail(200)
	list.AddAfter(200, 300)
	if !list.FindByValue(300) {
		t.Errorf("Элемент 300 должен существовать")
	}
	if list.Head.Next.Key != 300 {
		t.Errorf("Второй элемент должен быть 300, получен %d", list.Head.Next.Key)
	}

	// 5. Добавление перед в списке с одним элементом
	list2 := NewSinglyLinkedList()
	list2.AddToTail(400)
	list2.AddBefore(400, 350)
	if !list2.FindByValue(350) {
		t.Errorf("Элемент 350 должен существовать")
	}
	if list2.Head.Key != 350 {
		t.Errorf("Head должен быть 350, получен %d", list2.Head.Key)
	}
}

func TestSinglyLinkedList_LargeList(t *testing.T) {
	list := NewSinglyLinkedList()
	count := 1000

	// Добавление 1000 элементов
	for i := 0; i < count; i++ {
		list.AddToTail(i)
	}

	// Проверка наличия
	if !list.FindByValue(0) || !list.FindByValue(count/2) || !list.FindByValue(count-1) {
		t.Errorf("Элементы должны существовать")
	}

	// Проверка размера
	if list.GetSize() != count {
		t.Errorf("Ожидался размер %d, получен %d", count, list.GetSize())
	}

	// Удаление всех элементов
	for i := 0; i < count; i++ {
		list.DeleteByValue(i)
	}

	// Проверка отсутствия
	if list.FindByValue(0) || list.FindByValue(count-1) {
		t.Errorf("После удаления элементы не должны существовать")
	}
	if list.Head != nil {
		t.Errorf("После удаления всех элементов Head должен быть nil")
	}
}

func TestSinglyLinkedList_MultipleOperations(t *testing.T) {
	list := NewSinglyLinkedList()

	// Комбинация различных операций
	list.AddToHead(10)
	list.AddToTail(20)
	list.AddAfter(10, 15)
	list.AddBefore(20, 18)
	list.AddToHead(5)
	list.AddToTail(25)

	// Проверка наличия
	expected := []int{5, 10, 15, 18, 20, 25}
	for _, val := range expected {
		if !list.FindByValue(val) {
			t.Errorf("Элемент %d должен существовать", val)
		}
	}

	// Проверка порядка
	current := list.Head
	for i, val := range expected {
		if current == nil {
			t.Errorf("Недостаточно элементов")
			break
		}
		if current.Key != val {
			t.Errorf("На позиции %d ожидался %d, получен %d", i, val, current.Key)
		}
		current = current.Next
	}

	// Удаление и проверка
	list.DeleteByValue(15)
	list.DeleteAfter(10)
	list.DeleteBefore(20)

	if list.FindByValue(15) || list.FindByValue(18) || list.FindByValue(10) {
		t.Errorf("Элементы должны быть удалены")
	}

	// Очистка
	list.Clear()
	if list.Head != nil {
		t.Errorf("После очистки Head должен быть nil")
	}

	// Повторное добавление
	list.AddToHead(50)
	list.AddToTail(60)
	if !list.FindByValue(50) || !list.FindByValue(60) {
		t.Errorf("После очистки должны работать операции")
	}
}

// Вспомогательная функция для получения размера
func (s *SinglyLinkedList) GetSize() int {
	count := 0
	current := s.Head
	for current != nil {
		count++
		current = current.Next
	}
	return count
}
