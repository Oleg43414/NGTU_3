# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude/include

# Директории
SRC_DIR = src/src
INC_DIR = include/include
BUILD_DIR = build

# Исходные файлы
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Massiv.cpp \
          $(SRC_DIR)/List_1.cpp \
          $(SRC_DIR)/List_2.cpp \
          $(SRC_DIR)/Steck.cpp \
          $(SRC_DIR)/Queue.cpp \
          $(SRC_DIR)/FullBinaryTree.cpp

# Объектные файлы
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Целевой исполняемый файл
TARGET = program.exe

# Правило по умолчанию
all: $(TARGET)

# Создание целевого файла
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Компиляция завершена! Запустите: ./$(TARGET)"

# Компиляция объектных файлов
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Создание директории для объектных файлов
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# Очистка скомпилированных файлов
clean:
	rm -rf $(BUILD_DIR) $(TARGET) *.txt
	@echo "Очистка завершена!"

# Очистка только объектных файлов (оставляет exe и txt)
clean-obj:
	rm -rf $(BUILD_DIR)

# Запуск программы
run: $(TARGET)
	./$(TARGET)

# Справка
help:
	@echo "Доступные команды:"
	@echo "  make          - Скомпилировать проект"
	@echo "  make run      - Скомпилировать и запустить"
	@echo "  make clean    - Удалить все скомпилированные файлы и данные"
	@echo "  make clean-obj - Удалить только объектные файлы"
	@echo "  make help     - Показать эту справку"

.PHONY: all clean clean-obj run help
