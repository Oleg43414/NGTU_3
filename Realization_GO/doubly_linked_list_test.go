package main

import (
	"os"
	"testing"
)

func TestDoublyLinkedList_BasicOperations(t *testing.T) {
	list := NewDoublyLinkedList()

	list.AddToHead(10)
	list.AddToTail(20)
	list.AddToHead(5)
	list.AddToTail(25)

	if !list.FindByValue(5) {
		t.Errorf("Ожидалось найти 5")
	}
	if !list.FindByValue(10) {
		t.Errorf("Ожидалось найти 10")
	}
	if !list.FindByValue(20) {
		t.Errorf("Ожидалось найти 20")
	}
	if !list.FindByValue(25) {
		t.Errorf("Ожидалось найти 25")
	}
	if list.FindByValue(30) {
		t.Errorf("Не ожидалось найти 30")
	}

	// Проверка размера через подсчет
	count := 0
	current := list.Head
	for current != nil {
		count++
		current = current.Next
	}
	if count != 4 {
		t.Errorf("Ожидалось 4 элемента, получено %d", count)
	}
}

func TestDoublyLinkedList_AddAfterValue(t *testing.T) {
	list := NewDoublyLinkedList()

	// Добавление после в пустом списке
	list.AddAfterValue(2, 99) // Должно вывести "Список пуст!"

	list.AddToTail(1)
	list.AddToTail(2)
	list.AddToTail(3)
	list.AddToTail(4)

	// Добавление после существующего элемента
	list.AddAfterValue(2, 99)
	if !list.FindByValue(99) {
		t.Errorf("Элемент 99 должен существовать")
	}

	// Добавление после несуществующего элемента
	list.AddAfterValue(999, 88) // Должно вывести "Элемент 999 не найден!"
	if list.FindByValue(88) {
		t.Errorf("Элемент 88 не должен быть добавлен")
	}

	// Добавление после последнего элемента
	list.AddAfterValue(4, 77)
	if !list.FindByValue(77) {
		t.Errorf("Элемент 77 должен существовать")
	}

	// Проверка что хвост обновился
	if list.Tail.Key != 77 {
		t.Errorf("Хвост должен быть 77, получен %d", list.Tail.Key)
	}

	// Добавление после головы
	list.AddAfterValue(1, 55)
	if !list.FindByValue(55) {
		t.Errorf("Элемент 55 должен существовать")
	}
}

func TestDoublyLinkedList_AddBeforeValue(t *testing.T) {
	list := NewDoublyLinkedList()

	// Добавление перед в пустом списке
	list.AddBeforeValue(2, 99) // Должно вывести "Список пуст!"

	list.AddToTail(10)

	// Добавление перед первым элементом
	list.AddBeforeValue(10, 5)
	if !list.FindByValue(5) {
		t.Errorf("Элемент 5 должен существовать")
	}
	if list.Head.Key != 5 {
		t.Errorf("Голова должна быть 5, получена %d", list.Head.Key)
	}

	list.AddToTail(20)
	list.AddToTail(30)
	list.AddToTail(40)

	// Добавление перед средним элементом
	list.AddBeforeValue(20, 15)
	if !list.FindByValue(15) {
		t.Errorf("Элемент 15 должен существовать")
	}

	// Добавление перед последним элементом
	list.AddBeforeValue(40, 35)
	if !list.FindByValue(35) {
		t.Errorf("Элемент 35 должен существовать")
	}

	// Добавление перед несуществующим элементом
	list.AddBeforeValue(999, 88) // Должно вывести "Элемент 999 не найден!"
	if list.FindByValue(88) {
		t.Errorf("Элемент 88 не должен быть добавлен")
	}

	// Проверка что голова не изменилась при добавлении перед несуществующим
	if list.Head.Key != 5 {
		t.Errorf("Голова должна остаться 5")
	}
}

func TestDoublyLinkedList_DeleteFromHead(t *testing.T) {
	list := NewDoublyLinkedList()

	// Удаление из пустого списка
	list.DeleteFromHead() // Должно вывести "Список пуст!"

	// Удаление из списка с одним элементом
	list.AddToTail(1)
	list.DeleteFromHead()
	if list.FindByValue(1) {
		t.Errorf("Элемент 1 должен быть удален")
	}
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Список должен быть пуст")
	}

	// Удаление из списка с несколькими элементами
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
		t.Errorf("Новая голова должна быть 3, получена %d", list.Head.Key)
	}
}

