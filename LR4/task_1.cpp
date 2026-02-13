#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <atomic>
#include <condition_variable>
#include <cstring>
#include <semaphore.h>

using namespace std;
using namespace chrono;

mutex cout_mutex;
sem_t semaphore;
atomic<bool> spinlock_flag{false};
atomic<int> barrier_counter{0};
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

// Реализация SpinWait
class SpinWait {
    int spinCount;
    
public:
    SpinWait() : spinCount(0) {}
    
    void spinOnce() {
        if (spinCount < 10) {
            // Пауза с помощью чтения процессора
            atomic_signal_fence(memory_order_seq_cst);
        } else if (spinCount < 20) {
            // Короткая пауза
            this_thread::yield();
        } else {
            // Более длительная пауза
            this_thread::sleep_for(microseconds(1));
        }
        spinCount++;
    }
    
    void reset() {
        spinCount = 0;
    }
    
    int getCount() const {
        return spinCount;
    }
    
    // Адаптивное ожидание
    bool nextSpinWillYield() const {
        return spinCount >= 10;
    }
};

// SpinLock с использованием SpinWait
class SpinLock {
    atomic_flag flag = ATOMIC_FLAG_INIT;
    
public:
    void lock() {
        SpinWait spinner;
        while (flag.test_and_set(memory_order_acquire)) {
            spinner.spinOnce();
        }
    }
    
    void unlock() {
        flag.clear(memory_order_release);
    }
    
    // Неблокирующая попытка захвата
    bool try_lock() {
        return !flag.test_and_set(memory_order_acquire);
    }
};

SpinLock spinlock;

// SpinLock с экспоненциальной задержкой
class BackoffSpinLock {
    atomic<bool> locked{false};
    
public:
    void lock() {
        SpinWait spinner;
        bool expected = false;
        
        while (!locked.compare_exchange_weak(expected, true, 
               memory_order_acquire, memory_order_relaxed)) {
            expected = false;
            spinner.spinOnce();
        }
    }
    
    void unlock() {
        locked.store(false, memory_order_release);
    }
};

BackoffSpinLock backoff_spinlock;

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

// Самодельный барьер с SpinWait
class SpinBarrier {
    atomic<int> count;
    int threshold;
    atomic<int> generation;
    
public:
    SpinBarrier(int threshold) : threshold(threshold), count(0), generation(0) {}
    
    void wait() {
        int gen = generation.load(memory_order_acquire);
        
        if (count.fetch_add(1, memory_order_acq_rel) == threshold - 1) {
            count.store(0, memory_order_release);
            generation.fetch_add(1, memory_order_acq_rel);
        } else {
            SpinWait spinner;
            while (generation.load(memory_order_acquire) == gen) {
                spinner.spinOnce();
            }
        }
    }
};

SpinBarrier spin_barrier(4);

// Самодельный барьер с мьютексом
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

// Функция потока с использованием базового SpinLock
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

// Функция потока с использованием SpinLock с Backoff
void thread_func_spinlock_backoff(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    SpinWait localSpinner; // Локальный SpinWait для демонстрации
    
    for (int i = 0; i < iterations; ++i) {
        backoff_spinlock.lock();
        
        char c = get_random_char();
        cout << "Thread " << id << " (SpinLock Backoff): " << c << " - " << i+1 << "/" << iterations << endl;
        
        // Демонстрация использования SpinWait в критической секции
        if (i % 2 == 0) {
            for (int j = 0; j < 5; j++) {
                localSpinner.spinOnce();
            }
            localSpinner.reset();
        }
        
        backoff_spinlock.unlock();
        this_thread::sleep_for(microseconds(50));
    }
    
    auto time = sw.stop();
    
    backoff_spinlock.lock();
    cout << "Thread " << id << " (SpinLock Backoff) завершен за " << time << " мс" << endl;
    cout << "  SpinWait использован " << localSpinner.getCount() << " раз" << endl;
    backoff_spinlock.unlock();
}

// Функция потока с использованием SpinWait барьера
void thread_func_spinbarrier(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    for (int i = 0; i < iterations; ++i) {
        char c = get_random_char();
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Thread " << id << " (SpinBarrier) до: " << c << endl;
        }
        
        spin_barrier.wait();
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Thread " << id << " (SpinBarrier) после синхронизации" << endl;
        }
        
        this_thread::sleep_for(microseconds(id * 100));
    }
    
    auto time = sw.stop();
    
    lock_guard<mutex> lock(cout_mutex);
    cout << "Thread " << id << " (SpinBarrier) завершен за " << time << " мс" << endl;
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

