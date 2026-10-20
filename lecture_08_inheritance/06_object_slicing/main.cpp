// Пример 06: срезка объектов (object slicing).
#include <iostream>

class Base {
public:
    int x = 10;
    virtual ~Base() = default;
    virtual void print() const {
        std::cout << "Base: x=" << x << '\n';
    }
};

class Derived : public Base {
public:
    int y = 20;
    void print() const override {
        std::cout << "Derived: x=" << x << ", y=" << y << '\n';
    }
};

// Передача по значению вызывает срезку: параметр — это копия Base-части.
void by_value(Base b) { b.print(); }

// Передача по ссылке сохраняет полиморфизм.
void by_ref(const Base& b) { b.print(); }

int main() {
    Derived d;

    std::cout << "-- прямой вызов --\n";
    d.print();                       // Derived: x=10, y=20

    std::cout << "-- срезка (по значению) --\n";
    Base b = d;                      // СРЕЗКА: копируется только Base-часть
    b.print();                       // Base: x=10 — полиморфизм потерян
    by_value(d);                     // СРЕЗКА в параметре функции

    std::cout << "-- без срезки (ссылка / указатель) --\n";
    by_ref(d);                       // Derived: полиморфизм работает
    Base& ref = d;
    ref.print();                     // Derived
    Base* ptr = &d;
    ptr->print();                    // Derived

    return 0;
}
