// Пример 08: спецификатор final для классов и методов.
#include <iostream>

// final у класса: от Framework нельзя наследоваться.
class Framework final {
public:
    void run() const { std::cout << "Framework::run\n"; }
};

// class MyFramework : public Framework {};  // ОШИБКА: Framework помечен final

class Base {
public:
    virtual ~Base() = default;

    // final у метода: process() нельзя переопределить в наследниках.
    virtual void process() final {
        std::cout << "Base::process (переопределение запрещено)\n";
    }

    virtual void handle() {
        std::cout << "Base::handle\n";
    }
};

class Derived : public Base {
public:
    // void process() override {}   // ОШИБКА: process() помечен final

    // handle() переопределять можно.
    void handle() override {
        std::cout << "Derived::handle\n";
    }
};

int main() {
    Framework fw;
    fw.run();

    Derived d;
    d.process();   // вызывается Base::process
    d.handle();    // вызывается Derived::handle

    Base& ref = d;
    ref.handle();  // полиморфизм: Derived::handle
    return 0;
}
