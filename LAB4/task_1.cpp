#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <semaphore.h> // для POSIX семафоров

using namespace std;
using namespace chrono;

// Глобальные переменные для синхронизации
mutex cout_mutex;
sem_t semaphore; // POSIX семафор
atomic<bool> spinlock_flag{false}; // SpinLock флаг
atomic<int> barrier_counter{0}; // Самодельный барьер
mutex barrier_mutex;
condition_variable barrier_cv;
const int BARRIER_THRESHOLD = 4;

condition_variable cv;
mutex cv_mutex;
bool ready = false;

// Stopwatch для измерения времени
class StopWatch {
    time_point<high_resolution_clock> start_time;
public:
    void start() {
        start_time = high_resolution_clock::now();
    }
    
    double stop() {
        auto end_time = high_resolution_clock::now();
        return duration<double, milli>(end_time - start_time).count();
    }
};

// SpinLock реализация
class SpinLock {
    atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (flag.test_and_set(memory_order_acquire)) {
            // SpinWait
            this_thread::yield();
        }
    }
    
    void unlock() {
        flag.clear(memory_order_release);
    }
};

SpinLock spinlock;

// Генератор случайных ASCII символов
char get_random_char() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int> dist(33, 126);
    
    return static_cast<char>(dist(gen));
}

// Монитор
class Monitor {
    mutex mtx;
    condition_variable cond;
    bool available = true;
    
public:
    void enter() {
        unique_lock<mutex> lock(mtx);
        cond.wait(lock, [this] { return available; });
        available = false;
    }
    
    void exit() {
        {
            lock_guard<mutex> lock(mtx);
            available = true;
        }
        cond.notify_one();
    }
};

Monitor monitor;

// Самодельный барьер
class SimpleBarrier {
    mutex mtx;
    condition_variable cv;
    int count;
    int threshold;
    int generation;
    
public:
    SimpleBarrier(int threshold) : threshold(threshold), count(0), generation(0) {}
    
    void wait() {
        unique_lock<mutex> lock(mtx);
        int gen = generation;
        
        if (++count == threshold) {
            count = 0;
            generation++;
            cv.notify_all();
        } else {
            cv.wait(lock, [this, gen] { return gen != generation; });
        }
    }
};

SimpleBarrier my_barrier(4);

// Функция потока с использованием мьютекса
void thread_func_mutex(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    for (int i = 0; i < iterations; ++i) {
        lock_guard<mutex> lock(cout_mutex);
        char c = get_random_char();
        cout << "Thread " << id << " (Mutex): " << c << " - " << i+1 << "/" << iterations << endl;
        this_thread::sleep_for(microseconds(50));
    }
    
    auto time = sw.stop();
    
    lock_guard<mutex> lock(cout_mutex);
    cout << "Thread " << id << " (Mutex) завершен за " << time << " мс" << endl;
}

// Функция потока с использованием семафора
void thread_func_semaphore(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    for (int i = 0; i < iterations; ++i) {
        sem_wait(&semaphore);
        
        char c = get_random_char();
        cout << "Thread " << id << " (Semaphore): " << c << " - " << i+1 << "/" << iterations << endl;
        
        sem_post(&semaphore);
        this_thread::sleep_for(microseconds(50));
    }
    
    auto time = sw.stop();
    
    lock_guard<mutex> lock(cout_mutex);
    cout << "Thread " << id << " (Semaphore) завершен за " << time << " мс" << endl;
}

// Функция потока с использованием SpinLock
void thread_func_spinlock(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    for (int i = 0; i < iterations; ++i) {
        spinlock.lock();
        
        char c = get_random_char();
        cout << "Thread " << id << " (SpinLock): " << c << " - " << i+1 << "/" << iterations << endl;
        
        spinlock.unlock();
        this_thread::sleep_for(microseconds(50));
    }
    
    auto time = sw.stop();
    
    spinlock.lock();
    cout << "Thread " << id << " (SpinLock) завершен за " << time << " мс" << endl;
    spinlock.unlock();
}

