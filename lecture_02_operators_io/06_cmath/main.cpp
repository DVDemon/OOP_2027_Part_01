// Пример 06: математические функции <cmath>.
#include <cmath>
#include <iostream>
#include <numbers>  // std::numbers::pi, std::numbers::e (C++20)

#include "float_compare.h"

int main() {
    std::cout << "=== Базовые функции ===\n";
    std::cout << "abs(-5)     = " << std::abs(-5) << '\n';
    std::cout << "sqrt(144)   = " << std::sqrt(144.0) << '\n';
    std::cout << "pow(2, 10)  = " << std::pow(2.0, 10) << '\n';
    std::cout << "ceil(3.2)   = " << std::ceil(3.2) << '\n';
    std::cout << "floor(3.8)  = " << std::floor(3.8) << '\n';
    std::cout << "round(3.5)  = " << std::round(3.5) << '\n';

    std::cout << "\n=== Тригонометрия (радианы) ===\n";
    // Используем std::numbers::pi (C++20) вместо нестандартного M_PI.
    constexpr double pi = std::numbers::pi;
    std::cout << "sin(pi/2)   = " << std::sin(pi / 2) << '\n';
    std::cout << "cos(0)      = " << std::cos(0.0) << '\n';

    std::cout << "\n=== Логарифмы ===\n";
    std::cout << "log(e)      = " << std::log(std::numbers::e) << '\n';
    std::cout << "log10(1000) = " << std::log10(1000.0) << '\n';
    std::cout << "log2(1024)  = " << std::log2(1024.0) << '\n';

    std::cout << "\n=== Гипотенуза без переполнения ===\n";
    std::cout << "hypot(3, 4) = " << mathx::hypotenuse(3.0, 4.0) << '\n';

    std::cout << "\n=== Сравнение double: == не работает ===\n";
    double sum = 0.1 + 0.2;  // не ровно 0.3 из-за двоичного представления
    std::cout << std::boolalpha;
    std::cout << "0.1 + 0.2 == 0.3            : " << (sum == 0.3)
              << "  (ПЛОХО, может быть false)\n";
    std::cout << "almost_equal(0.1+0.2, 0.3)  : "
              << mathx::almost_equal(sum, 0.3) << "  (ХОРОШО)\n";

    return 0;
}
