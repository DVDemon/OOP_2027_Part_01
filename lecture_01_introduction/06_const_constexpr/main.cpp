// Пример 06: const vs constexpr.
#include <iostream>

// constexpr-функция: может вычисляться на этапе компиляции.
constexpr int square(int x) {
    return x * x;
}

int main() {
    // --- const: неизменяемость в рантайме ---
    const int max_size = 100;
    // max_size = 200;   // ОШИБКА: присваивание const-переменной
    std::cout << "max_size = " << max_size << '\n';

    // --- constexpr: вычисление на этапе компиляции ---
    constexpr int result = square(5);
    static_assert(result == 25, "square(5) должно быть 25 на этапе компиляции");
    std::cout << "square(5) = " << result << '\n';

    // const может быть вычислен в рантайме (в отличие от constexpr):
    int n = 0;
    std::cout << "введите число: ";
    if (!(std::cin >> n)) n = 7;        // если ввода нет — берём 7
    const int runtime_const = n * 2;    // ok: const, но не constexpr
    std::cout << "runtime_const = " << runtime_const << '\n';

    return 0;
}
