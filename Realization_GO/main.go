package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

// Функции демонстрации

func demonstrateFullBinaryTree() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ ПОЛНОГО БИНАРНОГО ДЕРЕВА ---")

	tree := NewFullBinaryTree()

	// Добавление элементов
	tree.Add(10)
	tree.Add(5)
	tree.Add(15)
	tree.Add(3)
	tree.Add(7)
	tree.Add(12)
	tree.Add(18)

	// Обходы
	tree.PrintInOrder()
	tree.PrintPreOrder()
	tree.PrintPostOrder()
	tree.PrintLevelOrder()

	// Поиск
	tree.Search(7)
	tree.Search(100)

	// Проверка полноты
	tree.IsFull()

	// Информация
	fmt.Printf("Количество узлов: %d\n", tree.GetNodeCount())
	fmt.Printf("Высота дерева: %d\n", tree.GetTreeHeight())

	// Сохранение/загрузка
	tree.SaveToFile("tree.txt")

	tree2 := NewFullBinaryTree()
	tree2.LoadFromFile("tree.txt")
	tree2.PrintLevelOrder()
}

func demonstrateSinglyLinkedList() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ ОДНОСВЯЗНОГО СПИСКА ---")

	list := NewSinglyLinkedList()

	// Добавление элементов
	list.AddToHead(30)
	list.AddToHead(20)
	list.AddToHead(10)
	list.AddToTail(40)
	list.AddToTail(50)

	// Печать
	list.Print()
	list.PrintReverse()

	// Добавление между элементами
	list.AddAfter(30, 35)
	list.AddBefore(40, 38)
	list.Print()

	// Удаление
	list.DeleteFromHead()
	list.DeleteFromTail()
	list.DeleteByValue(35)
	list.Print()

	// Поиск
	fmt.Printf("Поиск 38: %v\n", list.FindByValue(38))
	fmt.Printf("Поиск 100: %v\n", list.FindByValue(100))

	// Сохранение/загрузка
	list.SaveToFile("singly_list.txt")

	list2 := NewSinglyLinkedList()
	list2.LoadFromFile("singly_list.txt")
	list2.Print()
}

func demonstrateDoublyLinkedList() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ ДВУСВЯЗНОГО СПИСКА ---")

	dlist := NewDoublyLinkedList()

	// Добавление элементов
	dlist.AddToHead(30)
	dlist.AddToHead(20)
	dlist.AddToHead(10)
	dlist.AddToTail(40)
	dlist.AddToTail(50)

	// Печать в обе стороны
	dlist.PrintForward()
	dlist.PrintBackward()

	// Добавление между элементами
	dlist.AddAfterValue(30, 35)
	dlist.AddBeforeValue(40, 38)
	dlist.PrintForward()

	// Удаление
	dlist.DeleteFromHead()
	dlist.DeleteFromTail()
	dlist.DeleteByValue(35)
	dlist.PrintForward()

	// Поиск
	fmt.Printf("Поиск 38: %v\n", dlist.FindByValue(38))

	// Сохранение/загрузка
	dlist.SaveToFile("doubly_list.txt")

	dlist2 := NewDoublyLinkedList()
	dlist2.LoadFromFile("doubly_list.txt")
	dlist2.PrintForward()
}

func demonstrateDynamicArray() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ ДИНАМИЧЕСКОГО МАССИВА ---")

	arr := NewDynamicArray(5)

	// Добавление элементов
	arr.PushBack("Hello")
	arr.PushBack("World")
	arr.PushBack("C++")
	arr.PushBack("Programming")
	arr.PushBack("Data Structures")

	// Печать
	arr.Print()

	// Вставка
	arr.Insert(2, "Awesome")
	arr.Print()

	// Изменение
	arr.Set(3, "Changed")
	arr.Print()

	// Удаление
	arr.Remove(1)
	arr.Print()

	// Получение элемента
	fmt.Printf("Элемент с индексом 2: %s\n", arr.Get(2))

	// Информация
	fmt.Printf("Размер: %d\n", arr.GetSize())
	fmt.Printf("Емкость: %d\n", arr.GetCapacity())

	// Сохранение/загрузка
	arr.SaveToFile("array.txt")

	arr2 := NewDynamicArray(5)
	arr2.LoadFromFile("array.txt")
	arr2.Print()
}

