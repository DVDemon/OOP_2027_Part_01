// Пример 06: деструкторы и порядок уничтожения.
//   - поля уничтожаются в ОБРАТНОМ порядке объявления;
//   - локальные объекты уничтожаются в обратном порядке создания.
#include <iostream>

class A {
public:
    A() { std::cout << "A ctor\n"; }
    ~A() { std::cout << "A dtor\n"; }
};

class B {
public:
    B() { std::cout << "B ctor\n"; }
    ~B() { std::cout << "B dtor\n"; }
};

class C {
    A a_;  // создаётся первым,  уничтожается последним
    B b_;  // создаётся вторым,  уничтожается первым

public:
    C() { std::cout << "C ctor\n"; }
    ~C() { std::cout << "C dtor\n"; }
};

int main() {
    std::cout << "--- порядок полей внутри C ---\n";
    {
        C c;
        // Создание: A ctor -> B ctor -> C ctor
        // Уничтожение: C dtor -> B dtor -> A dtor
    }

    std::cout << "--- порядок локальных объектов ---\n";
    {
        A first;   // создан первым
        B second;  // создан вторым
        // Уничтожение в обратном порядке: B dtor -> A dtor
        (void)first;
        (void)second;
    }

    std::cout << "--- конец main ---\n";
    return 0;
}
