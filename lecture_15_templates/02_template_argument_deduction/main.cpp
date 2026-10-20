// Пример 02: Параметры шаблона.
//
// Демонстрирует:
//   * несколько параметров шаблона;
//   * параметры-типы и параметры-значения;
//   * значения по умолчанию.

#include <iostream>
#include <format>
#include <typeinfo>

// Несколько параметров шаблона
template <typename T, typename U>
auto multiply(T a, U b) -> decltype(a * b) {
    return a * b;
}

// Параметр-значение (non-type template parameter)
template <typename T, int Size>
class StaticArray {
    T data_[Size] = {};
public:
    int size() const { return Size; }
    T& operator[](int i) { return data_[i]; }
    const T& operator[](int i) const { return data_[i]; }
};

// Параметр с значением по умолчанию
template <typename T = double>
T default_precision() {
    return static_cast<T>(0.001);
}

int main() {
    // Несколько параметров
    std::cout << "=== Несколько параметров ===\n";
    std::cout << std::format("multiply<int, double>(5, 3.14) = {}\n",
        multiply<int, double>(5, 3.14));
    std::cout << std::format("multiply(3, 4.5) = {} (вывод типов)\n",
        multiply(3, 4.5));

    // Параметр-значение
    std::cout << "\n=== Параметр-значение ===\n";
    StaticArray<int, 5> arr;
    for (int i = 0; i < arr.size(); ++i) {
        arr[i] = i * 10;
    }
    std::cout << "StaticArray<int, 5>: ";
    for (int i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::format("\nРазмер: {}\n", arr.size());

    // Значение по умолчанию
    std::cout << "\n=== Значение по умолчанию ===\n";
    std::cout << std::format("default_precision<>() = {}\n", default_precision<>());
    std::cout << std::format("default_precision<float>() = {}\n", default_precision<float>());

    return 0;
}
