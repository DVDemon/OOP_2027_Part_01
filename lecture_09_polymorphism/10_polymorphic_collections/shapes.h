#pragma once

#include <memory>
#include <string>
#include <vector>

namespace draw {

// --- Полиморфная иерархия фигур ---
class Shape {
public:
    virtual double area() const = 0;
    virtual std::string name() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override;
    std::string name() const override;
};

class Rectangle : public Shape {
    double w_;
    double h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override;
    std::string name() const override;
};

// --- Паттерн Factory Method ---
// Инкапсулирует создание объектов: клиент не знает конкретные классы.
std::unique_ptr<Shape> createShape(const std::string& type, double a, double b = 0.0);

// --- Полиморфная коллекция ---
// Хранит фигуры через unique_ptr<Shape> — без срезки, с владением.
class Canvas {
    std::vector<std::unique_ptr<Shape>> shapes_;
public:
    void add(std::unique_ptr<Shape> s);
    double totalArea() const;
    std::size_t count() const { return shapes_.size(); }
};

// --- Паттерн Strategy ---
// Выбор алгоритма во время выполнения через интерфейс.
class SortStrategy {
public:
    virtual void sort(std::vector<int>& data) const = 0;
    virtual std::string name() const = 0;
    virtual ~SortStrategy() = default;
};

class BubbleSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override;
    std::string name() const override { return "BubbleSort"; }
};

class StdSort : public SortStrategy {
public:
    void sort(std::vector<int>& data) const override;
    std::string name() const override { return "StdSort"; }
};

// Контекст, использующий стратегию (можно менять алгоритм в runtime).
class Sorter {
    std::unique_ptr<SortStrategy> strategy_;
public:
    void setStrategy(std::unique_ptr<SortStrategy> s) { strategy_ = std::move(s); }
    std::string strategyName() const;
    void run(std::vector<int>& data) const;
};

}  // namespace draw
