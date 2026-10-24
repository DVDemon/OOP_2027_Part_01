// Пример 03: порядок конструирования и разрушения объектов.
//
// Демонстрирует:
//   * при создании: конструктор базы -> члены базы -> члены наследника -> тело;
//   * при разрушении: строго обратный порядок;
//   * что порядок задаёт список баз и объявление полей, а не список инициализации.

#include <iostream>
#include <string>
#include <utility>

// Вспомогательный член, печатающий своё создание/уничтожение.
class Member {
    std::string tag_;

public:
    explicit Member(std::string tag) : tag_(std::move(tag)) {
        std::cout << "  Member(" << tag_ << ")\n";
    }
    ~Member() {
        std::cout << "  ~Member(" << tag_ << ")\n";
    }
};

class Base {
    Member m_{"Base::m"};

public:
    Base() { std::cout << "Base()\n"; }
    ~Base() { std::cout << "~Base()\n"; }
};

class Derived : public Base {
    Member m_{"Derived::m"};

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
    //   Member(Base::m) -> Base() -> Member(Derived::m) -> Derived()
    // Разрушение — строго наоборот:
    //   ~Derived() -> ~Member(Derived::m) -> ~Base() -> ~Member(Base::m)
    return 0;
}
