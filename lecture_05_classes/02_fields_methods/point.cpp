#include "point.h"

#include <cmath>
#include <iostream>

namespace geom {

// Реализация вне класса: имя метода квалифицируем как Point::distanceTo.
// const в сигнатуре должен повторяться и здесь, и в заголовке.
double Point::distanceTo(const Point& other) const {
    const double dx = x_ - other.x_;
    const double dy = y_ - other.y_;
    return std::sqrt(dx * dx + dy * dy);
}

void Point::print() const {
    std::cout << "(" << x_ << ", " << y_ << ")";
}

}  // namespace geom