func demonstrateQueue() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ ОЧЕРЕДИ ---")

	queue := NewQueue()

	// Добавление элементов
	queue.Push(10)
	queue.Push(20)
	queue.Push(30)
	queue.Push(40)
	queue.Push(50)

	// Отображение
	queue.Display()

	// Удаление
	queue.Pop()
	queue.Pop()
	queue.Display()

	// Получение первого элемента
	fmt.Printf("Первый элемент: %d\n", queue.Front())

	// Информация
	fmt.Printf("Размер очереди: %d\n", queue.Size())
	fmt.Printf("Очередь пуста: %v\n", queue.IsEmpty())

	// Сохранение/загрузка
	queue.SaveToFile("queue.txt")

	queue2 := NewQueue()
	queue2.LoadFromFile("queue.txt")
	queue2.Display()
}

func demonstrateStack() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ СТЕКА ---")

	stack := NewStack()

	// Добавление элементов
	stack.Push(10)
	stack.Push(20)
	stack.Push(30)
	stack.Push(40)
	stack.Push(50)

	// Печать
	stack.Print()

	// Удаление
	stack.Pop()
	stack.Pop()
	stack.Print()

	// Получение верхнего элемента
	fmt.Printf("Верхний элемент: %d\n", stack.Peek())

	// Информация
	fmt.Printf("Размер стека: %d\n", stack.Size())
	fmt.Printf("Стек пуст: %v\n", stack.IsEmpty())

	// Сохранение/загрузка
	stack.SaveToFile("stack.txt")

	stack2 := NewStack()
	stack2.LoadFromFile("stack.txt")
	stack2.Print()
}

func demonstrateHashTable() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ ХЕШ-ТАБЛИЦ ---")

	// Создаем две реализации хеш-таблиц
	chainTable := NewChainHashTable(5)
	openTable := NewOpenHashTable(5)

	fmt.Println("\n1. Добавление элементов:")
	chainTable.Insert("name", "Ivan")
	openTable.Insert("name", "Ivan")

	chainTable.Insert("age", "25")
	openTable.Insert("age", "25")

	chainTable.Insert("city", "Moscow")
	openTable.Insert("city", "Moscow")

	chainTable.Insert("job", "Developer")
	openTable.Insert("job", "Developer")

	chainTable.Insert("country", "Russia")
	openTable.Insert("country", "Russia")

	// Показываем таблицы
	fmt.Println("\n2. Содержимое таблиц:")
	chainTable.Display()
	openTable.Display()

	// Поиск элементов
	fmt.Println("\n3. Поиск элементов:")
	fmt.Printf("Поиск 'name': %s\n", chainTable.Find("name"))
	fmt.Printf("Поиск 'age': %s\n", openTable.Find("age"))
	fmt.Printf("Поиск несуществующего 'salary': %s\n", chainTable.Find("salary"))

	// Демонстрация коллизий
	fmt.Println("\n4. Демонстрация коллизий:")
	chainTable.Insert("abc", "value1")
	chainTable.Insert("cba", "value2")

	openTable.Insert("abc", "value1")
	openTable.Insert("cba", "value2")

	chainTable.Display()
	openTable.Display()

	// Удаление
	fmt.Println("\n5. Удаление элементов:")
	chainTable.Remove("city")
	openTable.Remove("city")

	fmt.Println("\n6. После удаления 'city':")
	chainTable.Display()
	openTable.Display()

	// Изменение размера
	fmt.Println("\n7. Изменение размера таблиц:")
	chainTable.Resize(10)
	openTable.Resize(10)

	fmt.Println("После изменения размера:")
	chainTable.Display()
	openTable.Display()

	// Статистика
	fmt.Println("\n8. Статистика:")
	fmt.Println("Цепочечная таблица:")
	fmt.Printf("  Емкость: %d\n", chainTable.GetCapacity())
	fmt.Printf("  Элементов: %d\n", chainTable.GetSize())

	fmt.Println("Таблица с открытой адресацией:")
	fmt.Printf("  Емкость: %d\n", openTable.GetCapacity())
	fmt.Printf("  Элементов: %d\n", openTable.GetSize())
	fmt.Printf("  Коэффициент заполнения: %.1f%%\n", openTable.GetLoadFactor()*100)

	// Сохранение и загрузка
	fmt.Println("\n9. Сохранение и загрузка:")
	chainTable.SaveToFile("chain_table.txt")
	openTable.SaveToFile("open_table.txt")

	chainTable2 := NewChainHashTable(5)
	openTable2 := NewOpenHashTable(5)

	chainTable2.LoadFromFile("chain_table.txt")
	openTable2.LoadFromFile("open_table.txt")

	fmt.Println("Загруженные таблицы:")
	chainTable2.Display()
	openTable2.Display()
}

