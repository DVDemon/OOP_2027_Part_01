// Пример 07: дружественные функции, операторы и классы (friend).
#include <iostream>
#include <string>

// Объявление вперёд: оператор и функция должны быть видимы при объявлении friend.
class Vector2D;

class Vector2D {
    double x_, y_;  // private

public:
    Vector2D(double x, double y) : x_(x), y_(y) {}

    // Дружественная свободная функция: получит доступ к private x_, y_.
    friend double dot(const Vector2D& a, const Vector2D& b);

    // Дружественный оператор вывода (определён прямо здесь — он inline).
    // Свободная функция: левый операнд — поток, поэтому методом её не сделать.
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        return os << "(" << v.x_ << ", " << v.y_ << ")";  // доступ к private
    }

    // Дружественный класс: все его методы видят private членов Vector2D.
    friend class VectorSerializer;
};

double dot(const Vector2D& a, const Vector2D& b) {
    return a.x_ * b.x_ + a.y_ * b.y_;  // доступ к private обоих объектов
}

class VectorSerializer {
public:
    static std::string toJson(const Vector2D& v) {
        return "{\"x\":" + std::to_string(v.x_) +
               ",\"y\":" + std::to_string(v.y_) + "}";  // доступ к private
    }
};

int main() {
    Vector2D a{1.0, 2.0};
    Vector2D b{3.0, 4.0};

    std::cout << "a = " << a << ", b = " << b << '\n';     // friend operator<<
    std::cout << "dot(a, b) = " << dot(a, b) << '\n';      // friend-функция: 1*3 + 2*4 = 11
    std::cout << "json(a) = " << VectorSerializer::toJson(a) << '\n';  // friend-класс

    return 0;
}
