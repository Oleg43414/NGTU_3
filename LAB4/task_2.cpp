#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <regex>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <random>

using namespace std;
using namespace chrono;

// Структура для хранения данных пользователя
struct User {
    string full_name;
    string phone;
    string registration_date; // формат: YYYY-MM-DD
    
    User(string name, string ph, string date) 
        : full_name(name), phone(ph), registration_date(date) {}
};

// Глобальные переменные для синхронизации
mutex result_mutex;
mutex cout_mutex;

// Шаблон номера телефона (регулярное выражение)
// Пример: +7-XXX-XXX-XX-XX или 8-XXX-XXX-XX-XX
const regex PHONE_PATTERN(R"(^(\+7|8)-\d{3}-\d{3}-\d{2}-\d{2}$)");

// Проверяет, соответствует ли номер телефона шаблону
bool is_phone_valid(const string& phone) {
    return regex_match(phone, PHONE_PATTERN);
}

// Проверяет, прошло ли менее месяца с даты регистрации
bool is_within_one_month(const string& reg_date) {
    // Парсим дату регистрации
    tm reg_tm = {};
    istringstream ss(reg_date);
    ss >> get_time(&reg_tm, "%Y-%m-%d");
    
    // Получаем текущее время
    time_t now = time(nullptr);
    tm* now_tm = localtime(&now);
    
    // Вычисляем разницу в днях
    time_t reg_time = mktime(&reg_tm);
    double diff_seconds = difftime(now, reg_time);
    int diff_days = static_cast<int>(diff_seconds / (60 * 60 * 24));
    
    return diff_days < 30; // Менее 30 дней
}

// Обработка пользователей (однопоточная версия)
vector<User> process_users_single_thread(const vector<User>& users) {
    vector<User> result;
    
    for (const auto& user : users) {
        if (is_phone_valid(user.phone) && is_within_one_month(user.registration_date)) {
            result.push_back(user);
        }
    }
    
    return result;
}

// Обработка пользователей (многопоточная версия)
vector<User> process_users_multi_thread(const vector<User>& users, int num_threads) {
    vector<User> result;
    vector<thread> threads;
    
    int chunk_size = users.size() / num_threads;
    
    // Функция для обработки части массива
    auto process_chunk = [&](int start, int end) {
        vector<User> local_result;
        
        for (int i = start; i < end && i < (int)users.size(); ++i) {
            if (is_phone_valid(users[i].phone) && is_within_one_month(users[i].registration_date)) {
                local_result.push_back(users[i]);
            }
        }
        
        // Синхронизация доступа к общему результату
        lock_guard<mutex> lock(result_mutex);
        result.insert(result.end(), local_result.begin(), local_result.end());
    };
    
    // Запускаем потоки
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? users.size() : (i + 1) * chunk_size;
        threads.emplace_back(process_chunk, start, end);
    }
    
    // Ждем завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }
    
    return result;
}

// Генерация тестовых данных
vector<User> generate_test_data(int size) {
    vector<User> users;
    vector<string> names = {
        "Иванов Иван Иванович",
        "Петров Петр Петрович",
        "Сидорова Анна Владимировна",
        "Кузнецов Алексей Сергеевич",
        "Смирнова Мария Дмитриевна",
        "Васильев Дмитрий Александрович",
        "Павлова Екатерина Игоревна",
        "Семенов Сергей Викторович",
        "Федорова Ольга Павловна",
        "Николаев Николай Николаевич"
    };
    
    vector<string> phones = {
        "+7-916-123-45-67",  // валидный
        "8-495-987-65-43",   // валидный
        "+7-999-888-77-66",  // валидный
        "8-800-555-35-35",   // валидный
        "123-45-67",         // невалидный
        "89161234567",       // невалидный
        "+7-916-123-45-6",   // невалидный (короткий)
        "8-916-123-45-678",  // невалидный (длинный)
        "+7-ABC-123-45-67",  // невалидный (буквы)
        "8-916-12-34-567"    // невалидный
    };
    
    // Текущая дата и даты для тестирования
    time_t now = time(nullptr);
    tm* now_tm = localtime(&now);
    
    // Генератор случайных чисел
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> name_dist(0, names.size() - 1);
    uniform_int_distribution<int> phone_dist(0, phones.size() - 1);
    uniform_int_distribution<int> day_dist(0, 60); // от 0 до 60 дней назад
    
    for (int i = 0; i < size; ++i) {
        // Генерируем случайную дату (от 0 до 60 дней назад)
        int days_ago = day_dist(gen);
        time_t reg_time = now - (days_ago * 24 * 60 * 60);
        tm reg_tm = *localtime(&reg_time);
        
        // Форматируем дату в строку
        ostringstream date_ss;
        date_ss << put_time(&reg_tm, "%Y-%m-%d");
        
        users.emplace_back(
            names[name_dist(gen)] + " #" + to_string(i),
            phones[phone_dist(gen)],
            date_ss.str()
        );
    }
    
    return users;
}