// Функция потока с демонстрацией SpinWait
void thread_func_spinwait_demo(int id, int iterations, StopWatch& sw) {
    sw.start();
    
    SpinWait spinner;
    int spinCount = 0;
    
    for (int i = 0; i < iterations; ++i) {
        // Имитация ожидания условия с использованием SpinWait
        int waitCycles = (id * 10) + (i % 5);
        
        for (int j = 0; j < waitCycles; j++) {
            spinner.spinOnce();
            spinCount++;
        }
        
        char c = get_random_char();
        
        {
            lock_guard<mutex> lock(cout_mutex);
            cout << "Thread " << id << " (SpinWait Demo): " << c 
                 << " - спин-циклов: " << waitCycles 
                 << " - " << i+1 << "/" << iterations << endl;
        }
        
        spinner.reset();
        this_thread::sleep_for(microseconds(30));
    }
    
    auto time = sw.stop();
    
    lock_guard<mutex> lock(cout_mutex);
    cout << "Thread " << id << " (SpinWait Demo) завершен за " << time << " мс" << endl;
    cout << "  Всего спин-циклов: " << spinCount << endl;
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
        } else if (sync_type == "spinlock_backoff") {
            threads.emplace_back(thread_func_spinlock_backoff, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "spinbarrier") {
            threads.emplace_back(thread_func_spinbarrier, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "barrier") {
            threads.emplace_back(thread_func_barrier, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "monitor") {
            threads.emplace_back(thread_func_monitor, i+1, iterations_per_thread, ref(watches[i]));
        } else if (sync_type == "spinwait") {
            threads.emplace_back(thread_func_spinwait_demo, i+1, iterations_per_thread, ref(watches[i]));
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

// Тест производительности SpinWait
void spinwait_performance_test() {
    cout << "\n=== ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ SPINWAIT ===" << endl;
    
    const int TEST_ITERATIONS = 1000000;
    StopWatch sw;
    
    // Тест 1: Пустой цикл
    sw.start();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
    }
    auto empty_loop_time = sw.stop();
    
    // Тест 2: SpinWait с yield
    sw.start();
    SpinWait spinner1;
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        spinner1.spinOnce();
        if (i % 1000 == 0) spinner1.reset();
    }
    auto spinwait_time = sw.stop();
    
    // Тест 3: Обычный yield
    sw.start();
    for (int i = 0; i < TEST_ITERATIONS; i++) {
        if (i % 100 == 0) {
            this_thread::yield();
        }
    }
    auto yield_time = sw.stop();
    
    cout << "Пустой цикл (" << TEST_ITERATIONS << " итераций): " << empty_loop_time << " мс" << endl;
    cout << "SpinWait (" << TEST_ITERATIONS << " итераций): " << spinwait_time << " мс" << endl;
    cout << "Yield каждые 100 итераций: " << yield_time << " мс" << endl;
    cout << "==============================" << endl;
}

int main() {
    sem_init(&semaphore, 0, 1);
    
    cout << "ПАРАЛЛЕЛЬНАЯ ГОНКА ПОТОКОВ СО СЛУЧАЙНЫМИ ASCII СИМВОЛАМИ\n" << endl;
    
    const int NUM_THREADS = 4;
    const int ITERATIONS = 5;
    
    spinwait_performance_test();
    benchmark(NUM_THREADS, ITERATIONS, "mutex");
    benchmark(NUM_THREADS, ITERATIONS, "semaphore");
    benchmark(NUM_THREADS, ITERATIONS, "spinlock");
    benchmark(NUM_THREADS, ITERATIONS, "spinlock_backoff");
    benchmark(NUM_THREADS, ITERATIONS, "spinbarrier");
    benchmark(NUM_THREADS, ITERATIONS, "barrier");
    benchmark(NUM_THREADS, ITERATIONS, "monitor");
    benchmark(NUM_THREADS, ITERATIONS, "spinwait");
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
    
    cout << "\n3. SpinLock (базовый):" << endl;
    cout << "   - Активное ожидание" << endl;
    cout << "   - Быстрее при коротких блокировках" << endl;
    cout << "   - Потребляет CPU" << endl;
    
    cout << "\n4. SpinLock с Backoff (SpinWait):" << endl;
    cout << "   - Адаптивное ожидание" << endl;
    cout << "   - Меньше contention" << endl;
    cout << "   - Экономит энергию CPU" << endl;
    
    cout << "\n5. SpinWait:" << endl;
    cout << "   - Адаптивная стратегия ожидания" << endl;
    cout << "   - Сначала короткие паузы, потом yield" << endl;
    cout << "   - Оптимально для гибридных сценариев" << endl;
    
    cout << "\n6. Барьер (Barrier):" << endl;
    cout << "   - Синхронизация группы потоков" << endl;
    cout << "   - Все ждут друг друга" << endl;
    cout << "   - Для параллельных алгоритмов" << endl;
    
    cout << "\n7. Монитор (Monitor):" << endl;
    cout << "   - Мьютекс + condition variable" << endl;
    cout << "   - Более высокоуровневый" << endl;
    cout << "   - Поддержка ожидания условий" << endl;
    
    cout << "\n8. SpinBarrier:" << endl;
    cout << "   - Барьер на основе SpinWait" << endl;
    cout << "   - Быстрее для коротких ожиданий" << endl;
    cout << "   - Меньше накладных расходов" << endl;
    
    return 0;
}