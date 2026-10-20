// Пример 06: dynamic_pointer_cast / static_pointer_cast для shared_ptr.
#include <iostream>
#include <memory>

class Base {
public:
    virtual ~Base() = default;
    virtual void info() const { std::cout << "Base\n"; }
};

class Derived : public Base {
public:
    void info() const override { std::cout << "Derived\n"; }
    void special() const { std::cout << "Derived::special\n"; }
};

class Other : public Base {
public:
    void info() const override { std::cout << "Other\n"; }
};

int main() {
    std::shared_ptr<Base> base = std::make_shared<Derived>();

    // static_pointer_cast — без проверки типа (как static_cast).
    // Корректно только если мы ТОЧНО знаем фактический тип.
    auto d1 = std::static_pointer_cast<Derived>(base);
    d1->special();

    // dynamic_pointer_cast — с проверкой типа (как dynamic_cast).
    // Возвращает пустой shared_ptr, если фактический тип не подходит.
    if (auto d2 = std::dynamic_pointer_cast<Derived>(base)) {
        std::cout << "dynamic_pointer_cast<Derived> успешен: ";
        d2->special();
    }

    if (auto o = std::dynamic_pointer_cast<Other>(base)) {
        o->info();
    } else {
        std::cout << "dynamic_pointer_cast<Other> вернул nullptr "
                     "(base — не Other)\n";
    }

    // Все shared_ptr, полученные приведением, разделяют один control block.
    std::cout << "use_count = " << base.use_count() << '\n';  // base + d1 = 2

    return 0;
}
