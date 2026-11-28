#pragma once

// LSP — принцип подстановки Лисков.
//
// Контрпример Square : public Rectangle нарушает LSP: переопределённый
// set_width() меняет ещё и высоту, ломая ожидания клиента, написанного для
// Rectangle. Решение — НЕ наследовать Square от Rectangle, а сделать обе
// фигуры неизменяемыми подтипами общей абстракции Shape.

namespace shapes {

// Общая абстракция. Контракт: area() возвращает неотрицательную площадь
// и не имеет побочных эффектов.
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

// Неизменяемый прямоугольник: ширина и высота задаются в конструкторе.
class Rectangle : public Shape {
    double w_, h_;

public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double width() const { return w_; }
    double height() const { return h_; }
    double area() const override { return w_ * h_; }
};

// Квадрат — отдельный подтип Shape, а не Rectangle.
// Он не наследует мутирующих сеттеров и не может нарушить чужие инварианты.
class Square : public Shape {
    double side_;

public:
    explicit Square(double side) : side_(side) {}
    double side() const { return side_; }
    double area() const override { return side_ * side_; }
};

}  // namespace shapes
