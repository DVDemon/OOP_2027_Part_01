// Пример 07: использование учебной реализации UniquePtr.
#include <iostream>
#include <string>
#include <utility>

#include "unique_ptr.h"

struct Tracked {
    int value;
    explicit Tracked(int v) : value(v) {
        std::cout << "Tracked(" << value << ") создан\n";
    }
    ~Tracked() { std::cout << "Tracked(" << value << ") разрушен\n"; }
};

int main() {
    auto p = edu::makeUnique<std::string>("Hello");
    std::cout << "*p = " << *p << ", p->size() = " << p->size() << '\n';

    // Перемещение: владение переходит, исходный становится пустым.
    auto p2 = std::move(p);
    std::cout << "после move p пуст? " << (p ? "нет" : "да") << '\n';
    std::cout << "*p2 = " << *p2 << '\n';

    // Демонстрация автоматического освобождения и reset.
    {
        auto t = edu::makeUnique<Tracked>(1);
        std::cout << "t->value = " << t->value << '\n';
        t.reset(new Tracked(2));  // старый Tracked(1) разрушится здесь
        std::cout << "после reset t->value = " << t->value << '\n';
    }  // здесь разрушится Tracked(2)

    // release: отдаём ресурс наружу, отвечаем за delete сами.
    auto r = edu::makeUnique<Tracked>(3);
    Tracked* raw = r.release();
    std::cout << "после release r пуст? " << (r ? "нет" : "да") << '\n';
    delete raw;  // вызывающий обязан освободить вручную

    return 0;
}
