// Пример 03: порядок конструирования и разрушения объектов.
#include <iostream>
#include <string>

// Вспомогательный член, печатающий своё создание/уничтожение.
class Member {
    std::string tag;

public:
    explicit Member(std::string t) : tag(std::move(t)) {
        std::cout << "  Member(" << tag << ")\n";
    }
    ~Member() {
        std::cout << "  ~Member(" << tag << ")\n";
    }
};

class Base {
    Member m{"Base::m"};

public:
    Base() { std::cout << "Base()\n"; }
    ~Base() { std::cout << "~Base()\n"; }
};

class Derived : public Base {
    Member m{"Derived::m"};

public:
    Derived() { std::cout << "Derived()\n"; }
    ~Derived() { std::cout << "~Derived()\n"; }
};

int main() {
    std::cout << "=== создание Derived ===\n";
    {
        Derived d;
        std::cout << "=== объект готов ===\n";
    }  // здесь d разрушается
    std::cout << "=== область видимости покинута ===\n";

    // Ожидаемый порядок:
    // 1) конструктор базы (и её члены), затем члены наследника, затем тело Derived();
    // 2) разрушение — строго в обратном порядке.
    return 0;
}
