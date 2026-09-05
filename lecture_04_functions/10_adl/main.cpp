// Пример 10: ADL (Argument-Dependent Lookup), он же поиск Кёнига.
// Компилятор ищет функцию не только в текущей области видимости,
// но и в пространствах имён ТИПОВ аргументов.
#include <iostream>
#include <utility>  // std::swap

namespace geometry {

struct Point {
    double x, y;
};

// Функция в том же namespace, что и Point.
void print(const Point& p) {
    std::cout << "(" << p.x << ", " << p.y << ")";
}

// operator<< в том же namespace, что и Point.
std::ostream& operator<<(std::ostream& os, const Point& p) {
    return os << "(" << p.x << ", " << p.y << ")";
}

}  // namespace geometry

namespace my {

struct Widget {
    int id;
};

// Специализированный swap для Widget — должен быть найден через ADL.
void swap(Widget& a, Widget& b) {
    std::cout << "  my::swap для Widget\n";
    std::swap(a.id, b.id);
}

}  // namespace my

// Идиома "using std::swap; swap(a, b);": ADL найдёт my::swap для my::Widget.
template <typename T>
void swap_two(T& a, T& b) {
    using std::swap;  // делаем std::swap кандидатом
    swap(a, b);       // ADL предпочтёт my::swap, если он есть
}

int main() {
    geometry::Point p{3.0, 4.0};

    // ADL находит geometry::print, хотя мы не писали geometry::
    std::cout << "print(p) -> ";
    print(p);
    std::cout << '\n';

    // ADL находит geometry::operator<< — поэтому это просто работает.
    std::cout << "cout << p -> " << p << '\n';

    // Идиома swap: ADL выбирает специализированный my::swap.
    my::Widget w1{1}, w2{2};
    std::cout << "swap_two(Widget): ";
    swap_two(w1, w2);
    std::cout << "  после: " << w1.id << ", " << w2.id << '\n';  // 2, 1

    return 0;
}
