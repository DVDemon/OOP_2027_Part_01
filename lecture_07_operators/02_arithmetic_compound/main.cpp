// Пример 02: арифметические (+, -, *) и составные (+=, -=, *=) операторы.
#include <iostream>

#include "vector2d.h"

int main() {
    Vector2D a(1.0, 2.0);
    Vector2D b(3.0, 4.0);

    std::cout << "a       = " << a << '\n';
    std::cout << "b       = " << b << '\n';
    std::cout << "a + b   = " << a + b << '\n';
    std::cout << "b - a   = " << b - a << '\n';
    std::cout << "-a      = " << -a << '\n';
    std::cout << "a * 2   = " << a * 2.0 << '\n';
    std::cout << "3 * b   = " << 3.0 * b << '\n';

    // Составной оператор изменяет объект на месте.
    a += b;
    std::cout << "после a += b: a = " << a << '\n';

    return 0;
}
