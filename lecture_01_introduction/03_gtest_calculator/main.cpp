// Демонстрационный запуск калькулятора (без тестов).
#include <iostream>

#include "calculator.h"

int main() {
    std::cout << "add(2, 3)      = " << calc::add(2, 3) << '\n';
    std::cout << "subtract(5, 3) = " << calc::subtract(5, 3) << '\n';
    std::cout << "divide(10, 4)  = " << calc::divide(10.0, 4.0) << '\n';
    std::cout << "divide(1, 0)   = " << calc::divide(1.0, 0.0) << " (inf)\n";
    std::cout << "factorial(5)   = " << calc::factorial(5) << '\n';
    return 0;
}
