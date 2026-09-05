// Пример 01: мотивация — почему нужны умные указатели.
// Ручное new/delete при наличии исключений неизбежно ведёт к утечкам.
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

// Функция, которая может бросить исключение в середине работы.
void process(int* a, const std::string* s) {
    std::cout << "process: *a = " << *a << ", *s = " << *s << '\n';
    throw std::runtime_error("сбой в process");
}

// ПЛОХО: ручное управление памятью.
// При исключении в process() delete не выполнится — утечка.
void dangerous() {
    int* a = new int(10);
    std::string* s = new std::string("hello");
    try {
        process(a, s);
        // Эти строки НЕ выполнятся при исключении:
        delete s;
        delete a;
    } catch (const std::exception& e) {
        std::cout << "dangerous поймал: " << e.what()
                  << " (память a и s утекла!)\n";
        // Чтобы хоть как-то починить, пришлось бы дублировать delete здесь —
        // и так в каждом catch. Это хрупко и не масштабируется.
    }
}

// ХОРОШО: RAII через умные указатели.
// Деструкторы unique_ptr освободят память при любом выходе из функции,
// в том числе при исключении.
void safe() {
    auto a = std::make_unique<int>(10);
    auto s = std::make_unique<std::string>("hello");
    try {
        process(a.get(), s.get());  // get() — не передаёт владения
    } catch (const std::exception& e) {
        std::cout << "safe поймал: " << e.what()
                  << " (память освобождена автоматически)\n";
    }
}  // здесь вызовутся ~unique_ptr — память освобождена в любом случае

int main() {
    std::cout << "--- dangerous (ручной new/delete) ---\n";
    dangerous();

    std::cout << "--- safe (RAII, make_unique) ---\n";
    safe();

    std::cout << "Принцип: в современном C++ new/delete не пишут вручную.\n";
    return 0;
}
