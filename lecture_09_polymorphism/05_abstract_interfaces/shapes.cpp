#include "shapes.h"

#include <string>

namespace shapes {

namespace {
constexpr double kPi = 3.14159265358979323846;
}

double Circle::area() const { return kPi * r_ * r_; }
double Circle::perimeter() const { return 2.0 * kPi * r_; }
std::string Circle::name() const { return "Circle"; }
std::string Circle::serialize() const {
    return "Circle:" + std::to_string(r_);
}

double Rectangle::area() const { return w_ * h_; }
double Rectangle::perimeter() const { return 2.0 * (w_ + h_); }
std::string Rectangle::name() const { return "Rectangle"; }
std::string Rectangle::serialize() const {
    return "Rectangle:" + std::to_string(w_) + "x" + std::to_string(h_);
}

}  // namespace shapes
