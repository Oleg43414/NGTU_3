package main

import (
	"fmt"
    "os"
    "strconv"
    "strings"
)

type Stack struct {
    key  []int
    top  int
}

const MAX_SIZE = 1000

func NewStack() *Stack {
    return &Stack{
        key: make([]int, MAX_SIZE),
        top: -1,
    }
}

func (s *Stack) push(value int) {
    if s.top >= MAX_SIZE-1 {
        fmt.Println("Стек переполнен!")
        return
    }
    s.top++
    s.key[s.top] = value
    fmt.Printf("Добавлен элемент: %d\n", value)
}

func (s *Stack) pop() int {
    if s.isEmpty() {
        fmt.Println("Стек пуст!")
        return -1
    }
    value := s.key[s.top]
    s.top--
    fmt.Printf("Удален элемент: %d\n", value)
    return value
}

func (s *Stack) peek() int {
    if s.isEmpty() {
        fmt.Println("Стек пуст!")
        return -1
    }
    return s.key[s.top]
}

func (s *Stack) isEmpty() bool {
    return s.top == -1
}

func (s *Stack) size() int {
    return s.top + 1
}

func (s *Stack) print() {
    if s.isEmpty() {
        fmt.Println("Стек пуст")
        return
    }
    fmt.Print("Стек (сверху вниз): ")
    for i := s.top; i >= 0; i-- {
        fmt.Printf("%d ", s.key[i])
    }
    fmt.Println()
}

func (s *Stack) clear() {
    s.top = -1
    fmt.Println("Стек очищен!")
}

func (s *Stack) saveToFile(filename string) {
    file, err := os.Create(filename)
    if err != nil {
        fmt.Printf("Ошибка создания файла: %s\n", filename)
        return
    }
    defer file.Close()

    fmt.Fprintf(file, "%d\n", s.size())
    for i := 0; i <= s.top; i++ {
        fmt.Fprintf(file, "%d ", s.key[i])
    }
    
    fmt.Printf("Стек сохранён в файл: %s\n", filename)
}

func (s *Stack) loadFromFile(filename string) {
    data, err := os.ReadFile(filename)
    if err != nil {
        fmt.Printf("Ошибка открытия файла: %s\n", filename)
        return
    }
    
    s.clear()
    lines := strings.Split(string(data), "\n")
    
    if len(lines) > 0 {
        n, err := strconv.Atoi(strings.TrimSpace(lines[0]))
        if err != nil {
            fmt.Printf("Ошибка чтения размера: %v\n", err)
            return
        }
        
        values := strings.Fields(lines[1])
        for i := 0; i < n && i < len(values); i++ {
            if value, err := strconv.Atoi(values[i]); err == nil && s.top < MAX_SIZE-1 {
                s.top++
                s.key[s.top] = value
            }
        }
    }
    
    fmt.Printf("Стек загружен из файла: %s\n", filename)
}

func main() {
    stack := NewStack()
    
    stack.push(10)
    stack.push(20)
    stack.push(30)
    stack.print()
    
    fmt.Printf("Верхний элемент: %d\n", stack.peek())
    fmt.Printf("Размер стека: %d\n", stack.size())

    stack.pop()
    stack.print()
    stack.clear()
    stack.push(1)
    stack.push(2)
    stack.push(3)
    stack.print()
    
    stack.saveToFile("stack.txt")
    
    stack2 := NewStack()
    stack2.loadFromFile("stack.txt")
    stack2.print()
}
