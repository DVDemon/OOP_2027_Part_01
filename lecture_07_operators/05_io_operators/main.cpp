// Пример 05: операторы ввода/вывода << и >>.
#include <iostream>
#include <sstream>

// Комплексное число. Операторы потока — ВСЕГДА свободные функции,
// так как левый операнд — это поток (std::ostream / std::istream).
class Complex {
    double re_;
    double im_;
public:
    Complex(double r = 0.0, double i = 0.0) : re_(r), im_(i) {}

    // Вывод: возвращаем ссылку на поток для цепочки cout << a << b.
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.re_;
        if (c.im_ >= 0) os << '+';
        os << c.im_ << 'i';
        return os;
    }

    // Ввод: формат "re im", например "3 -4".
    friend std::istream& operator>>(std::istream& is, Complex& c) {
        is >> c.re_ >> c.im_;
        return is;
    }
};

int main() {
    Complex a(3, -4);
    Complex b(1, 2);

    // Цепочка вывода работает благодаря возврату ostream&.
    std::cout << "a = " << a << ", b = " << b << '\n';

    // Читаем из строкового потока (демонстрация operator>>).
    std::istringstream in("5 6");
    Complex c;
    in >> c;
    std::cout << "разобрали из \"5 6\": " << c << '\n';

    return 0;
}
