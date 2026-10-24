#pragma once

#include <string>

namespace shapes {

// Базовый класс иерархии фигур. Координаты центра + полиморфный интерфейс.
class Shape {
protected:
    double x_;
    double y_;

public:
    Shape(double x, double y) noexcept : x_(x), y_(y) {}
    virtual ~Shape() = default;  // ОБЯЗАТЕЛЬНО для полиморфного базового класса

    virtual double area() const noexcept = 0;
    virtual double perimeter() const noexcept = 0;

    // Невиртуальная часть info() общая, описание фигуры — через name().
    std::string info() const;
    virtual std::string name() const { return "Shape"; }

    double x() const noexcept { return x_; }
    double y() const noexcept { return y_; }
};

// Прямоугольник — это фигура (is-a).
class Rectangle : public Shape {
protected:
    double width_;
    double height_;

public:
    Rectangle(double x, double y, double w, double h) noexcept
        : Shape(x, y), width_(w), height_(h) {}

    double area() const noexcept override { return width_ * height_; }
    double perimeter() const noexcept override { return 2.0 * (width_ + height_); }
    std::string name() const override { return "Rectangle"; }

    double width() const noexcept { return width_; }
    double height() const noexcept { return height_; }
};

// Квадрат как наследник Rectangle. С точки зрения LSP это спорно:
// у Rectangle можно независимо менять стороны, у Square — нет.
// Поэтому мы НЕ даём независимых сеттеров сторон и фиксируем инвариант
// в конструкторе (сторона одна). Конструктор наследуем через using.
class Square final : public Rectangle {  // final: дальше наследоваться запрещаем
public:
    Square(double x, double y, double side) noexcept
        : Rectangle(x, y, side, side) {}

    std::string name() const override { return "Square"; }
};

}  // namespace shapes
