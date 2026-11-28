// Пример 06: OCP — принцип открытости/закрытости.
//
// «Программные сущности должны быть открыты для расширения, но закрыты
// для модификации» — Бертран Мейер.
//
// ПЛОХО — расчёт площади через цепочку if/else по типу фигуры:
//
//     double calculate(const Shape& shape) {
//         if (shape.type == "circle")    return 3.14159 * shape.r * shape.r;
//         else if (shape.type == "rect") return shape.w * shape.h;
//         // добавление новой фигуры требует ПРАВКИ этого кода
//     }
//
// ХОРОШО — полиморфизм: total_area() закрыт для модификации, иерархия
// Shape открыта для расширения (добавляем Triangle, ничего не ломая).

#include <iostream>
#include <memory>
#include <numbers>
#include <vector>

class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
    double radius_;

public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override {
        return std::numbers::pi * radius_ * radius_;
    }
};

class Rectangle : public Shape {
    double w_, h_;

public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
};

// Новая фигура добавляется БЕЗ изменения total_area().
class Triangle : public Shape {
    double base_, height_;

public:
    Triangle(double base, double height) : base_(base), height_(height) {}
    double area() const override { return 0.5 * base_ * height_; }
};

// Этот код НИКОГДА не меняется при добавлении новых фигур.
double total_area(const std::vector<std::unique_ptr<Shape>>& shapes) {
    double sum = 0.0;
    for (const auto& s : shapes) {
        sum += s->area();
    }
    return sum;
}

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(1.0));
    shapes.push_back(std::make_unique<Rectangle>(2.0, 3.0));
    shapes.push_back(std::make_unique<Triangle>(4.0, 5.0));  // расширение

    std::cout << "Суммарная площадь: " << total_area(shapes) << '\n';
    return 0;
}
