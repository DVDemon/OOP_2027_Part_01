// Пример 04: наследование конструкторов (using Base::Base).
//
// Демонстрирует:
//   * using Base::Base наследует все конструкторы базы;
//   * унаследованный конструктор НЕ инициализирует поля наследника —
//     для них работают default member initializers;
//   * копирующий и перемещающий конструкторы так не наследуются;
//   * собственный конструктор наследника можно добавить рядом.

#include <iostream>
#include <string>
#include <utility>

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
    // Поле наследника не инициализируется унаследованным конструктором.
    std::string label_{"<нет>"};

public:
    using Base::Base;  // наследуем все конструкторы Base

    // Собственный конструктор: явно вызывает базу и задаёт label_.
    Derived() : Base(0), label_{"по умолчанию"} {
        std::cout << "Derived(): label=" << label_ << '\n';
    }

    void show() const {
        std::cout << "label = " << label_ << '\n';
    }
};

int main() {
    Derived d1{42};         // через using -> Base(int)
    d1.show();

    Derived d2{1, 3.14};    // через using -> Base(int,double)
    d2.show();

    Derived d3;             // собственный Derived()
    d3.show();

    // Копирование не «наследуется»: это обычный копирующий конструктор,
    // который создаёт компилятор.
    Derived copy{d1};
    copy.show();

    return 0;
}
