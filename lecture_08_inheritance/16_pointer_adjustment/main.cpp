// Пример 06: корректировка указателей (pointer adjustment).
//
// Демонстрирует:
//   * при множественном наследовании приведение к разным базам может менять
//     числовое значение указателя;
//   * static_cast/dynamic_cast выполняют корректировку автоматически;
//   * reinterpret_cast НЕ корректирует -> навигация по иерархии им запрещена.

#include <cassert>
#include <iostream>

class A {
    int a = 1;
public:
    virtual ~A() = default;
    void hello_a() const { std::cout << "A::hello_a, a=" << a << '\n'; }
};

class B {
    int b = 2;
public:
    virtual ~B() = default;
    void hello_b() const { std::cout << "B::hello_b, b=" << b << '\n'; }
};

// C наследуется от двух баз: A-подобъект и B-подобъект лежат по разным смещениям.
class C : public A, public B {
    int c = 3;
public:
    void hello_c() const { std::cout << "C::hello_c, c=" << c << '\n'; }
};

int main() {
    C obj;
    C* pc = &obj;

    // Приведение вверх к базам — компилятор корректирует адрес автоматически.
    A* pa = &obj;  // обычно совпадает с началом C
    B* pb = &obj;  // СМЕЩЁН: B-подобъект лежит после A-подобъекта

    std::cout << "C*: " << static_cast<const void*>(pc) << '\n';
    std::cout << "A*: " << static_cast<const void*>(pa) << '\n';
    std::cout << "B*: " << static_cast<const void*>(pb) << '\n';
    std::cout << "pa == pc (как void*)? " << std::boolalpha
              << (static_cast<const void*>(pa) == static_cast<const void*>(pc))
              << '\n';
    std::cout << "pb == pc (как void*)? "
              << (static_cast<const void*>(pb) == static_cast<const void*>(pc))
              << "  <- обычно false: указатель скорректирован\n";

    // Обратное приведение к C — снова корректировка, получаем исходный адрес.
    C* back = static_cast<C*>(pb);
    assert(back == pc);
    std::cout << "static_cast<C*>(pb) вернул исходный объект? "
              << (back == pc) << '\n';

    // ОПАСНО: reinterpret_cast НЕ выполняет корректировку.
    // reinterpret_cast<C*>(pb) дал бы неверный адрес и UB при разыменовании.
    // Поэтому для навигации по иерархии используем static_cast/dynamic_cast.

    pa->hello_a();
    pb->hello_b();
    back->hello_c();

    return 0;
}
