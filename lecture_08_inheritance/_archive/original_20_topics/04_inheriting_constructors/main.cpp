// Пример 04: наследование конструкторов (using Base::Base).
#include <iostream>
#include <string>

class Base {
public:
    explicit Base(int x) {
        std::cout << "Base(int): " << x << '\n';
    }
    Base(int x, double y) {
        std::cout << "Base(int,double): " << x << ", " << y << '\n';
    }
};

class Derived : public Base {
    // Унаследованные конструкторы НЕ инициализируют члены наследника,
    // поэтому задаём значение по умолчанию прямо здесь (default member initializer).
    std::string label{"<нет>"};

public:
    using Base::Base;  // наследуем все конструкторы Base

    // Можно добавить собственный конструктор.
    Derived() : Base(0), label{"по умолчанию"} {
        std::cout << "Derived(): label=" << label << '\n';
    }

    void show() const {
        std::cout << "label = " << label << '\n';
    }
};

int main() {
    Derived d1{42};         // через using -> Base(int); label = "<нет>"
    d1.show();

    Derived d2{1, 3.14};    // через using -> Base(int,double); label = "<нет>"
    d2.show();

    Derived d3;             // собственный Derived(); label = "по умолчанию"
    d3.show();

    return 0;
}