func demonstrateBinarySerialization() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ БИНАРНОЙ СЕРИАЛИЗАЦИИ ---")

	// Пример для двусвязного списка
	binaryList := NewDoublyLinkedList()
	binaryList.AddToHead(100)
	binaryList.AddToTail(200)
	binaryList.AddToTail(300)

	fmt.Println("\nИсходный список:")
	binaryList.PrintForward()

	// Сохраняем в бинарный файл
	binaryList.SaveToBinaryFile("binary_list.bin")

	// Загружаем из бинарного файла
	loadedList := NewDoublyLinkedList()
	loadedList.LoadFromBinaryFile("binary_list.bin")

	fmt.Println("\nЗагруженный список:")
	loadedList.PrintForward()

	// Пример для динамического массива
	fmt.Println("\n=== ДИНАМИЧЕСКИЙ МАССИВ ===")
	binaryArray := NewDynamicArray(5)
	binaryArray.PushBack("Hello")
	binaryArray.PushBack("World")
	binaryArray.PushBack("Binary")

	fmt.Println("\nИсходный массив:")
	binaryArray.Print()

	// Сохраняем в бинарный файл
	binaryArray.SaveToBinaryFile("binary_array.bin")

	// Загружаем из бинарного файла
	loadedArray := NewDynamicArray(5)
	loadedArray.LoadFromBinaryFile("binary_array.bin")

	fmt.Println("\nЗагруженный массив:")
	loadedArray.Print()

	// Пример для очереди
	fmt.Println("\n=== ОЧЕРЕДЬ ===")
	binaryQueue := NewQueue()
	binaryQueue.Push(555)
	binaryQueue.Push(666)
	binaryQueue.Push(777)

	fmt.Println("\nИсходная очередь:")
	binaryQueue.Display()

	// Сохраняем в бинарный файл
	binaryQueue.SaveToBinaryFile("binary_queue.bin")

	// Загружаем из бинарного файла
	loadedQueue := NewQueue()
	loadedQueue.LoadFromBinaryFile("binary_queue.bin")

	fmt.Println("\nЗагруженная очередь:")
	loadedQueue.Display()

	// Пример для стека
	fmt.Println("\n--- СТЕК ---")
	binaryStack := NewStack()
	binaryStack.Push(111)
	binaryStack.Push(222)
	binaryStack.Push(333)

	fmt.Println("\nИсходный стек:")
	binaryStack.Print()

	// Сохраняем в бинарный файл
	binaryStack.SaveToBinaryFile("binary_stack.bin")

	// Загружаем из бинарного файла
	loadedStack := NewStack()
	loadedStack.LoadFromBinaryFile("binary_stack.bin")

	fmt.Println("\nЗагруженный стек:")
	loadedStack.Print()

	fmt.Println("\nБинарная сериализация успешно продемонстрирована!")
	fmt.Println("Созданы файлы: binary_list.bin, binary_array.bin, binary_queue.bin, binary_stack.bin")
}

