// Пример 09: рекурсия — прямая, хвостовая, взаимная.
#include <iostream>

#include "recursion.h"

int main() {
    // Прямая рекурсия.
    std::cout << "factorial(5)        = " << rec::factorial(5) << '\n';        // 120

    // Хвостовая рекурсия (тот же результат, удобна для оптимизации в цикл).
    std::cout << "factorial_tail(5)   = " << rec::factorial_tail(5) << '\n';   // 120

    // Взаимная рекурсия: is_even вызывает is_odd и наоборот.
    std::cout << std::boolalpha;
    std::cout << "is_even(4) = " << rec::is_even(4) << '\n';  // true
    std::cout << "is_odd(7)  = " << rec::is_odd(7) << '\n';   // true

    return 0;
}