func TestDoublyLinkedList_DeleteFromTail(t *testing.T) {
	list := NewDoublyLinkedList()

	// Удаление из пустого списка
	list.DeleteFromTail() // Должно вывести "Список пуст!"

	// Удаление из списка с одним элементом
	list.AddToTail(100)
	list.DeleteFromTail()
	if list.FindByValue(100) {
		t.Errorf("Элемент 100 должен быть удален")
	}
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Список должен быть пуст")
	}

	// Удаление из списка с несколькими элементами
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
	if list.Tail.Key != 300 {
		t.Errorf("Новый хвост должен быть 300, получен %d", list.Tail.Key)
	}
}

func TestDoublyLinkedList_DeleteByValue(t *testing.T) {
	list := NewDoublyLinkedList()

	// Удаление из пустого списка
	list.DeleteByValue(99) // Должно вывести "Список пуст!"

	list.AddToTail(1)
	list.AddToTail(2)
	list.AddToTail(3)
	list.AddToTail(4)
	list.AddToTail(5)

	// Удаление головы
	list.DeleteByValue(1)
	if list.FindByValue(1) {
		t.Errorf("Элемент 1 должен быть удален")
	}
	if list.Head.Key != 2 {
		t.Errorf("Новая голова должна быть 2, получена %d", list.Head.Key)
	}

	// Удаление среднего элемента
	list.DeleteByValue(3)
	if list.FindByValue(3) {
		t.Errorf("Элемент 3 должен быть удален")
	}

	// Удаление хвоста
	list.DeleteByValue(5)
	if list.FindByValue(5) {
		t.Errorf("Элемент 5 должен быть удален")
	}
	if list.Tail.Key != 4 {
		t.Errorf("Новый хвост должен быть 4, получен %d", list.Tail.Key)
	}

	// Удаление последнего элемента
	list.DeleteByValue(2)
	list.DeleteByValue(4)
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Список должен быть пуст")
	}

	// Удаление несуществующего элемента
	list.DeleteByValue(999) // Должно вывести "Элемент 999 не найден!"
}

func TestDoublyLinkedList_DeleteAfterValue(t *testing.T) {
	list := NewDoublyLinkedList()

	// Удаление после в пустом списке
	list.DeleteAfterValue(20) // Должно вывести "Список пуст!"

	list.AddToTail(10)
	list.AddToTail(20)
	list.AddToTail(30)
	list.AddToTail(40)
	list.AddToTail(50)

	// Удаление после существующего элемента
	list.DeleteAfterValue(20)
	if list.FindByValue(30) {
		t.Errorf("Элемент 30 должен быть удален")
	}

	// Удаление после несуществующего элемента
	list.DeleteAfterValue(999) // Должно вывести "Элемент 999 не найден!"

	// Удаление после последнего элемента
	list.DeleteAfterValue(50) // Должно вывести "После элемента 50 нет элементов для удаления!"

	// Удаление после в списке с одним элементом
	list.Clear()
	list.AddToTail(60)
	list.DeleteAfterValue(60) // Должно вывести "После элемента 60 нет элементов для удаления!"

	// Проверка что хвост корректно обновляется
	list.Clear()
	list.AddToTail(100)
	list.AddToTail(200)
	list.AddToTail(300)
	list.DeleteAfterValue(200) // Удаляем 300
	if list.Tail.Key != 200 {
		t.Errorf("Хвост должен быть 200, получен %d", list.Tail.Key)
	}
}

