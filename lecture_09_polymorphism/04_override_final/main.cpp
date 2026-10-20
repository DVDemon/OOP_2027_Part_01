// Пример 04: спецификаторы override и final.
#include <iostream>

class Base {
public:
    virtual void process(int x) const {
        std::cout << "Base::process(" << x << ")\n";
    }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    // override заставляет компилятор проверить, что мы ДЕЙСТВИТЕЛЬНО
    // переопределяем виртуальную функцию базового класса.
    void process(int x) const override {
        std::cout << "Derived::process(" << x << ")\n";
    }

    // Закомментированные строки НЕ скомпилировались бы с override:
    // void process(int x) { ... }          // нет const -> другая сигнатура
    // void process(double x) const { ... }  // другой тип параметра
    // void proces(int x) const { ... }      // опечатка в имени
    // Без override эти ошибки прошли бы молча, создав НОВУЮ функцию.
};

// final запрещает дальнейшее переопределение метода.
class Middle : public Base {
public:
    void process(int x) const final {
        std::cout << "Middle::process(" << x << ") [final]\n";
    }
};

class Bottom : public Middle {
public:
    // void process(int x) const override {} // ОШИБКА: process помечен final
    void announce() const {
        std::cout << "Bottom не может переопределить process\n";
    }
};

// final у целого класса запрещает наследование от него.
class Sealed final {
public:
    void hello() const { std::cout << "Sealed::hello\n"; }
};
// class X : public Sealed {};  // ОШИБКА: нельзя наследовать от final-класса

int main() {
    Derived d;
    Base& b = d;
    b.process(1);  // Derived::process — override работает

    Bottom bot;
    Base& mb = bot;
    mb.process(2);  // Middle::process — final-версия
    bot.announce();

    Sealed s;
    s.hello();

    return 0;
}
