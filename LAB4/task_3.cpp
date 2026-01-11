#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class BankersAlgorithm {
private:
    int processes;  // Количество процессов
    int resources;  // Количество типов ресурсов
    
    // Матрицы и векторы
    vector<vector<int>> max;      // Максимальная потребность
    vector<vector<int>> allocation; // Выделенные ресурсы
    vector<vector<int>> need;     // Оставшаяся потребность
    vector<int> available;        // Доступные ресурсы
    vector<bool> finished;        // Завершенные процессы
    
public:
    // Конструктор
    BankersAlgorithm(int p, int r) : processes(p), resources(r) {
        max.resize(p, vector<int>(r, 0));
        allocation.resize(p, vector<int>(r, 0));
        need.resize(p, vector<int>(r, 0));
        available.resize(r, 0);
        finished.resize(p, false);
    }
    
    // Установка максимальной потребности
    void setMax(int process, const vector<int>& values) {
        if (process >= 0 && process < processes && values.size() == resources) {
            max[process] = values;
        }
    }
    
    // Установка выделенных ресурсов
    void setAllocation(int process, const vector<int>& values) {
        if (process >= 0 && process < processes && values.size() == resources) {
            allocation[process] = values;
            // Пересчет оставшейся потребности
            for (int i = 0; i < resources; i++) {
                need[process][i] = max[process][i] - allocation[process][i];
            }
        }
    }
    
    // Установка доступных ресурсов
    void setAvailable(const vector<int>& values) {
        if (values.size() == resources) {
            available = values;
        }
    }
    
    // Проверка безопасности системы
    bool isSafeState() {
        // Копируем доступные ресурсы для работы
        vector<int> work = available;
        vector<bool> finish = finished;
        vector<int> safeSequence;
        
        int count = 0;
        while (count < processes) {
            bool found = false;
            
            // Ищем процесс, который может быть выполнен
            for (int p = 0; p < processes; p++) {
                if (!finish[p]) {
                    // Проверяем, достаточно ли ресурсов для процесса
                    bool canExecute = true;
                    for (int r = 0; r < resources; r++) {
                        if (need[p][r] > work[r]) {
                            canExecute = false;
                            break;
                        }
                    }
                    
                    // Если процесс может быть выполнен
                    if (canExecute) {
                        // Освобождаем ресурсы процесса
                        for (int r = 0; r < resources; r++) {
                            work[r] += allocation[p][r];
                        }
                        
                        safeSequence.push_back(p);
                        finish[p] = true;
                        found = true;
                        count++;
                    }
                }
            }
            
            // Если не нашли ни одного процесса для выполнения
            if (!found) {
                cout << "\nСИСТЕМА НЕ В БЕЗОПАСНОМ СОСТОЯНИИ!" << endl;
                cout << "Обнаружена возможность взаимной блокировки (deadlock)" << endl;
                return false;
            }
        }
        
        // Система в безопасном состоянии
        cout << "\nСИСТЕМА В БЕЗОПАСНОМ СОСТОЯНИИ!" << endl;
        cout << "Безопасная последовательность выполнения процессов: ";
        for (size_t i = 0; i < safeSequence.size(); i++) {
            cout << "P" << safeSequence[i];
            if (i != safeSequence.size() - 1) cout << " → ";
        }
        cout << endl;
        return true;
    }
    
    // Запрос ресурсов процессом
    bool requestResources(int process, const vector<int>& request) {
        cout << "\nЗапрос ресурсов процессом P" << process << ": ";
        for (int val : request) cout << val << " ";
        cout << endl;
        
        // Проверка 1: Запрос не превышает оставшуюся потребность
        for (int i = 0; i < resources; i++) {
            if (request[i] > need[process][i]) {
                cout << "ОШИБКА: Запрос превышает максимальную потребность!" << endl;
                return false;
            }
        }
        
        // Проверка 2: Достаточно ли доступных ресурсов
        for (int i = 0; i < resources; i++) {
            if (request[i] > available[i]) {
                cout << "Процесс должен ждать. Недостаточно доступных ресурсов." << endl;
                return false;
            }
        }
        
        // Пробуем выделить ресурсы
        for (int i = 0; i < resources; i++) {
            available[i] -= request[i];
            allocation[process][i] += request[i];
            need[process][i] -= request[i];
        }
        
        // Проверяем, остается ли система в безопасном состоянии
        if (isSafeState()) {
            cout << "Запрос одобрен. Ресурсы выделены." << endl;
            return true;
        } else {
            // Откат изменений
            for (int i = 0; i < resources; i++) {
                available[i] += request[i];
                allocation[process][i] -= request[i];
                need[process][i] += request[i];
            }
            cout << "Запрос отклонен. Выделение ресурсов приведет к небезопасному состоянию." << endl;
            return false;
        }
    }
    
    // Освобождение ресурсов процессом
    void releaseResources(int process, const vector<int>& release) {
        cout << "\nОсвобождение ресурсов процессом P" << process << ": ";
        for (int val : release) cout << val << " ";
        cout << endl;
        
        for (int i = 0; i < resources; i++) {
            if (release[i] <= allocation[process][i]) {
                allocation[process][i] -= release[i];
                available[i] += release[i];
                // Увеличиваем потребность (освобожденные ресурсы могут быть запрошены снова)
                need[process][i] += release[i];
            }
        }
        
        cout << "Ресурсы освобождены." << endl;
    }
    
    // Вывод состояния системы
    void printState() {
        cout << "\n--- ТЕКУЩЕЕ СОСТОЯНИЕ СИСТЕМЫ ---" << endl;
        
        cout << "\nДоступные ресурсы: ";
        for (int val : available) cout << val << " ";
        cout << endl;
        
        cout << "\nПроцесс | Максимум   | Выделено   | Потребность" << endl;
        cout << "--------|------------|------------|------------" << endl;
        
        for (int p = 0; p < processes; p++) {
            cout << "P" << p << "      | ";
            for (int val : max[p]) cout << val << " ";
            cout << "     | ";
            for (int val : allocation[p]) cout << val << " ";
            cout << "     | ";
            for (int val : need[p]) cout << val << " ";
            cout << endl;
        }
    }
};

