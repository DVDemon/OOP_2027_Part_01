#pragma once

#include <string>

namespace shapes {

// --- Интерфейс: только чисто виртуальные функции + виртуальный деструктор ---
// В C++ "интерфейс" — это абстрактный класс без данных и реализаций.
class ISerializable {
public:
    virtual std::string serialize() const = 0;
    virtual ~ISerializable() = default;
};

// --- Абстрактный класс: хотя бы одна чисто виртуальная функция ---
// Создать объект Shape напрямую нельзя.
class Shape {
public:
    virtual double area() const = 0;       // чисто виртуальная
    virtual double perimeter() const = 0;  // чисто виртуальная

    // Чисто виртуальная функция МОЖЕТ иметь общую реализацию по умолчанию,
    // но потомок всё равно обязан её переопределить (или вызвать явно).
    virtual std::string name() const = 0;

    virtual ~Shape() = default;
};

// Circle реализует и абстрактный Shape, и интерфейс ISerializable.
class Circle : public Shape, public ISerializable {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override;
    double perimeter() const override;
    std::string name() const override;
    std::string serialize() const override;
};

class Rectangle : public Shape, public ISerializable {
    double w_;
    double h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override;
    double perimeter() const override;
    std::string name() const override;
    std::string serialize() const override;
};

}  // namespace shapes
