// Пример 10: ADL (Argument-Dependent Lookup), он же поиск Кёнига.
// Компилятор ищет функцию не только в текущей области видимости,
// но и в пространствах имён ТИПОВ аргументов.
#include <iostream>

// --- 1. Тип объявлен в namespace — функция ищется там же --------------------

namespace geometry {

// Перечисление объявлено внутри geometry, поэтому его namespace — geometry.
enum Shape { Circle, Square, Triangle };

// Функция в том же namespace, что и Shape.
const char* describe(Shape s) {
    switch (s) {
        case Circle:   return "круг";
        case Square:   return "квадрат";
        case Triangle: return "треугольник";
    }
    return "неизвестно";
}

int sides(Shape s) {
    switch (s) {
        case Circle:   return 0;
        case Square:   return 4;
        case Triangle: return 3;
    }
    return -1;
}

}  // namespace geometry

// --- 2. Одноимённые функции в разных namespace ------------------------------

// Есть ещё один describe — и он не конфликтует с geometry::describe:
// ADL выбирает функцию по namespace ТИПА аргумента.
namespace physics {

enum Unit { Meter, Second };

const char* describe(Unit u) {
    return u == Meter ? "метр" : "секунда";
}

}  // namespace physics


const char* describe(physics::Unit u) {
    return u == physics::Meter ? "сам ты метр" : "и секунда";
}


int main() {
    // ADL находит geometry::describe, хотя мы не написали geometry::
    std::cout << "describe(Circle)       -> " << describe(geometry::Circle) << '\n';
    std::cout << "sides(Square)          -> " << sides(geometry::Square) << '\n';

    // Имя describe одно и то же, но ADL выбирает функцию по типу аргумента.
    std::cout << "describe(Meter)        -> " << physics::describe(physics::Meter) << '\n';


    return 0;
}
