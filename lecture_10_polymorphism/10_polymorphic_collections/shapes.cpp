#include "shapes.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace draw {

namespace {
constexpr double kPi = 3.14159265358979323846;
}

double Circle::area() const { return kPi * r_ * r_; }
std::string Circle::name() const { return "Circle"; }

double Rectangle::area() const { return w_ * h_; }
std::string Rectangle::name() const { return "Rectangle"; }

std::unique_ptr<Shape> createShape(const std::string& type, double a, double b) {
    if (type == "circle") {
        return std::make_unique<Circle>(a);
    }
    if (type == "rectangle") {
        return std::make_unique<Rectangle>(a, b);
    }
    throw std::invalid_argument("Неизвестный тип фигуры: " + type);
}

void Canvas::add(std::unique_ptr<Shape> s) {
    shapes_.push_back(std::move(s));
}

double Canvas::totalArea() const {
    double sum = 0.0;
    for (const auto& s : shapes_) {
        sum += s->area();  // виртуальный вызов
    }
    return sum;
}

void BubbleSort::sort(std::vector<int>& data) const {
    for (std::size_t i = 0; i < data.size(); ++i) {
        for (std::size_t j = 0; j + 1 + i < data.size(); ++j) {
            if (data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
            }
        }
    }
}

void StdSort::sort(std::vector<int>& data) const {
    std::sort(data.begin(), data.end());
}

std::string Sorter::strategyName() const {
    return strategy_ ? strategy_->name() : "<none>";
}

void Sorter::run(std::vector<int>& data) const {
    if (strategy_) {
        strategy_->sort(data);
    }
}

}  // namespace draw
