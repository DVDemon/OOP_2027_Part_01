// Пример 05: сокрытие имён (name hiding) и директива using.
#include <iostream>

class Base {
public:
    void foo(int x) const { std::cout << "Base::foo(int): " << x << '\n'; }
    void foo(double x) const { std::cout << "Base::foo(double): " << x << '\n'; }
};

// Без using: объявление foo(int) скрыло БЫ все foo из Base.
// Хороший вариант: возвращаем перегрузки базы в область видимости через using.
class Derived : public Base {
public:
    using Base::foo;  // "воскрешаем" все foo из Base

    void foo(int x) const {
        std::cout << "Derived::foo(int): " << x << '\n';
    }
};

int main() {
    Derived d;

    d.foo(42);     // Derived::foo(int) — наша перегрузка
    d.foo(3.14);   // Base::foo(double) — доступна благодаря using

    // Без using: d.foo(3.14) вызвал бы Derived::foo(int) с неявным
    // преобразованием 3.14 -> 3, что почти всегда не то, что ожидалось.

    // Перегрузку базы всегда можно вызвать и явно:
    d.Base::foo(7);

    return 0;
}
