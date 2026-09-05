// Пример 01: три вида полиморфизма — ad-hoc, параметрический, подтипов.
#include <iostream>
#include <string>

// --- 1. Ad-hoc полиморфизм: перегрузка функций ---
// Одно имя — разные реализации для разных типов аргументов.
// Выбор реализации происходит на этапе компиляции по типам.
int add(int a, int b) {
    return a + b;
}
double add(double a, double b) {
    return a + b;
}
std::string add(const std::string& a, const std::string& b) {
    return a + b;  // конкатенация
}

// --- 2. Параметрический полиморфизм: шаблоны ---
// Один код работает с любым типом, поддерживающим нужные операции.
// Компилятор генерирует отдельную версию для каждого типа.
template <typename T>
T multiply(T a, T b) {
    return a * b;
}

// --- 3. Полиморфизм подтипов: виртуальные функции ---
// Выбор реализации происходит в runtime по фактическому типу объекта.
class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
};

class Square : public Shape {
    double a_;
public:
    explicit Square(double a) : a_(a) {}
    double area() const override { return a_ * a_; }
};

// Функция работает с любым Shape, не зная конкретный тип.
void printArea(const Shape& s) {
    std::cout << "  площадь = " << s.area() << '\n';
}

int main() {
    std::cout << "1. Ad-hoc (перегрузка), выбор на этапе компиляции:\n";
    std::cout << "  add(2, 3)        = " << add(2, 3) << '\n';
    std::cout << "  add(2.5, 3.5)    = " << add(2.5, 3.5) << '\n';
    std::cout << "  add(\"a\", \"b\")  = " << add(std::string("a"), std::string("b")) << '\n';

    std::cout << "\n2. Параметрический (шаблоны), один код для всех типов:\n";
    std::cout << "  multiply(4, 5)     = " << multiply(4, 5) << '\n';
    std::cout << "  multiply(1.5, 2.0) = " << multiply(1.5, 2.0) << '\n';

    std::cout << "\n3. Подтипов (virtual), выбор в runtime:\n";
    Circle c{2.0};
    Square sq{3.0};
    printArea(c);   // вызовет Circle::area
    printArea(sq);  // вызовет Square::area

    return 0;
}
