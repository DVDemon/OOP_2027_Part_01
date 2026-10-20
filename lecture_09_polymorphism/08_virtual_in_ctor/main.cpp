// Пример 08: виртуальные вызовы из конструктора и деструктора.
//
// Во время конструирования/разрушения объект "является" только своим текущим
// уровнем иерархии: vptr указывает на vtable ТЕКУЩЕГО класса. Поэтому
// виртуальные вызовы НЕ диспетчеризуются на потомка.
#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base(): вызываю init()\n";
        init();  // вызовет Base::init, даже если создаём Derived!
    }
    virtual void init() {
        std::cout << "  Base::init\n";
    }
    virtual ~Base() {
        std::cout << "~Base(): вызываю cleanup()\n";
        cleanup();  // тоже Base::cleanup, потомок уже разрушен
    }
    virtual void cleanup() {
        std::cout << "  Base::cleanup\n";
    }
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived(): тело конструктора потомка\n";
    }
    void init() override {
        std::cout << "  Derived::init\n";
    }
    void cleanup() override {
        std::cout << "  Derived::cleanup\n";
    }
    ~Derived() override {
        std::cout << "~Derived(): тело деструктора потомка\n";
    }
};

// ХОРОШО: двухфазная инициализация — вызываем виртуальный метод ПОСЛЕ
// полного построения объекта, когда полиморфизм уже работает.
void buildAndInit() {
    Derived d;
    std::cout << "После конструктора — теперь объект полностью Derived:\n";
    d.init();  // здесь это уже Derived::init
}

int main() {
    std::cout << "=== Создание и разрушение Derived ===\n";
    {
        Derived d;
        // В конструкторе init() вызвал Base::init, НЕ Derived::init!
    }

    std::cout << "\n=== Правильно: инициализация после построения ===\n";
    buildAndInit();

    return 0;
}
