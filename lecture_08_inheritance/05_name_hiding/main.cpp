// Пример 05: сокрытие имён (name hiding) и директива using.
//
// Демонстрирует:
//   * объявление метода в наследнике скрывает ВСЕ одноимённые перегрузки базы;
//   * типичную ловушку: d.foo(3.14) молча вызывает foo(int);
//   * возврат перегрузок базы в область видимости через using Base::foo;
//   * явный вызов перегрузки базы через Base::foo.

#include <iostream>
#include <string>

class Base {
public:
    void foo(int x) const { std::cout << "Base::foo(int): " << x << '\n'; }
    void foo(double x) const { std::cout << "Base::foo(double): " << x << '\n'; }
    void foo(const std::string &) const { std::cout << "Base::foo(string) \n";};
};

// Без using: объявление foo(int) скрывает обе перегрузки Base.
class HidingDerived : public Base {
public:
    void foo(int x) const { std::cout << "HidingDerived::foo(int): " << x << '\n'; }
};

// С using: перегрузки базы возвращаются в область видимости наследника.
class UsingDerived : public Base {
public:
    using Base::foo;  // "воскрешаем" все foo из Base

    void foo(int x) const {
        std::cout << "UsingDerived::foo(int): " << x << '\n';
    }
};

int main() {
    std::cout << "=== без using ===\n";
    HidingDerived h;
    const double pi = 3.14;
    h.foo(42);     // HidingDerived::foo(int)
    h.foo(pi);     // HidingDerived::foo(int) с неявным 3.14 -> 3 (ловушка!)
    std::string str("Bla bla bla");
    h.Base::foo(str);
    h.Base::foo(pi);  // обойти можно только явной квалификацией

    std::cout << "\n=== с using ===\n";
    UsingDerived u;
    u.foo(42);     // UsingDerived::foo(int)
    u.foo(3.14);   // Base::foo(double) — перегрузка снова видна
    u.foo(str);

    return 0;
}
