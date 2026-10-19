// Пример 01: метод класса vs свободная функция; что можно/нельзя перегружать.
#include <iostream>

// Класс рационального числа (дробь).
class Fraction {
    int num_;  // числитель
    int den_;  // знаменатель
public:
    Fraction(int n = 0, int d = 1) : num_(n), den_(d) {}

    // --- Оператор как МЕТОД: левый операнд всегда Fraction ---
    // Подходит, когда левый операнд по смыслу не нуждается в преобразовании.
    Fraction operator*(const Fraction& rhs) const {
        return Fraction(num_ * rhs.num_, den_ * rhs.den_);
    }

    // --- Оператор как СВОБОДНАЯ функция (friend) ---
    // Нужен, чтобы работало "2 * frac": левый операнд — int, который
    // неявно преобразуется в Fraction. Метод здесь НЕ помог бы:
    // у int нет метода operator* для Fraction.
    friend Fraction operator*(int lhs, const Fraction& rhs) {
        return Fraction(lhs * rhs.num_, rhs.den_);
    }

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        return os << f.num_ << '/' << f.den_;
    }
};

int main() {
    Fraction half(1, 2);
    Fraction third(1, 3);

    // Левый операнд — Fraction: вызывается метод.
    std::cout << "half * third = " << half * third << '\n';

    // Левый операнд — int: вызывается свободная функция.
    std::cout << "2 * half     = " << 2 * half << '\n';

    // half * 2 тоже работает: 2 неявно станет Fraction(2, 1) для метода.
    std::cout << "half * 2     = " << half * 2 << '\n';

    return 0;
}
