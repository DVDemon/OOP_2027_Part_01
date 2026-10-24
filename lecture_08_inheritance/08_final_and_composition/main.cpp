// Пример 08: спецификатор final и принцип «предпочитайте композицию».
//
// Демонстрирует:
//   * final у класса — от него нельзя наследоваться;
//   * final у виртуального метода — его нельзя переопределить;
//   * композицию вместо наследования для отношения has-a (Car содержит Engine);
//   * нарушение LSP на примере Circle : Ellipse.

#include <iostream>
#include <string>
#include <utility>

// ---------- final у класса ------------------------------------------------

class Framework final {
public:
    void run() const { std::cout << "Framework::run\n"; }
};

// class MyFramework : public Framework {};  // ОШИБКА: Framework помечен final

// ---------- final у метода ------------------------------------------------

class Base {
public:
    virtual ~Base() = default;

    // final у метода: process() нельзя переопределить в наследниках.
    virtual void process() final {
        std::cout << "Base::process (переопределение запрещено)\n";
    }

    virtual void handle() {
        std::cout << "Base::handle\n";
    }
};

class Derived : public Base {
public:
    // void process() override {}   // ОШИБКА: process() помечен final

    void handle() override {
        std::cout << "Derived::handle\n";
    }
};

// ---------- композиция вместо наследования --------------------------------

class Engine {
    int power_;

public:
    explicit Engine(int power) noexcept : power_(power) {}
    int power() const noexcept { return power_; }
    std::string ignite() const {
        return "двигатель запущен (" + std::to_string(power_) + " л.с.)";
    }
};

// Машина СОДЕРЖИТ двигатель (has-a), а не «является» им.
// public-наследование Car : Engine нарушило бы семантику is-a.
class Car {
    Engine engine_;
    std::string model_;

public:
    Car(std::string model, int power)
        : engine_(power), model_(std::move(model)) {}

    std::string start() const { return model_ + ": " + engine_.ignite(); }
    int horsepower() const noexcept { return engine_.power(); }
    const std::string& model() const noexcept { return model_; }
};

// ---------- LSP-ловушка ---------------------------------------------------
// Круг «является» эллипсом математически, но не по контракту:
// у Ellipse можно независимо менять полуоси, у Circle — нельзя.
// class Circle : public Ellipse { ... };  // нарушение LSP

int main() {
    Framework fw;
    fw.run();

    Derived d;
    d.process();   // Base::process — переопределение запрещено
    d.handle();    // Derived::handle

    Base& ref = d;
    ref.handle();  // Derived::handle

    Car car{"Тойота", 150};
    std::cout << car.start() << '\n';
    std::cout << "модель: " << car.model()
              << ", мощность: " << car.horsepower() << " л.с.\n";

    return 0;
}
