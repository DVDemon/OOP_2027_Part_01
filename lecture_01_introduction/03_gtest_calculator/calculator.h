#pragma once

namespace calc {

int add(int a, int b);
int subtract(int a, int b);

// При делении на ноль возвращает +inf (std::numeric_limits<double>::infinity()).
double divide(double a, double b);

// constexpr-функция: может вычисляться на этапе компиляции.
// Рекурсивный факториал для n >= 0.
constexpr long long factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}

}  // namespace calc
