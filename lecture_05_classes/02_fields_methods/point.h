#pragma once

namespace geom {

// Точка на плоскости. Демонстрирует два способа определять методы:
// 1) прямо в теле класса (неявно inline);
// 2) только объявление в .h, а реализация — в point.cpp.
class Point {
    double x_;
    double y_;

public:
    Point(double x, double y) : x_(x), y_(y) {}

    // --- Определение ВНУТРИ класса (неявно inline) ---
    // Короткие тривиальные геттеры удобно держать в заголовке.
    double getX() const { return x_; }
    double getY() const { return y_; }

    // --- Только ОБЪЯВЛЕНИЕ; реализация в point.cpp ---
    // Нетривиальную логику выносим в .cpp: меньше зависимостей в заголовке
    // и быстрее перекомпиляция при изменении реализации.
    double distanceTo(const Point& other) const;
    void print() const;
};

}  // namespace geom
