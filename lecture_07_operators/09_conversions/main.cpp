// Пример 09: операторы преобразования типа operator T(), explicit conversions.
#include <iostream>

// Дробь с двумя операторами преобразования: неявным и явным.
class Fraction {
    int num_;
    int den_;
public:
    Fraction(int n, int d = 1) : num_(n), den_(d) {}

    // Неявное преобразование в double — естественно для дроби (значение).
    operator double() const {
        return static_cast<double>(num_) / den_;
    }

    // Явное (explicit) преобразование в bool — только в булевом контексте.
    // Без explicit число случайно участвовало бы в арифметике через bool->int.
    explicit operator bool() const { return num_ != 0; }
};

int main() {
    Fraction f(3, 4);

    // Неявная конверсия в double сработала автоматически.
    double d = f;
    std::cout << "f как double = " << d << '\n';

    // explicit operator bool работает в условии (контекстное преобразование).
    if (f) {
        std::cout << "f не равно нулю\n";
    }

    Fraction zero(0, 5);
    std::cout << std::boolalpha
              << "bool(zero) = " << static_cast<bool>(zero) << '\n';

    // ВНИМАНИЕ: будь operator bool НЕявным, выражение (f + 1) втихую
    // вычислялось бы как (true + 1) == 2. explicit это запрещает.

    return 0;
}