func TestDoublyLinkedList_DeleteBeforeValue(t *testing.T) {
	list := NewDoublyLinkedList()

	// Удаление перед в пустом списке
	list.DeleteBeforeValue(30) // Должно вывести "Список пуст!"

	list.AddToTail(10)
	list.AddToTail(20)
	list.AddToTail(30)
	list.AddToTail(40)
	list.AddToTail(50)

	// Удаление перед существующим элементом (не первым)
	list.DeleteBeforeValue(30)
	if list.FindByValue(20) {
		t.Errorf("Элемент 20 должен быть удален")
	}

	// Удаление перед первым элементом
	list.DeleteBeforeValue(10) // Должно вывести "Перед элементом 10 нет элементов для удаления!"

	// Удаление перед несуществующим элементом
	list.DeleteBeforeValue(999) // Должно вывести "Элемент 999 не найден!"

	// Удаление перед вторым элементом
	list.Clear()
	list.AddToTail(100)
	list.AddToTail(200)
	list.DeleteBeforeValue(200)
	if list.FindByValue(100) {
		t.Errorf("Элемент 100 должен быть удален")
	}
	if list.Head.Key != 200 {
		t.Errorf("Голова должна быть 200, получена %d", list.Head.Key)
	}

	// Удаление перед в списке с одним элементом
	list.Clear()
	list.AddToTail(500)
	list.DeleteBeforeValue(500) // Должно вывести "Список пуст! Невозможно удалить элемент."
}

func TestDoublyLinkedList_PrintFunctions(t *testing.T) {
	list := NewDoublyLinkedList()

	// Перехватываем stdout для проверки
	old := os.Stdout

	// Печать пустого списка
	list.PrintForward()
	list.PrintBackward()

	// Печать непустого списка
	list.AddToTail(1)
	list.AddToTail(2)
	list.AddToTail(3)
	list.AddToTail(4)
	list.AddToTail(5)

	list.PrintForward()
	list.PrintBackward()

	// Печать после удаления
	list.DeleteFromHead()
	list.DeleteFromTail()
	list.PrintForward()
	list.PrintBackward()

	// Восстанавливаем stdout
	os.Stdout = old
}

func TestDoublyLinkedList_ClearList(t *testing.T) {
	list := NewDoublyLinkedList()

	// Очистка пустого списка
	list.Clear()
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Пустой список после очистки должен быть пуст")
	}

	// Очистка непустого списка
	list.AddToTail(100)
	list.AddToTail(200)
	list.AddToTail(300)
	list.AddToTail(400)
	list.AddToTail(500)

	list.Clear()

	if list.FindByValue(100) || list.FindByValue(200) || list.FindByValue(300) ||
		list.FindByValue(400) || list.FindByValue(500) {
		t.Errorf("После очистки элементы не должны существовать")
	}
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Head и Tail должны быть nil после очистки")
	}

	// Проверка что после очистки можно добавлять
	list.AddToTail(600)
	if !list.FindByValue(600) {
		t.Errorf("После очистки должен работать AddToTail")
	}
	if list.Head.Key != 600 || list.Tail.Key != 600 {
		t.Errorf("Head и Tail должны быть 600")
	}
}

func TestDoublyLinkedList_SaveLoadTextFile(t *testing.T) {
	list := NewDoublyLinkedList()

	// Сохранение пустого списка
	err := list.SaveToFile("empty_doubly.txt")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого списка: %v", err)
	}

	// Загрузка пустого списка
	emptyList := NewDoublyLinkedList()
	err = emptyList.LoadFromFile("empty_doubly.txt")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого списка: %v", err)
	}
	if emptyList.FindByValue(1) {
		t.Errorf("Пустой список не должен содержать элементов")
	}
	if emptyList.Head != nil || emptyList.Tail != nil {
		t.Errorf("Head и Tail должны быть nil")
	}

	// Сохранение непустого списка
	list.AddToTail(111)
	list.AddToTail(222)
	list.AddToTail(333)
	list.AddToTail(444)
	list.AddToTail(555)

	filename := "test_doubly.txt"
	err = list.SaveToFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	// Загрузка из файла
	newList := NewDoublyLinkedList()
	err = newList.LoadFromFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if !newList.FindByValue(111) || !newList.FindByValue(222) || !newList.FindByValue(333) ||
		!newList.FindByValue(444) || !newList.FindByValue(555) {
		t.Errorf("Элементы не найдены после загрузки")
	}

	// Проверка порядка элементов
	current := newList.Head
	expected := []int{111, 222, 333, 444, 555}
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

	// Проверка обратных ссылок
	current = newList.Tail
	for i := len(expected) - 1; i >= 0; i-- {
		if current == nil {
			t.Errorf("Недостаточно элементов при обратном обходе")
			break
		}
		if current.Key != expected[i] {
			t.Errorf("При обратном обходе на позиции %d ожидался %d, получен %d", i, expected[i], current.Key)
		}
		current = current.Prev
	}

	// Ошибка загрузки из несуществующего файла
	err = list.LoadFromFile("nonexistent_file.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	os.Remove(filename)
	os.Remove("empty_doubly.txt")
}

