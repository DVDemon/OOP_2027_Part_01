// Пример 01: Простейший шаблон функции.
//
// Демонстрирует:
//   * базовый синтаксис template<typename T>;
//   * шаблон max для разных типов;
//   * шаблон swap;
//   * инстанцирование шаблона.

#include <iostream>
#include <format>
#include <string>

// Шаблон max (своя версия, чтобы не конфликтовать с std::max)
template <typename T>
T my_max(T a, T b) {
    return (a > b) ? a : b;
}

// Шаблон swap
template <typename T>
void my_swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

int main() {
    // int
    int x = 10, y = 20;
    std::cout << std::format("max({}, {}) = {}\n", x, y, my_max(x, y));

    // double
    double a = 3.14, b = 2.71;
    std::cout << std::format("max({}, {}) = {}\n", a, b, my_max(a, b));

    // string
    std::string s1 = "apple", s2 = "orange";
    std::cout << std::format("max({}, {}) = {}\n", s1, s2, my_max(s1, s2));

    // swap
    std::cout << std::format("\nДо swap: x={}, y={}\n", x, y);
    my_swap(x, y);
    std::cout << std::format("После swap: x={}, y={}\n", x, y);

    // Явное указание типа
    std::cout << std::format("\nЯвное указание: my_max<double>(10, 20.5) = {}\n",
        my_max<double>(10, 20.5));

    return 0;
}
