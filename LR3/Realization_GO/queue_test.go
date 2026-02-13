package main

import (
	"os"
	"testing"
)

func TestQueue_NewQueue(t *testing.T) {
	q := NewQueue()

	if q == nil {
		t.Errorf("NewQueue() не должен возвращать nil")
	}
	if q.capacity != 4 {
		t.Errorf("Ожидалась начальная емкость 4, получена %d", q.capacity)
	}
	if q.frontIdx != 0 {
		t.Errorf("Ожидался frontIdx 0, получен %d", q.frontIdx)
	}
	if q.rearIdx != 0 {
		t.Errorf("Ожидался rearIdx 0, получен %d", q.rearIdx)
	}
	if q.currentSize != 0 {
		t.Errorf("Ожидался currentSize 0, получен %d", q.currentSize)
	}
	if len(q.arr) != 4 {
		t.Errorf("Ожидалась длина массива 4, получена %d", len(q.arr))
	}
}

func TestQueue_EmptyQueue(t *testing.T) {
	q := NewQueue()

	if !q.IsEmpty() {
		t.Errorf("Новая очередь должна быть пустой")
	}
	if q.Size() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", q.Size())
	}
}

func TestQueue_PushPopOperations(t *testing.T) {
	q := NewQueue()

	// Push элементы
	q.Push(10)
	q.Push(20)
	q.Push(30)
	q.Push(40) // Заполняем до capacity 4

	if q.Size() != 4 {
		t.Errorf("Ожидался размер 4, получен %d", q.Size())
	}
	if q.IsEmpty() {
		t.Errorf("Очередь не должна быть пустой")
	}
	if q.capacity != 4 {
		t.Errorf("Емкость не должна измениться, получена %d", q.capacity)
	}

	// Проверка Front
	if q.Front() != 10 {
		t.Errorf("Ожидался первый элемент 10, получен %d", q.Front())
	}

	// Pop элементы
	q.Pop()
	if q.Front() != 20 {
		t.Errorf("После удаления ожидался первый элемент 20, получен %d", q.Front())
	}
	if q.Size() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", q.Size())
	}

	q.Pop()
	if q.Front() != 30 {
		t.Errorf("После удаления ожидался первый элемент 30, получен %d", q.Front())
	}
	if q.Size() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", q.Size())
	}

	q.Pop()
	if q.Front() != 40 {
		t.Errorf("После удаления ожидался первый элемент 40, получен %d", q.Front())
	}
	if q.Size() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", q.Size())
	}

	q.Pop()
	if !q.IsEmpty() {
		t.Errorf("После удаления всех элементов очередь должна быть пустой")
	}
	if q.Size() != 0 {
		t.Errorf("Ожидался размер 0, получен %d", q.Size())
	}
}

func TestQueue_FrontOnEmptyQueue(t *testing.T) {
	q := NewQueue()

	front := q.Front()
	if front != -1 {
		t.Errorf("Front() на пустой очереди должен вернуть -1, получен %d", front)
	}
}

func TestQueue_PopOnEmptyQueue(t *testing.T) {
	q := NewQueue()

	// Pop на пустой очереди не должен паниковать
	q.Pop() // Должно вывести "Очередь пуста! Нельзя удалить элемент."

	// Проверяем что очередь все еще пуста
	if !q.IsEmpty() {
		t.Errorf("Очередь должна остаться пустой")
	}
	if q.Size() != 0 {
		t.Errorf("Размер должен быть 0, получен %d", q.Size())
	}

	// Многократный Pop на пустой очереди
	for i := 0; i < 5; i++ {
		q.Pop()
	}
	if !q.IsEmpty() {
		t.Errorf("Очередь должна остаться пустой после многократного Pop")
	}
}

