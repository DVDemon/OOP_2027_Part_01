// Пример 05: чисто виртуальные функции, абстрактные классы, интерфейсы.
#include <iostream>

#include "shapes.h"

// Функция работает с любым Shape через ссылку на абстрактный базовый класс.
void describe(const shapes::Shape& s) {
    std::cout << s.name() << ": площадь = " << s.area()
              << ", периметр = " << s.perimeter() << '\n';
}

// Функция работает с любым объектом, реализующим интерфейс ISerializable.
void store(const shapes::ISerializable& obj) {
    std::cout << "  сериализовано: " << obj.serialize() << '\n';
}

int main() {
    // shapes::Shape s;  // ОШИБКА: нельзя создать объект абстрактного класса

    shapes::Circle c{2.0};
    shapes::Rectangle r{3.0, 4.0};

    std::cout << "Полиморфизм через абстрактный класс Shape:\n";
    describe(c);
    describe(r);

    std::cout << "\nПолиморфизм через интерфейс ISerializable:\n";
    store(c);
    store(r);

    return 0;
}
