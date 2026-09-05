#pragma once

#include <iostream>

// Двумерный вектор. Сквозная сущность для демонстрации арифметики.
// Канонический подход: составной оператор (+=) — базовый, бинарный (+) —
// реализуется через него.
class Vector2D {
    double x_;
    double y_;
public:
    Vector2D(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }

    // --- Составные операторы: модифицируют *this, возвращают ссылку ---
    Vector2D& operator+=(const Vector2D& rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;  // ссылка нужна для цепочки a += b += c
    }

    Vector2D& operator-=(const Vector2D& rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    Vector2D& operator*=(double scalar) {
        x_ *= scalar;
        y_ *= scalar;
        return *this;
    }

    // --- Унарный минус ---
    Vector2D operator-() const { return Vector2D(-x_, -y_); }

    // --- Бинарные операторы через составные (свободные функции) ---
    // lhs принимаем ПО ЗНАЧЕНИЮ — это и есть будущий результат.
    friend Vector2D operator+(Vector2D lhs, const Vector2D& rhs) {
        lhs += rhs;
        return lhs;
    }

    friend Vector2D operator-(Vector2D lhs, const Vector2D& rhs) {
        lhs -= rhs;
        return lhs;
    }

    // Умножение на скаляр с обеих сторон (v * s и s * v).
    friend Vector2D operator*(Vector2D v, double s) {
        v *= s;
        return v;
    }

    friend Vector2D operator*(double s, Vector2D v) {
        v *= s;
        return v;
    }

    friend bool operator==(const Vector2D& a, const Vector2D& b) {
        return a.x_ == b.x_ && a.y_ == b.y_;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        return os << '(' << v.x_ << ", " << v.y_ << ')';
    }
};
