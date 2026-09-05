// Пример 06: inline-функции и constexpr-функции.
#include <iostream>

// inline-функция: основная роль в современном C++ — разрешить определение
// в заголовке без нарушения ODR (если бы это был .h, включаемый в несколько .cpp).
// Здесь — для демонстрации; решение «встраивать или нет» компилятор принимает сам.
inline int square(int x) {
    return x * x;
}

// constexpr-функция: МОЖЕТ вычисляться на этапе компиляции,
// если все аргументы — константные выражения. Иначе вычисляется в рантайме.
constexpr long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// С C++14 constexpr-функции могут содержать циклы и локальные переменные.
constexpr int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

int main() {
    // constexpr-результат: вычислен на этапе компиляции, проверен static_assert.
    constexpr long long f5 = factorial(5);
    static_assert(f5 == 120, "factorial(5) должно быть 120 на этапе компиляции");
    static_assert(fibonacci(10) == 55, "fibonacci(10) должно быть 55");

    std::cout << "square(7)      = " << square(7) << '\n';
    std::cout << "factorial(5)   = " << f5 << '\n';
    std::cout << "fibonacci(10)  = " << fibonacci(10) << '\n';

    // Та же constexpr-функция в рантайме: аргумент известен только во время выполнения.
    int n = 0;
    std::cout << "введите n для factorial: ";
    if (!(std::cin >> n)) n = 6;  // если ввода нет — берём 6
    std::cout << "factorial(n)   = " << factorial(n) << '\n';  // вычисление в рантайме

    return 0;
}
