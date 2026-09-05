// Пример 08: использование учебной реализации SharedPtr.
#include <iostream>

#include "shared_ptr.h"

struct Tracked {
    int value;
    explicit Tracked(int v) : value(v) {
        std::cout << "Tracked(" << value << ") создан\n";
    }
    ~Tracked() { std::cout << "Tracked(" << value << ") разрушен\n"; }
};

int main() {
    edu::SharedPtr<Tracked> a(new Tracked(42));
    std::cout << "count = " << a.use_count() << '\n';  // 1

    {
        edu::SharedPtr<Tracked> b = a;  // копирование — счётчик растёт
        std::cout << "count = " << a.use_count() << '\n';  // 2
        std::cout << "b->value = " << b->value << '\n';
    }  // b разрушен — счётчик уменьшается, объект ещё жив

    std::cout << "count = " << a.use_count() << '\n';  // 1

    // Присваивание копированием: старый ресурс c освобождается, новый разделяется.
    edu::SharedPtr<Tracked> c(new Tracked(7));
    c = a;  // Tracked(7) разрушится здесь, c теперь разделяет Tracked(42)
    std::cout << "после присваивания count = " << a.use_count() << '\n';  // 2
    std::cout << "c->value = " << c->value << '\n';

    return 0;
}  // последние владельцы a и c уходят — Tracked(42) разрушится
