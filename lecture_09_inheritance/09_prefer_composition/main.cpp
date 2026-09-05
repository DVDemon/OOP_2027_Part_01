// Пример 09: принцип "предпочитайте композицию наследованию".
#include <iostream>

#include "car.h"

int main() {
    vehicle::Car car{"Тойота", 150};

    std::cout << car.start() << '\n';
    std::cout << "модель: " << car.model()
              << ", мощность: " << car.horsepower() << " л.с.\n";

    // Машина "содержит" двигатель (has-a), а не "является" двигателем (is-a),
    // поэтому наследовать Car от Engine было бы семантически неверно.
    return 0;
}