func TestDoublyLinkedList_SaveLoadBinaryFile(t *testing.T) {
	list := NewDoublyLinkedList()

	// Сохранение пустого списка
	err := list.SaveToBinaryFile("empty_doubly.bin")
	if err != nil {
		t.Errorf("Ошибка сохранения пустого списка в бинарный файл: %v", err)
	}

	// Загрузка пустого списка
	emptyList := NewDoublyLinkedList()
	err = emptyList.LoadFromBinaryFile("empty_doubly.bin")
	if err != nil {
		t.Errorf("Ошибка загрузки пустого списка из бинарного файла: %v", err)
	}
	if emptyList.FindByValue(1) {
		t.Errorf("Пустой список не должен содержать элементов")
	}
	if emptyList.Head != nil || emptyList.Tail != nil {
		t.Errorf("Head и Tail должны быть nil")
	}

	// Сохранение непустого списка
	list.AddToTail(111)
	list.AddToTail(222)
	list.AddToTail(333)
	list.AddToTail(444)
	list.AddToTail(555)

	filename := "test_doubly.bin"
	err = list.SaveToBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	// Загрузка из бинарного файла
	newList := NewDoublyLinkedList()
	err = newList.LoadFromBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if !newList.FindByValue(111) || !newList.FindByValue(222) || !newList.FindByValue(333) ||
		!newList.FindByValue(444) || !newList.FindByValue(555) {
		t.Errorf("Элементы не найдены после загрузки")
	}

	// Проверка порядка элементов
	current := newList.Head
	expected := []int{111, 222, 333, 444, 555}
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

	// Проверка обратных ссылок
	current = newList.Tail
	for i := len(expected) - 1; i >= 0; i-- {
		if current == nil {
			t.Errorf("Недостаточно элементов при обратном обходе")
			break
		}
		if current.Key != expected[i] {
			t.Errorf("При обратном обходе на позиции %d ожидался %d, получен %d", i, expected[i], current.Key)
		}
		current = current.Prev
	}

	// Ошибка загрузки из несуществующего файла
	err = list.LoadFromBinaryFile("nonexistent_file.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего бинарного файла")
	}

	os.Remove(filename)
	os.Remove("empty_doubly.bin")
}

func TestDoublyLinkedList_EdgeCases(t *testing.T) {
	list := NewDoublyLinkedList()

	// Все операции с пустым списком не должны паниковать
	list.DeleteFromHead()
	list.DeleteFromTail()
	list.DeleteByValue(99)
	list.DeleteAfterValue(10)
	list.DeleteBeforeValue(10)

	// Добавление в пустой список - проверка head и tail
	list.AddToHead(1)
	if list.Head.Key != 1 || list.Tail.Key != 1 {
		t.Errorf("Head и Tail должны быть 1")
	}
	list.Clear()

	list.AddToTail(1)
	if list.Head.Key != 1 || list.Tail.Key != 1 {
		t.Errorf("Head и Tail должны быть 1")
	}
	list.Clear()

	// Добавление после/перед в пустом списке
	list.AddAfterValue(1, 2)
	list.AddBeforeValue(1, 2)

	// Список с одним элементом - удаление из начала
	list.AddToHead(42)
	list.DeleteFromHead()
	if list.FindByValue(42) {
		t.Errorf("Элемент 42 должен быть удален")
	}
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Список должен быть пуст")
	}

	// Список с одним элементом - удаление из конца
	list.AddToTail(42)
	list.DeleteFromTail()
	if list.FindByValue(42) {
		t.Errorf("Элемент 42 должен быть удален")
	}
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Список должен быть пуст")
	}

	// Удаление по значению в списке с одним элементом
	list.AddToHead(100)
	list.DeleteByValue(100)
	if list.FindByValue(100) {
		t.Errorf("Элемент 100 должен быть удален")
	}

	// Проверка что после всех операций список пуст
	if list.Head != nil || list.Tail != nil {
		t.Errorf("Список должен быть пуст")
	}
}