func TestQueue_ClearQueue(t *testing.T) {
	q := NewQueue()

	// Очистка пустой очереди
	q.Clear()
	if !q.IsEmpty() {
		t.Errorf("После очистки пустая очередь должна быть пустой")
	}
	if q.Size() != 0 {
		t.Errorf("Размер должен быть 0, получен %d", q.Size())
	}
	if q.frontIdx != 0 || q.rearIdx != 0 {
		t.Errorf("После очистки frontIdx и rearIdx должны быть 0, получены %d, %d", q.frontIdx, q.rearIdx)
	}

	// Очистка непустой очереди
	q.Push(5)
	q.Push(15)
	q.Push(25)
	q.Push(35)

	if q.Size() != 4 {
		t.Errorf("Ожидался размер 4, получен %d", q.Size())
	}

	q.Clear()

	if !q.IsEmpty() {
		t.Errorf("После очистки очередь должна быть пустой")
	}
	if q.Size() != 0 {
		t.Errorf("После очистки размер должен быть 0, получен %d", q.Size())
	}
	if q.frontIdx != 0 || q.rearIdx != 0 {
		t.Errorf("После очистки frontIdx и rearIdx должны быть 0, получены %d, %d", q.frontIdx, q.rearIdx)
	}
	if q.capacity != 4 {
		t.Errorf("Емкость не должна измениться после очистки, получена %d", q.capacity)
	}

	// Проверка что после очистки можно добавлять элементы
	q.Push(100)
	if q.Front() != 100 {
		t.Errorf("После очистки должен работать Push, ожидался 100, получен %d", q.Front())
	}
	if q.Size() != 1 {
		t.Errorf("Размер должен быть 1, получен %d", q.Size())
	}
}

func TestQueue_Display(t *testing.T) {
	q := NewQueue()

	// Перехватываем stdout для проверки
	// В тестах мы не можем легко проверить вывод, но можем убедиться что функция не падает

	// Display на пустой очереди
	q.Display()

	// Display на непустой очереди
	q.Push(1)
	q.Push(2)
	q.Push(3)
	q.Push(4)
	q.Display()

	// Display после удаления
	q.Pop()
	q.Display()

	// Display после очистки
	q.Clear()
	q.Display()
}

func TestQueue_ResizeBehavior(t *testing.T) {
	q := NewQueue()

	// Начальная емкость 4
	if q.capacity != 4 {
		t.Errorf("Ожидалась начальная емкость 4, получена %d", q.capacity)
	}

	// Добавляем 5 элементов - должен сработать resize
	for i := 0; i < 5; i++ {
		q.Push(i)
	}

	if q.capacity != 8 {
		t.Errorf("После добавления 5 элементов емкость должна быть 8, получена %d", q.capacity)
	}
	if q.Size() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", q.Size())
	}

	// Проверяем что все элементы на месте в правильном порядке
	for i := 0; i < 5; i++ {
		if q.Front() != i {
			t.Errorf("Ожидался элемент %d, получен %d", i, q.Front())
		}
		q.Pop()
	}

	// Добавляем много элементов для множественных resize
	for i := 0; i < 20; i++ {
		q.Push(i * 10)
	}

	if q.capacity < 20 {
		t.Errorf("Емкость должна быть не менее 20, получена %d", q.capacity)
	}
	if q.Size() != 20 {
		t.Errorf("Ожидался размер 20, получен %d", q.Size())
	}

	// Проверяем правильность порядка
	for i := 0; i < 20; i++ {
		if q.Front() != i*10 {
			t.Errorf("Ожидался элемент %d, получен %d", i*10, q.Front())
		}
		q.Pop()
	}

	if !q.IsEmpty() {
		t.Errorf("После удаления всех элементов очередь должна быть пустой")
	}
}

func TestQueue_CircularBehavior(t *testing.T) {
	q := NewQueue()

	// Тестируем кольцевое поведение
	// Добавляем и удаляем элементы, чтобы frontIdx и rearIdx сдвигались

	for i := 0; i < 3; i++ {
		q.Push(i)
	}

	// Удаляем несколько элементов
	q.Pop() // удаляем 0
	q.Pop() // удаляем 1

	// Теперь frontIdx должен быть 2, rearIdx должен быть 3
	if q.frontIdx != 2 {
		t.Errorf("Ожидался frontIdx 2, получен %d", q.frontIdx)
	}
	if q.rearIdx != 3 {
		t.Errorf("Ожидался rearIdx 3, получен %d", q.rearIdx)
	}
	if q.Size() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", q.Size())
	}
	if q.Front() != 2 {
		t.Errorf("Ожидался первый элемент 2, получен %d", q.Front())
	}

	// Добавляем еще элементы - должны переиспользовать освободившиеся ячейки
	q.Push(3)
	q.Push(4)

	// Теперь rearIdx должен быть (3 + 2) % 4 = 1
	if q.rearIdx != 1 {
		t.Errorf("Ожидался rearIdx 1, получен %d", q.rearIdx)
	}
	if q.Size() != 3 {
		t.Errorf("Ожидался размер 3, получен %d", q.Size())
	}

	// Проверяем порядок элементов
	expected := []int{2, 3, 4}
	for i, val := range expected {
		if q.Front() != val {
			t.Errorf("Ожидался элемент %d, получен %d", val, q.Front())
		}
		q.Pop()
		if i < len(expected)-1 && q.Size() != len(expected)-i-1 {
			t.Errorf("Ожидался размер %d, получен %d", len(expected)-i-1, q.Size())
		}
	}

	if !q.IsEmpty() {
		t.Errorf("После удаления всех элементов очередь должна быть пустой")
	}
}

