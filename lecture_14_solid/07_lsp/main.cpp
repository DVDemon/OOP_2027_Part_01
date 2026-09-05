// Пример 07: LSP — принцип подстановки Лисков.
//
// «Если S является подтипом T, то объекты T можно заменять объектами S
// без нарушения корректности программы».
//
// ПЛОХО — классический контрпример Square : public Rectangle:
//
//     class Rectangle {
//     protected: double width_, height_;
//     public:
//         virtual void set_width(double w)  { width_ = w; }
//         virtual void set_height(double h) { height_ = h; }
//         double area() const { return width_ * height_; }
//     };
//     class Square : public Rectangle {
//     public:
//         void set_width(double w)  override { width_ = height_ = w; } // !!
//         void set_height(double h) override { width_ = height_ = h; } // !!
//     };
//
//     void test(Rectangle& r) {     // код написан для Rectangle
//         r.set_width(5);
//         r.set_height(4);
//         assert(r.area() == 20);   // для Square == 16 -> подстановка ломает
//     }                             // программу => нарушение LSP
//
// ХОРОШО — Rectangle и Square независимы и неизменяемы (см. shapes.h).
// Любой Shape можно безопасно подставить в total_area().

#include <iostream>
#include <memory>
#include <vector>

#include "shapes.h"

// Клиентский код зависит от контракта Shape::area() и работает с любым
// подтипом одинаково корректно.
double total_area(const std::vector<std::unique_ptr<shapes::Shape>>& items) {
    double sum = 0.0;
    for (const auto& s : items) {
        sum += s->area();
    }
    return sum;
}

int main() {
    std::vector<std::unique_ptr<shapes::Shape>> items;
    items.push_back(std::make_unique<shapes::Rectangle>(5.0, 4.0));
    items.push_back(std::make_unique<shapes::Square>(3.0));

    std::cout << "Площадь прямоугольника 5x4: " << items[0]->area() << '\n';
    std::cout << "Площадь квадрата 3x3: " << items[1]->area() << '\n';
    std::cout << "Суммарная площадь: " << total_area(items) << '\n';

    return 0;
}
