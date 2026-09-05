// Пример 10: продвинутые темы.
//
//   std::exception_ptr      — перенос исключения между потоками;
//   std::nested_exception   — цепочка причин (throw_with_nested);
//   исключения в конструкторах (норма) и деструкторах (опасно).
#include <exception>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

// --- 1. exception_ptr: вытащить исключение из другого потока ---
void worker(std::exception_ptr& slot) {
    try {
        throw std::runtime_error("Ошибка в рабочем потоке");
    } catch (...) {
        slot = std::current_exception();  // сохраняем исключение
    }
}

void demo_exception_ptr() {
    std::exception_ptr captured = nullptr;
    std::thread t(worker, std::ref(captured));
    t.join();

    if (captured) {
        try {
            std::rethrow_exception(captured);  // повторно бросаем в этом потоке
        } catch (const std::exception& e) {
            std::cout << "Из потока: " << e.what() << '\n';
        }
    }
}

// --- 2. nested_exception: цепочка «ошибка БД из-за ошибки диска» ---
void low_level() {
    throw std::runtime_error("Ошибка диска");
}

void mid_level() {
    try {
        low_level();
    } catch (...) {
        // Оборачиваем текущее исключение в новое, сохраняя причину.
        std::throw_with_nested(std::runtime_error("Ошибка базы данных"));
    }
}

void print_nested(const std::exception& e, int level = 0) {
    std::cout << std::string(static_cast<std::size_t>(level) * 2, ' ')
              << "- " << e.what() << '\n';
    try {
        std::rethrow_if_nested(e);  // если есть вложенное — бросит его
    } catch (const std::exception& nested) {
        print_nested(nested, level + 1);  // рекурсивно печатаем причину
    }
}

void demo_nested() {
    try {
        mid_level();
    } catch (const std::exception& e) {
        print_nested(e);
    }
}

// --- 3. Исключение в конструкторе — нормально (даже рекомендуется) ---
class Connection {
public:
    explicit Connection(const std::string& host) {
        if (host.empty())
            throw std::invalid_argument("Пустой хост");
        // Если бы мы захватывали ресурсы вручную, их нужно было бы освободить
        // перед throw. RAII-члены (умные указатели и т.п.) освобождаются сами.
        std::cout << "Соединение установлено: " << host << '\n';
    }
};

void demo_ctor() {
    try {
        Connection bad("");  // конструктор бросит — объект не создан
    } catch (const std::exception& e) {
        std::cout << "Конструктор бросил: " << e.what() << '\n';
    }
    Connection ok("db.example.com");  // успешное построение
}

int main() {
    std::cout << "[exception_ptr]\n";
    demo_exception_ptr();

    std::cout << "\n[nested_exception]\n";
    demo_nested();

    std::cout << "\n[исключение в конструкторе]\n";
    demo_ctor();

    // Про деструкторы: бросать из них НЕЛЬЗЯ. Деструкторы неявно noexcept,
    // и исключение из деструктора (особенно во время раскрутки стека)
    // приводит к std::terminate(). Поэтому здесь мы это только описываем,
    // а не воспроизводим.

    return 0;
}
