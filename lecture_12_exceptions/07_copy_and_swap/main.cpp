// Пример 07: copy-and-swap для строгой гарантии.
//
// Демонстрируем работу operator=, реализованного через copy-and-swap.
#include <iostream>

#include "buffer.h"

int main() {
    Buffer a(3);
    a.at(0) = 10;
    a.at(1) = 20;
    a.at(2) = 30;

    Buffer b(1);
    b.at(0) = 99;

    std::cout << "До присваивания: a.size = " << a.size()
              << ", b.size = " << b.size() << '\n';

    b = a;  // copy-and-swap: создаётся копия a, затем swap с b

    std::cout << "После b = a:      b.size = " << b.size()
              << ", b.at(2) = " << b.at(2) << '\n';

    // Самоприсваивание безопасно: копия создаётся до swap.
    // (Подавляем предупреждение компилятора — здесь это намеренная демонстрация.)
#if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wself-assign-overloaded"
#endif
    a = a;
#if defined(__clang__)
#  pragma clang diagnostic pop
#endif
    std::cout << "После a = a:      a.at(1) = " << a.at(1) << '\n';

    // Проверка границ через at — строгая защита (бросает out_of_range).
    try {
        a.at(100);
    } catch (const std::exception& e) {
        std::cout << "at(100): " << e.what() << '\n';
    }

    return 0;
}