int main() {
    cout << "-----------------------------------------------------" << endl;
    cout << "АЛГОРИТМ БАНКИРА - ПРЕДОТВРАЩЕНИЕ ВЗАИМНЫХ БЛОКИРОВОК" << endl;
    cout << "-----------------------------------------------------" << endl;
    
    // Создаем систему: 5 процессов, 3 типа ресурсов
    BankersAlgorithm system(5, 3);
    
    // Устанавливаем максимальную потребность
    system.setMax(0, {7, 5, 3});
    system.setMax(1, {3, 2, 2});
    system.setMax(2, {9, 0, 2});
    system.setMax(3, {2, 2, 2});
    system.setMax(4, {4, 3, 3});
    
    // Устанавливаем выделенные ресурсы
    system.setAllocation(0, {0, 1, 0});
    system.setAllocation(1, {2, 0, 0});
    system.setAllocation(2, {3, 0, 2});
    system.setAllocation(3, {2, 1, 1});
    system.setAllocation(4, {0, 0, 2});
    
    // Устанавливаем доступные ресурсы
    system.setAvailable({3, 3, 2});
    
    // Выводим начальное состояние
    system.printState();
    
    // Проверяем безопасность системы
    cout << "\n--- ПРОВЕРКА БЕЗОПАСНОСТИ СИСТЕМЫ ---" << endl;
    system.isSafeState();
    
    // Пример 1: Запрос ресурсов процессом P1
    cout << "\n--- ПРИМЕР 1: ЗАПРОС РЕСУРСОВ ---" << endl;
    system.requestResources(1, {1, 0, 2});
    
    // Пример 2: Еще один запрос (может быть отклонен)
    cout << "\n--- ПРИМЕР 2: ЕЩЕ ОДИН ЗАПРОС ---" << endl;
    system.requestResources(4, {3, 3, 0});
    
    // Пример 3: Освобождение ресурсов
    cout << "\n--- ПРИМЕР 3: ОСВОБОЖДЕНИЕ РЕСУРСОВ ---" << endl;
    system.releaseResources(2, {1, 0, 1});
    
    // Финальное состояние
    system.printState();
    
    // Проверяем безопасность после изменений
    cout << "\n--- ФИНАЛЬНАЯ ПРОВЕРКА БЕЗОПАСНОСТИ ---" << endl;
    system.isSafeState();
    
    return 0;
}