// Вывод результатов
void print_results(const vector<User>& users) {
    lock_guard<mutex> lock(cout_mutex);
    
    cout << "\nНайденные пользователи (" << users.size() << "):\n";
    cout << "========================================\n";
    
    for (const auto& user : users) {
        cout << "ФИО: " << user.full_name << endl;
        cout << "Телефон: " << user.phone << endl;
        cout << "Дата регистрации: " << user.registration_date << endl;
        cout << "----------------------------------------\n";
    }
}

int main() {
    // Ввод параметров
    int data_size, num_threads;
    
    cout << "=== Обработка пользователей интернет-магазина ===\n";
    cout << "Шаблон телефона: +7-XXX-XXX-XX-XX или 8-XXX-XXX-XX-XX\n";
    cout << "Условие: телефон соответствует шаблону И с даты регистрации < 30 дней\n\n";
    
    cout << "Введите размер массива данных: ";
    cin >> data_size;
    
    cout << "Введите количество потоков: ";
    cin >> num_threads;
    
    // Генерация тестовых данных
    cout << "\nГенерация тестовых данных...\n";
    vector<User> users = generate_test_data(data_size);
    
    cout << "Сгенерировано " << data_size << " пользователей.\n";
    
    // Однопоточная обработка
    cout << "\n--- Однопоточная обработка ---\n";
    auto start_time = high_resolution_clock::now();
    vector<User> result_single = process_users_single_thread(users);
    auto end_time = high_resolution_clock::now();
    
    double time_single = duration<double, milli>(end_time - start_time).count();
    cout << "Время выполнения: " << time_single << " мс\n";
    cout << "Найдено пользователей: " << result_single.size() << endl;
    
    // Многопоточная обработка
    cout << "\n--- Многопоточная обработка (" << num_threads << " потоков) ---\n";
    start_time = high_resolution_clock::now();
    vector<User> result_multi = process_users_multi_thread(users, num_threads);
    end_time = high_resolution_clock::now();
    
    double time_multi = duration<double, milli>(end_time - start_time).count();
    cout << "Время выполнения: " << time_multi << " мс\n";
    cout << "Найдено пользователей: " << result_multi.size() << endl;
    
    // Сравнение результатов
    cout << "\n=== СРАВНЕНИЕ РЕЗУЛЬТАТОВ ===\n";
    cout << "Однопоточная обработка: " << time_single << " мс\n";
    cout << "Многопоточная обработка: " << time_multi << " мс\n";
    cout << "Ускорение: " << time_single / time_multi << "x\n";
    
    // Проверка корректности (результаты должны быть одинаковыми)
    if (result_single.size() != result_multi.size()) {
        cout << "\nОШИБКА: Результаты не совпадают!\n";
    } else {
        // Сортируем для сравнения
        sort(result_single.begin(), result_single.end(), 
             [](const User& a, const User& b) { return a.full_name < b.full_name; });
        sort(result_multi.begin(), result_multi.end(),
             [](const User& a, const User& b) { return a.full_name < b.full_name; });
        
        bool match = true;
        for (size_t i = 0; i < result_single.size(); ++i) {
            if (result_single[i].full_name != result_multi[i].full_name ||
                result_single[i].phone != result_multi[i].phone) {
                match = false;
                break;
            }
        }
        
        if (match) {
            cout << "\nРезультаты совпадают!\n";
        } else {
            cout << "\nОШИБКА: Результаты не совпадают!\n";
        }
    }
    
    // Вывод результатов (если их не слишком много)
    if (result_multi.size() <= 20) {
        print_results(result_multi);
    } else {
        cout << "\nРезультатов слишком много для вывода (" << result_multi.size() << ")\n";
        cout << "Вывести первых 10:\n";
        
        vector<User> first_10(result_multi.begin(), 
                             result_multi.begin() + min(10, (int)result_multi.size()));
        print_results(first_10);
    }
    
    // Анализ эффективности
    cout << "\n=== АНАЛИЗ ЭФФЕКТИВНОСТИ ===\n";
    cout << "Размер данных: " << data_size << " записей\n";
    cout << "Количество потоков: " << num_threads << "\n";
    cout << "Эффективность многопоточности: " << (time_single - time_multi) / time_single * 100 << "%\n";
    
    if (time_multi > time_single) {
        cout << "Многопоточность оказалась МЕДЛЕННЕЕ однопоточной обработки.\n";
        cout << "Возможные причины:\n";
        cout << "1. Накладные расходы на создание потоков\n";
        cout << "2. Синхронизация доступа к общим данным\n";
        cout << "3. Маленький объем данных\n";
        cout << "4. Проблемы с балансировкой нагрузки\n";
    } else {
        cout << "Многопоточность эффективнее на " 
             << (time_single - time_multi) / time_single * 100 << "%\n";
    }
    
    return 0;
}