func demonstrateAllSerialization() {
	fmt.Println("\n--- ДЕМОНСТРАЦИЯ ВСЕХ ВИДОВ СЕРИАЛИЗАЦИИ ---")

	// Тест для односвязного списка
	fmt.Println("\n1. ОДНОСВЯЗНЫЙ СПИСОК:")
	list := NewSinglyLinkedList()
	list.AddToHead(50)
	list.AddToTail(60)
	list.AddToTail(70)

	fmt.Print("Исходный список: ")
	list.Print()

	// Текстовая сериализация
	list.SaveToFile("test_singly.txt")
	// Бинарная сериализация
	list.SaveToBinaryFile("test_singly.bin")

	listFromText := NewSinglyLinkedList()
	listFromBinary := NewSinglyLinkedList()
	listFromText.LoadFromFile("test_singly.txt")
	listFromBinary.LoadFromBinaryFile("test_singly.bin")

	fmt.Print("Из текстового файла: ")
	listFromText.Print()
	fmt.Print("Из бинарного файла: ")
	listFromBinary.Print()

	// Тест для хеш-таблиц
	fmt.Println("\n2. ХЕШ-ТАБЛИЦЫ:")
	hashTable := NewChainHashTable(3)
	hashTable.Insert("test", "value1")
	hashTable.Insert("key", "value2")

	fmt.Println("Исходная таблица:")
	hashTable.Display()

	// Текстовая сериализация
	hashTable.SaveToFile("test_hash.txt")
	// Бинарная сериализация
	hashTable.SaveToBinaryFile("test_hash.bin")

	hashFromText := NewChainHashTable(3)
	hashFromBinary := NewChainHashTable(3)
	hashFromText.LoadFromFile("test_hash.txt")
	hashFromBinary.LoadFromBinaryFile("test_hash.bin")

	fmt.Println("Из текстового файла:")
	hashFromText.Display()
	fmt.Println("Из бинарного файла:")
	hashFromBinary.Display()

	fmt.Println("\nВсе виды сериализации успешно протестированы!")
}

