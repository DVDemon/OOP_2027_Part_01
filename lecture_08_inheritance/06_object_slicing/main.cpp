// Пример 06: срезка объектов (object slicing).
//
// Демонстрирует:
//   * копирование наследника в переменную базового типа отбрасывает
//     производную часть (поля и переопределения);
//   * срезку в параметре функции при передаче по значению;
//   * срезку в массиве базового типа;
//   * решение: передавать по ссылке или указателю.
//
// Примечание: виртуальные функции подробно разбираются в лекции 9;
// здесь важен сам факт потери производной части при копировании.

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
    double weight = 1.5;  // поле, которому заведомо нет места в Base

    void print() const override {
        std::cout << "Derived: x=" << x << ", y=" << y
                  << ", weight=" << weight << '\n';
    }
};

// Передача по значению вызывает срезку: параметр — копия Base-части.
void by_value(Base b) { b.print(); }

// Передача по ссылке сохраняет динамический тип объекта.
void by_ref(const Base& b) { b.print(); }

int main() {
    Derived d;

    std::cout << "sizeof(Base)    = " << sizeof(Base) << '\n';
    std::cout << "sizeof(Derived) = " << sizeof(Derived) << '\n';

    std::cout << "\n-- прямой вызов --\n";
    d.print();                       // Derived: x=10, y=20

    std::cout << "\n-- срезка при копировании --\n";
    Base b = d;                      // СРЕЗКА: копируется только Base-часть
    b.print();                       // Base: x=10 — поле y потеряно
    by_value(d);                     // СРЕЗКА в параметре функции

    std::cout << "\n-- срезка в массиве --\n";
    Base arr[2] = {d, d};            // обе ячейки — «обрезанные» Base
    for (const Base& e : arr) {
        e.print();                   // Base: x=10
    }

    std::cout << "\n-- без срезки: ссылка и указатель --\n";
    by_ref(d);
    Base& ref = d;
    ref.print();
    Base* ptr = &d;
    ptr->print();

    return 0;
}
