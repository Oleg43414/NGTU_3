cmake_minimum_required(VERSION 3.10)
project(CppDataStructures)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ЯВНО включаем покрытие для Debug
set(COVERAGE_FLAGS "--coverage -fprofile-arcs -ftest-coverage")

# Устанавливаем флаги для разных типов сборки
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 ${COVERAGE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0 ${COVERAGE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${COVERAGE_FLAGS}")

# Основные файлы
set(MAIN_SOURCE_FILES
    main.cpp
    Hash.cpp
    FullBinaryTree.cpp
    SinglyLinkedList.cpp
    DoublyLinkedList.cpp
    DynamicArray.cpp
    Queue.cpp
    Stack.cpp
)

set(HEADER_FILES
    Hash.h
    FullBinaryTree.h
    SinglyLinkedList.h
    DoublyLinkedList.h
    DynamicArray.h
    Queue.h
    Stack.h
)

# Основная программа
add_executable(main ${MAIN_SOURCE_FILES} ${HEADER_FILES})

# Тестовые файлы
set(TEST_SOURCE_FILES
    test_coverage.cpp
    test_doubly_linked_list.cpp
    test_dynamic_array.cpp
    test_full_binary_tree.cpp
    test_hash.cpp
    test_queue.cpp
    test_singly_linked_list.cpp
    test_stack.cpp
)

set(TEST_LIB_SOURCE_FILES
    Hash.cpp
    FullBinaryTree.cpp
    SinglyLinkedList.cpp
    DoublyLinkedList.cpp
    DynamicArray.cpp
    Queue.cpp
    Stack.cpp
)

# Тестовый исполняемый файл
add_executable(run_tests ${TEST_SOURCE_FILES} ${TEST_LIB_SOURCE_FILES} ${HEADER_FILES})

# Найти GoogleTest
find_package(GTest REQUIRED)
target_link_libraries(run_tests GTest::gtest GTest::gtest_main)

# Директории включения
target_include_directories(run_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Цель для создания отчета покрытия
add_custom_target(coverage
    COMMAND ${CMAKE_COMMAND} -E echo "Запуск тестов..."
    COMMAND ./run_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Сбор данных покрытия..."
    COMMAND gcovr --exclude-unreachable-branches --exclude-throw-branches
                  --root ${CMAKE_SOURCE_DIR}
                  --html --html-details -o ${CMAKE_BINARY_DIR}/coverage_full.html
    COMMAND ${CMAKE_COMMAND} -E echo "Упрощенный отчет..."
    COMMAND gcovr --root ${CMAKE_SOURCE_DIR} --html --html-details -o ${CMAKE_BINARY_DIR}/coverage.html
    COMMAND ${CMAKE_COMMAND} -E echo "Отчет создан: ${CMAKE_BINARY_DIR}/coverage.html"
    DEPENDS run_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)