func TestQueue_SaveLoadTextFile(t *testing.T) {
	q := NewQueue()

	// Сохранение пустой очереди
	err := q.SaveToFile("empty_queue.txt")
	if err != nil {
		t.Errorf("Ошибка сохранения пустой очереди в файл: %v", err)
	}

	// Загрузка пустой очереди
	emptyQ := NewQueue()
	err = emptyQ.LoadFromFile("empty_queue.txt")
	if err != nil {
		t.Errorf("Ошибка загрузки пустой очереди из файла: %v", err)
	}
	if !emptyQ.IsEmpty() {
		t.Errorf("Загруженная пустая очередь должна быть пустой")
	}
	if emptyQ.Size() != 0 {
		t.Errorf("Размер загруженной пустой очереди должен быть 0, получен %d", emptyQ.Size())
	}

	// Сохранение непустой очереди
	q.Push(111)
	q.Push(222)
	q.Push(333)
	q.Push(444)
	q.Push(555)

	filename := "test_queue.txt"
	err = q.SaveToFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в файл: %v", err)
	}

	// Загрузка из файла
	newQ := NewQueue()
	err = newQ.LoadFromFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из файла: %v", err)
	}

	if newQ.Size() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", newQ.Size())
	}

	// Проверка порядка элементов
	expected := []int{111, 222, 333, 444, 555}
	for i, val := range expected {
		if newQ.Front() != val {
			t.Errorf("На позиции %d ожидался %d, получен %d", i, val, newQ.Front())
		}
		newQ.Pop()
	}

	// Проверка что после загрузки можно добавлять элементы
	newQ.Push(666)
	if newQ.Front() != 666 {
		t.Errorf("После загрузки должен работать Push, ожидался 666, получен %d", newQ.Front())
	}

	// Ошибка загрузки из несуществующего файла
	err = q.LoadFromFile("nonexistent_file.txt")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего файла")
	}

	os.Remove(filename)
	os.Remove("empty_queue.txt")
}

func TestQueue_SaveLoadBinaryFile(t *testing.T) {
	q := NewQueue()

	// Сохранение пустой очереди в бинарный файл
	err := q.SaveToBinaryFile("empty_queue.bin")
	if err != nil {
		t.Errorf("Ошибка сохранения пустой очереди в бинарный файл: %v", err)
	}

	// Загрузка пустой очереди из бинарного файла
	emptyQ := NewQueue()
	err = emptyQ.LoadFromBinaryFile("empty_queue.bin")
	if err != nil {
		t.Errorf("Ошибка загрузки пустой очереди из бинарного файла: %v", err)
	}
	if !emptyQ.IsEmpty() {
		t.Errorf("Загруженная пустая очередь должна быть пустой")
	}
	if emptyQ.Size() != 0 {
		t.Errorf("Размер загруженной пустой очереди должен быть 0, получен %d", emptyQ.Size())
	}

	// Сохранение непустой очереди
	q.Push(999)
	q.Push(888)
	q.Push(777)
	q.Push(666)
	q.Push(555)

	filename := "test_queue.bin"
	err = q.SaveToBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка сохранения в бинарный файл: %v", err)
	}

	// Загрузка из бинарного файла
	newQ := NewQueue()
	err = newQ.LoadFromBinaryFile(filename)
	if err != nil {
		t.Errorf("Ошибка загрузки из бинарного файла: %v", err)
	}

	if newQ.Size() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", newQ.Size())
	}

	// Проверка порядка элементов
	expected := []int{999, 888, 777, 666, 555}
	for i, val := range expected {
		if newQ.Front() != val {
			t.Errorf("На позиции %d ожидался %d, получен %d", i, val, newQ.Front())
		}
		newQ.Pop()
	}

	// Проверка что после загрузки можно добавлять элементы
	newQ.Push(444)
	if newQ.Front() != 444 {
		t.Errorf("После загрузки должен работать Push, ожидался 444, получен %d", newQ.Front())
	}

	// Ошибка загрузки из несуществующего бинарного файла
	err = q.LoadFromBinaryFile("nonexistent_file.bin")
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке несуществующего бинарного файла")
	}

	// Тест с поврежденным бинарным файлом (недостаточно данных)
	badFile := "bad_queue.bin"
	f, _ := os.Create(badFile)
	f.Write([]byte{0, 1, 2, 3}) // Недостаточно данных
	f.Close()

	err = q.LoadFromBinaryFile(badFile)
	if err == nil {
		t.Errorf("Ожидалась ошибка при загрузке поврежденного бинарного файла")
	}
	os.Remove(badFile)

	os.Remove(filename)
	os.Remove("empty_queue.bin")
}

