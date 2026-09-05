// Пример 02: поля и методы; определение внутри класса и вынос в .cpp.
#include <iostream>

#include "point.h"

int main() {
    geom::Point a{0.0, 0.0};
    geom::Point b{3.0, 4.0};

    std::cout << "a = ";
    a.print();
    std::cout << ", b = ";
    b.print();
    std::cout << '\n';

    // getX/getY определены в заголовке (inline), distanceTo — в point.cpp.
    std::cout << "a.getX() = " << a.getX() << ", a.getY() = " << a.getY() << '\n';
    std::cout << "distance(a, b) = " << a.distanceTo(b) << '\n';  // ожидаем 5

    return 0;
}