// Интерактивный режим для хеш-таблиц
func demonstrateHashTableInteractive() {
	fmt.Println("\n--- ИНТЕРАКТИВНАЯ РАБОТА С ХЕШ-ТАБЛИЦАМИ ---")

	chainTable := NewChainHashTable(10)
	openTable := NewOpenHashTable(10)

	reader := bufio.NewReader(os.Stdin)
	var choice int
	var key, value string

	for {
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
		fmt.Println("0. Вернуться в главное меню")
		fmt.Print("Выберите действие: ")

		fmt.Scanln(&choice)

		switch choice {
		case 1:
			fmt.Print("Введите ключ: ")
			key, _ = reader.ReadString('\n')
			key = strings.TrimSpace(key)

			fmt.Print("Введите значение: ")
			value, _ = reader.ReadString('\n')
			value = strings.TrimSpace(value)

			chainTable.Insert(key, value)
			openTable.Insert(key, value)

		case 2:
			fmt.Print("Введите ключ для поиска: ")
			key, _ = reader.ReadString('\n')
			key = strings.TrimSpace(key)

			chainResult := chainTable.Find(key)
			openResult := openTable.Find(key)

			if chainResult != "" {
				fmt.Printf("Цепочечная таблица: %s -> %s\n", key, chainResult)
			} else {
				fmt.Printf("Цепочечная таблица: ключ '%s' не найден\n", key)
			}

			if openResult != "" {
				fmt.Printf("Открытая адресация: %s -> %s\n", key, openResult)
			} else {
				fmt.Printf("Открытая адресация: ключ '%s' не найден\n", key)
			}

		case 3:
			fmt.Print("Введите ключ для удаления: ")
			key, _ = reader.ReadString('\n')
			key = strings.TrimSpace(key)

			chainTable.Remove(key)
			openTable.Remove(key)

		case 4:
			chainTable.Display()
			openTable.Display()

		case 5:
			chainTable.Clear()
			openTable.Clear()
			fmt.Println("Обе таблицы очищены")

		case 6:
			var newSize int
			fmt.Print("Введите новый размер таблиц: ")
			fmt.Scanln(&newSize)

			if newSize > 0 {
				chainTable.Resize(newSize)
				openTable.Resize(newSize)
			} else {
				fmt.Println("Размер должен быть положительным числом")
			}

		case 7:
			chainTable.SaveToFile("chain_hash_interactive.txt")
			openTable.SaveToFile("open_hash_interactive.txt")

		case 8:
			chainTable.LoadFromFile("chain_hash_interactive.txt")
			openTable.LoadFromFile("open_hash_interactive.txt")

		case 9:
			fmt.Println("\n--- СТАТИСТИКА ---")
			fmt.Println("Цепочечная таблица:")
			fmt.Printf("  Емкость таблицы: %d\n", chainTable.GetCapacity())
			fmt.Printf("  Количество элементов: %d\n", chainTable.GetSize())
			fmt.Printf("  Заполнение: %d/%d элементов\n",
				chainTable.GetSize(), chainTable.GetCapacity())

			fmt.Println("\nОткрытая адресация:")
			fmt.Printf("  Емкость таблицы: %d\n", openTable.GetCapacity())
			fmt.Printf("  Количество элементов: %d\n", openTable.GetSize())
			fmt.Printf("  Коэффициент заполнения: %.1f%%\n",
				openTable.GetLoadFactor()*100)

		case 0:
			fmt.Println("Возврат в главное меню")
			return

		default:
			fmt.Println("Неверный выбор. Попробуйте снова.")
		}
	}
}

// Main - главная функция
func main() {
	fmt.Println("ДЕМОНСТРАЦИЯ РЕАЛИЗАЦИИ СТРУКТУР ДАННЫХ НА GO")

	var choice int

	for {
		fmt.Println("\n--- ГЛАВНОЕ МЕНЮ ---")
		fmt.Println("1. Полное бинарное дерево")
		fmt.Println("2. Односвязный список")
		fmt.Println("3. Двусвязный список")
		fmt.Println("4. Динамический массив")
		fmt.Println("5. Очередь")
		fmt.Println("6. Стек")
		fmt.Println("7. Хеш-таблицы (демонстрация)")
		fmt.Println("8. Хеш-таблицы (интерактивный режим)")
		fmt.Println("9. Бинарная сериализация")
		fmt.Println("10. Все виды сериализации")
		fmt.Println("11. Все структуры данных")
		fmt.Println("0. Выход")
		fmt.Print("Выберите вариант: ")

		fmt.Scanln(&choice)

		switch choice {
		case 1:
			demonstrateFullBinaryTree()
		case 2:
			demonstrateSinglyLinkedList()
		case 3:
			demonstrateDoublyLinkedList()
		case 4:
			demonstrateDynamicArray()
		case 5:
			demonstrateQueue()
		case 6:
			demonstrateStack()
		case 7:
			demonstrateHashTable()
		case 8:
			demonstrateHashTableInteractive()
		case 9:
			demonstrateBinarySerialization()
		case 10:
			demonstrateAllSerialization()
		case 11:
			demonstrateFullBinaryTree()
			demonstrateSinglyLinkedList()
			demonstrateDoublyLinkedList()
			demonstrateDynamicArray()
			demonstrateQueue()
			demonstrateStack()
			demonstrateHashTable()
		case 0:
			fmt.Println("Выход из программы.")
			return
		default:
			fmt.Println("Неверный выбор!")
		}
	}
}