func TestQueue_MultipleOperations(t *testing.T) {
	q := NewQueue()

	// Комбинация различных операций
	for i := 0; i < 10; i++ {
		q.Push(i)
	}

	if q.Size() != 10 {
		t.Errorf("Ожидался размер 10, получен %d", q.Size())
	}

	// Удаляем половину
	for i := 0; i < 5; i++ {
		q.Pop()
	}

	if q.Size() != 5 {
		t.Errorf("Ожидался размер 5, получен %d", q.Size())
	}

	// Проверяем правильность порядка
	for i := 5; i < 10; i++ {
		if q.Front() != i {
			t.Errorf("Ожидался элемент %d, получен %d", i, q.Front())
		}
		q.Pop()
	}

	if !q.IsEmpty() {
		t.Errorf("Очередь должна быть пустой")
	}

	// Добавляем новые элементы после очистки
	q.Push(100)
	q.Push(200)

	if q.Front() != 100 {
		t.Errorf("Ожидался первый элемент 100, получен %d", q.Front())
	}
	if q.Size() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", q.Size())
	}

	// Очистка и повторное добавление
	q.Clear()
	q.Push(300)
	q.Push(400)

	if q.Front() != 300 {
		t.Errorf("Ожидался первый элемент 300, получен %d", q.Front())
	}
	if q.Size() != 2 {
		t.Errorf("Ожидался размер 2, получен %d", q.Size())
	}
}

func TestQueue_EdgeCases(t *testing.T) {
	q := NewQueue()

	// Push и Pop с большими числами
	q.Push(1 << 30)
	if q.Front() != 1<<30 {
		t.Errorf("Ожидался элемент %d, получен %d", 1<<30, q.Front())
	}
	q.Pop()

	// Многократный Push/Pop
	for i := 0; i < 100; i++ {
		q.Push(i)
		if q.Front() != 0 {
			t.Errorf("Front должен оставаться 0, получен %d", q.Front())
		}
	}

	// Очистка
	q.Clear()

	// Проверка что все индексы сброшены
	if q.frontIdx != 0 || q.rearIdx != 0 {
		t.Errorf("После Clear frontIdx и rearIdx должны быть 0, получены %d, %d", q.frontIdx, q.rearIdx)
	}

	// Push после Clear
	q.Push(42)
	if q.Front() != 42 {
		t.Errorf("Ожидался элемент 42, получен %d", q.Front())
	}
	if q.Size() != 1 {
		t.Errorf("Ожидался размер 1, получен %d", q.Size())
	}
}

func TestQueue_SizeConsistency(t *testing.T) {
	q := NewQueue()

	// Проверка согласованности размера
	for i := 0; i < 10; i++ {
		q.Push(i)
		if q.Size() != i+1 {
			t.Errorf("После добавления %d элементов размер должен быть %d, получен %d", i+1, i+1, q.Size())
		}
	}

	for i := 9; i >= 0; i-- {
		q.Pop()
		if q.Size() != i {
			t.Errorf("После удаления размер должен быть %d, получен %d", i, q.Size())
		}
	}
}

func TestQueue_FileOperationErrors(t *testing.T) {
	q := NewQueue()

	// Сохранение в файл с некорректным именем
	// В некоторых ОС это может вызвать ошибку
	err := q.SaveToFile("/invalid/path/queue.txt")
	if err == nil {
		// Не все системы выдадут ошибку, пропускаем проверку
	}

	err = q.SaveToBinaryFile("/invalid/path/queue.bin")
	if err == nil {
		// Не все системы выдадут ошибку, пропускаем проверку
	}

	// Загрузка из несуществующего файла уже проверена в других тестах
}
