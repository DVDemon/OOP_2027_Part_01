// Пример 03: Перегрузка шаблонов и обычных функций.
//
// Демонстрирует:
//   * перегрузку шаблонов с разным количеством параметров;
//   * перегрузку шаблона и обычной функции;
//   * приоритет вызова.

#include <iostream>
#include <format>
#include <string>
#include <vector>

// 1. Обычная функция
void print(const std::string& s) {
    std::cout << std::format("обычная: {}\n", s);
}

// 2. Шаблон для одного аргумента
template <typename T>
void print(const T& value) {
    std::cout << std::format("шаблон: {}\n", value);
}

// 3. Шаблон для двух аргументов
template <typename T, typename U>
void print(const T& a, const U& b) {
    std::cout << std::format("шаблон(2): {} и {}\n", a, b);
}

// 4. Специализация для vector
template <typename T>
void print(const std::vector<T>& vec) {
    std::cout << "вектор: [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i + 1 < vec.size()) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    std::cout << "=== Перегрузка ===\n";

    // Вызов обычной функции (точное совпадение)
    print(std::string("Hello"));

    // Вызов шаблона (int не совпадает с string)
    print(42);

    // Вызов шаблона с двумя параметрами
    print(10, 20.5);

    // Вызов специализации для vector
    print(std::vector{1, 2, 3, 4, 5});

    // Явный вызов шаблона
    print<int>(100);

    return 0;
}
