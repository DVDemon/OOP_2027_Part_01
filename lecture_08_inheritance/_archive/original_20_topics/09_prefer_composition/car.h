#pragma once

#include <string>

namespace vehicle {

// Двигатель — отдельная сущность со своим поведением.
class Engine {
    int power_;  // мощность, л.с.

public:
    explicit Engine(int power) noexcept : power_(power) {}

    int power() const noexcept { return power_; }
    std::string ignite() const;  // "запуск" двигателя
};

// Машина СОДЕРЖИТ двигатель (отношение has-a) — это композиция, не наследование.
class Car {
    Engine engine_;
    std::string model_;

public:
    Car(std::string model, int power)
        : engine_(power), model_(std::move(model)) {}

    std::string start() const;       // делегирует работу двигателю
    int horsepower() const noexcept; // делегирует engine_.power()
    const std::string& model() const noexcept { return model_; }
};

}  // namespace vehicle
