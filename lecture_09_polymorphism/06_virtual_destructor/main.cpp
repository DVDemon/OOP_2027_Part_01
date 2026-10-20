// Пример 06: виртуальный деструктор (обязателен для полиморфных классов).
#include <iostream>
#include <memory>

// ХОРОШО: у полиморфного базового класса деструктор виртуальный.
class Base {
public:
    Base() { std::cout << "  Base()\n"; }
    virtual void use() const { std::cout << "  Base::use\n"; }
    // Виртуальный деструктор: delete через Base* вызовет деструктор потомка.
    virtual ~Base() { std::cout << "  ~Base()\n"; }
};

class Derived : public Base {
    std::unique_ptr<int[]> data_;  // ресурс, который нужно освободить
public:
    Derived() : data_(std::make_unique<int[]>(1000)) {
        std::cout << "  Derived() — выделил 1000 int\n";
    }
    void use() const override { std::cout << "  Derived::use\n"; }
    ~Derived() override {
        std::cout << "  ~Derived() — освобождаю ресурс\n";
    }
};

int main() {
    std::cout << "Удаление через указатель на базовый класс:\n";
    {
        Base* ptr = new Derived();
        ptr->use();
        delete ptr;  // виртуальный ~Base => вызовется ~Derived, затем ~Base
    }

    std::cout << "\nТо же через unique_ptr<Base> (рекомендуемый способ):\n";
    {
        std::unique_ptr<Base> ptr = std::make_unique<Derived>();
        ptr->use();
        // деструктор сработает автоматически и полиморфно
    }

    return 0;
}