func TestDoublyLinkedList_LargeListOperations(t *testing.T) {
	list := NewDoublyLinkedList()
	count := 100

	// Добавление 100 элементов
	for i := 0; i < count; i++ {
		list.AddToTail(i)
	}

	// Проверка наличия всех элементов
	for i := 0; i < count; i++ {
		if !list.FindByValue(i) {
			t.Errorf("Элемент %d должен существовать", i)
		}
	}

	// Проверка head и tail
	if list.Head.Key != 0 {
		t.Errorf("Head должен быть 0, получен %d", list.Head.Key)
	}
	if list.Tail.Key != count-1 {
		t.Errorf("Tail должен быть %d, получен %d", count-1, list.Tail.Key)
	}

	// Удаление первых 50 элементов
	for i := 0; i < count/2; i++ {
		list.DeleteByValue(i)
	}

	// Проверка удаленных элементов
	for i := 0; i < count/2; i++ {
		if list.FindByValue(i) {
			t.Errorf("Элемент %d должен быть удален", i)
		}
	}

	// Проверка оставшихся элементов
	for i := count / 2; i < count; i++ {
		if !list.FindByValue(i) {
			t.Errorf("Элемент %d должен существовать", i)
		}
	}

	// Проверка head после удаления
	if list.Head.Key != count/2 {
		t.Errorf("Head должен быть %d, получен %d", count/2, list.Head.Key)
	}

	list.Clear()
	if list.FindByValue(50) {
		t.Errorf("После очистки элементы должны быть удалены")
	}
	if list.Head != nil || list.Tail != nil {
		t.Errorf("После очистки Head и Tail должны быть nil")
	}
}

func TestDoublyLinkedList_EmptyListFileOperations(t *testing.T) {
	list := NewDoublyLinkedList()

	txtFile := "test_doubly_empty.txt"
	binFile := "test_doubly_empty.bin"

	// Сохранение пустого списка
	err := list.SaveToFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустого списка: %v", err)
	}

	// Проверка что файл создан
	if _, err := os.Stat(txtFile); os.IsNotExist(err) {
		t.Errorf("Файл %s не был создан", txtFile)
	}

	err = list.SaveToBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка сохранения пустого списка в бинарный файл: %v", err)
	}

	// Проверка что бинарный файл создан
	if _, err := os.Stat(binFile); os.IsNotExist(err) {
		t.Errorf("Файл %s не был создан", binFile)
	}

	// Загрузка пустого списка из текстового файла
	emptyList := NewDoublyLinkedList()
	err = emptyList.LoadFromFile(txtFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустого списка: %v", err)
	}
	if emptyList.FindByValue(1) {
		t.Errorf("Пустой список не должен содержать элементов")
	}
	if emptyList.Head != nil || emptyList.Tail != nil {
		t.Errorf("Head и Tail должны быть nil")
	}

	// Загрузка пустого списка из бинарного файла
	err = emptyList.LoadFromBinaryFile(binFile)
	if err != nil {
		t.Errorf("Ошибка загрузки пустого списка из бинарного файла: %v", err)
	}
	if emptyList.FindByValue(1) {
		t.Errorf("Пустой список не должен содержать элементов")
	}
	if emptyList.Head != nil || emptyList.Tail != nil {
		t.Errorf("Head и Tail должны быть nil")
	}

	os.Remove(txtFile)
	os.Remove(binFile)
}

