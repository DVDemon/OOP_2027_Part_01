// Пример 09: умные указатели и полиморфизм.
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Shape {
public:
    virtual double area() const = 0;
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
    void draw() const override { std::cout << "Circle(r=" << r_ << ")\n"; }
private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    void draw() const override { std::cout << "Rect(" << w_ << "x" << h_ << ")\n"; }
private:
    double w_, h_;
};

// Фабрика возвращает базовый тип через unique_ptr — владение у вызывающего.
std::unique_ptr<Shape> createShape(const std::string& type) {
    if (type == "circle") return std::make_unique<Circle>(5.0);
    if (type == "rect")   return std::make_unique<Rect>(3.0, 4.0);
    throw std::invalid_argument("неизвестная фигура: " + type);
}

int main() {
    // unique_ptr и полиморфизм: коллекция разнородных фигур.
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(5.0));
    shapes.push_back(std::make_unique<Rect>(3.0, 4.0));
    shapes.push_back(createShape("circle"));  // через фабрику

    for (const auto& s : shapes) {
        s->draw();
        std::cout << "  area = " << s->area() << '\n';
    }

    // shared_ptr и полиморфизм + dynamic_pointer_cast.
    std::shared_ptr<Shape> shape = std::make_shared<Circle>(10.0);
    if (auto circle = std::dynamic_pointer_cast<Circle>(shape)) {
        std::cout << "Это круг! area = " << circle->area() << '\n';
    }

    return 0;
}
