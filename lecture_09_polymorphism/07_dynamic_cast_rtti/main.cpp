// Пример 07: dynamic_cast, RTTI и typeid.
#include <iostream>
#include <typeinfo>

class Base {
public:
    // Для dynamic_cast/typeid нужен полиморфный класс (хотя бы одна virtual).
    virtual ~Base() = default;
};

class Derived1 : public Base {
public:
    void special1() const { std::cout << "  Derived1::special1\n"; }
};

class Derived2 : public Base {
public:
    void special2() const { std::cout << "  Derived2::special2\n"; }
};

// dynamic_cast для указателя: при неудаче возвращает nullptr.
void process(Base* b) {
    if (auto* d1 = dynamic_cast<Derived1*>(b)) {
        d1->special1();  // безопасно: b действительно Derived1
    } else if (auto* d2 = dynamic_cast<Derived2*>(b)) {
        d2->special2();
    } else {
        std::cout << "  неизвестный тип\n";
    }
}

// typeid возвращает информацию о фактическом типе во время выполнения.
void identify(const Base& b) {
    std::cout << "  typeid(b).name() = " << typeid(b).name() << '\n';
    if (typeid(b) == typeid(Derived1)) {
        std::cout << "  это Derived1\n";
    }
}

int main() {
    Derived1 d1;
    Derived2 d2;
    Base base;

    std::cout << "dynamic_cast для указателя (nullptr при неудаче):\n";
    process(&d1);
    process(&d2);
    process(&base);

    std::cout << "\ntypeid для распознавания типа:\n";
    identify(d1);
    identify(d2);

    std::cout << "\ndynamic_cast для ссылки (бросает std::bad_cast при неудаче):\n";
    try {
        Base& ref = base;
        [[maybe_unused]] Derived1& bad = dynamic_cast<Derived1&>(ref);
        std::cout << "  приведение удалось (не ожидалось)\n";
    } catch (const std::bad_cast& e) {
        std::cout << "  поймано исключение std::bad_cast: " << e.what() << '\n';
    }

    return 0;
}