func TestDoublyLinkedList_MultipleOperations(t *testing.T) {
	list := NewDoublyLinkedList()

	// Комбинация различных операций
	list.AddToHead(10)
	list.AddToTail(20)
	list.AddAfterValue(10, 15)
	list.AddBeforeValue(20, 18)

	if !list.FindByValue(10) || !list.FindByValue(15) || !list.FindByValue(18) || !list.FindByValue(20) {
		t.Errorf("Все элементы должны существовать")
	}

	// Проверка порядка: 10 -> 15 -> 18 -> 20
	expected := []int{10, 15, 18, 20}
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

	// Удаление и добавление
	list.DeleteByValue(15)
	list.AddAfterValue(10, 16)

	if list.FindByValue(15) {
		t.Errorf("Элемент 15 должен быть удален")
	}
	if !list.FindByValue(16) {
		t.Errorf("Элемент 16 должен существовать")
	}

	// Очистка и повторное добавление
	list.Clear()
	list.AddToHead(50)
	list.AddToTail(60)

	if list.Head.Key != 50 || list.Tail.Key != 60 {
		t.Errorf("Head должен быть 50, Tail должен быть 60")
	}
	if list.Head.Next != list.Tail || list.Tail.Prev != list.Head {
		t.Errorf("Связи между элементами нарушены")
	}
}

func TestDoublyLinkedList_FindByValueEdgeCases(t *testing.T) {
	list := NewDoublyLinkedList()

	// Поиск в пустом списке
	if list.FindByValue(42) {
		t.Errorf("В пустом списке не должно быть элементов")
	}

	// Поиск существующего элемента
	list.AddToTail(100)
	list.AddToTail(200)
	list.AddToTail(300)

	if !list.FindByValue(100) {
		t.Errorf("Элемент 100 должен быть найден")
	}
	if !list.FindByValue(200) {
		t.Errorf("Элемент 200 должен быть найден")
	}
	if !list.FindByValue(300) {
		t.Errorf("Элемент 300 должен быть найден")
	}

	// Поиск несуществующего элемента
	if list.FindByValue(999) {
		t.Errorf("Элемент 999 не должен быть найден")
	}

	// Поиск после удаления
	list.DeleteByValue(200)
	if list.FindByValue(200) {
		t.Errorf("Элемент 200 не должен быть найден после удаления")
	}
}

func TestDoublyLinkedList_HeadTailConsistency(t *testing.T) {
	list := NewDoublyLinkedList()

	// Проверка согласованности head и tail

	// Пустой список
	if list.Head != nil || list.Tail != nil {
		t.Errorf("В пустом списке head и tail должны быть nil")
	}

	// Один элемент
	list.AddToHead(42)
	if list.Head != list.Tail {
		t.Errorf("В списке с одним элементом head и tail должны указывать на один узел")
	}
	if list.Head.Key != 42 || list.Tail.Key != 42 {
		t.Errorf("Key должен быть 42")
	}

	// Два элемента - добавление в начало
	list.AddToHead(41)
	if list.Head.Key != 41 || list.Tail.Key != 42 {
		t.Errorf("Head должен быть 41, Tail должен быть 42")
	}
	if list.Head.Next != list.Tail || list.Tail.Prev != list.Head {
		t.Errorf("Связи между элементами нарушены")
	}

	// Два элемента - добавление в конец
	list.Clear()
	list.AddToTail(42)
	list.AddToTail(43)
	if list.Head.Key != 42 || list.Tail.Key != 43 {
		t.Errorf("Head должен быть 42, Tail должен быть 43")
	}
	if list.Head.Next != list.Tail || list.Tail.Prev != list.Head {
		t.Errorf("Связи между элементами нарушены")
	}

	// Удаление из начала в списке с двумя элементами
	list.DeleteFromHead()
	if list.Head.Key != 43 || list.Tail.Key != 43 {
		t.Errorf("После удаления head и tail должны быть 43")
	}
	if list.Head != list.Tail {
		t.Errorf("Head и Tail должны указывать на один узел")
	}

	// Удаление из конца в списке с двумя элементами
	list.Clear()
	list.AddToTail(42)
	list.AddToTail(43)
	list.DeleteFromTail()
	if list.Head.Key != 42 || list.Tail.Key != 42 {
		t.Errorf("После удаления head и tail должны быть 42")
	}
	if list.Head != list.Tail {
		t.Errorf("Head и Tail должны указывать на один узел")
	}
}