// Функция потока с использованием барьера
void thread_func_barrier(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    for (int i = 0; i < iterations; ++i) {
        char c = get_random_char();
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Thread " << id << " (Barrier) до: " << c << endl;
        }
        
        my_barrier.wait();
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Thread " << id << " (Barrier) после синхронизации" << endl;
        }
        
        this_thread::sleep_for(microseconds(id * 100));
    }
    
    auto time = sw.stop();
    
    lock_guard<mutex> lock(cout_mutex);
    cout << "Thread " << id << " (Barrier) завершен за " << time << " мс" << endl;
}

// Функция потока с использованием монитора
void thread_func_monitor(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    for (int i = 0; i < iterations; ++i) {
        monitor.enter();
        
        char c = get_random_char();
        cout << "Thread " << id << " (Monitor): " << c << " - " << i+1 << "/" << iterations << endl;
        
        monitor.exit();
        this_thread::sleep_for(microseconds(50));
    }
    
    auto time = sw.stop();
    
    monitor.enter();
    cout << "Thread " << id << " (Monitor) завершен за " << time << " мс" << endl;
    monitor.exit();
}

// Бенчмарк функция
void benchmark(int num_threads, int iterations_per_thread, const string& sync_type) {
    cout << "\n=== Бенчмарк: " << sync_type << " ===" << endl;
    cout << "Потоков: " << num_threads << ", Итераций на поток: " << iterations_per_thread << endl;
    
    vector<thread> threads;
    vector<StopWatch> watches(num_threads);
    
    StopWatch total_time;
    total_time.start();
    
    for (int i = 0; i < num_threads; ++i) {
        if (sync_type == "mutex") {
            threads.emplace_back(thread_func_mutex, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "semaphore") {
            threads.emplace_back(thread_func_semaphore, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "spinlock") {
            threads.emplace_back(thread_func_spinlock, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "barrier") {
            threads.emplace_back(thread_func_barrier, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "monitor") {
            threads.emplace_back(thread_func_monitor, i+1, iterations_per_thread, ref(watches[i]));
        }
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto total = total_time.stop();
    
    cout << "\nИтоговая статистика (" << sync_type << "):" << endl;
    cout << "Общее время выполнения: " << total << " мс" << endl;
    cout << "Среднее время на поток: " << total / num_threads << " мс" << endl;
    cout << "==============================" << endl;
}

int main() {
    // Инициализация POSIX семафора
    sem_init(&semaphore, 0, 1);
    
    cout << "Параллельная гонка потоков со случайными ASCII символами\n" << endl;
    
    const int NUM_THREADS = 4;
    const int ITERATIONS = 5;
    
    // Запуск бенчмарков
    benchmark(NUM_THREADS, ITERATIONS, "mutex");
    benchmark(NUM_THREADS, ITERATIONS, "semaphore");
    benchmark(NUM_THREADS, ITERATIONS, "spinlock");
    benchmark(NUM_THREADS, ITERATIONS, "barrier");
    benchmark(NUM_THREADS, ITERATIONS, "monitor");
    
    // Уничтожение семафора
    sem_destroy(&semaphore);
    
    cout << "\n=== СРАВНИТЕЛЬНЫЙ АНАЛИЗ ===" << endl;
    cout << "\n1. Мьютекс (Mutex):" << endl;
    cout << "   - Блокировка потока при занятости" << endl;
    cout << "   - Переключение контекста дорогое" << endl;
    cout << "   - Хорош для длительных операций" << endl;
    
    cout << "\n2. Семафор (Semaphore):" << endl;
    cout << "   - Позволяет нескольким потокам" << endl;
    cout << "   - Счетчик ресурсов" << endl;
    cout << "   - Гибче мьютекса" << endl;
    
    cout << "\n3. SpinLock:" << endl;
    cout << "   - Активное ожидание" << endl;
    cout << "   - Быстрее при коротких блокировках" << endl;
    cout << "   - Потребляет CPU" << endl;
    
    cout << "\n4. Барьер (Barrier):" << endl;
    cout << "   - Синхронизация группы потоков" << endl;
    cout << "   - Все ждут друг друга" << endl;
    cout << "   - Для параллельных алгоритмов" << endl;
    
    cout << "\n5. Монитор (Monitor):" << endl;
    cout << "   - Мьютекс + condition variable" << endl;
    cout << "   - Более высокоуровневый" << endl;
    cout << "   - Поддержка ожидания условий" << endl;
    
    return 0;
}