// Пример 10: полиморфные коллекции + паттерны Factory Method и Strategy.
#include <iostream>
#include <memory>
#include <vector>

#include "shapes.h"

int main() {
    // --- Factory Method: создаём фигуры, не зная конкретных классов ---
    std::cout << "Factory Method + полиморфная коллекция:\n";
    draw::Canvas canvas;
    canvas.add(draw::createShape("circle", 5.0));
    canvas.add(draw::createShape("rectangle", 3.0, 4.0));
    canvas.add(draw::createShape("circle", 1.0));

    std::cout << "  фигур в коллекции: " << canvas.count() << '\n';
    std::cout << "  суммарная площадь: " << canvas.totalArea() << '\n';

    // --- Strategy: выбираем алгоритм сортировки во время выполнения ---
    std::cout << "\nStrategy (выбор алгоритма в runtime):\n";
    std::vector<int> data{5, 2, 8, 1, 9, 3};

    draw::Sorter sorter;
    sorter.setStrategy(std::make_unique<draw::BubbleSort>());
    std::cout << "  стратегия: " << sorter.strategyName() << '\n';
    {
        std::vector<int> copy = data;
        sorter.run(copy);
        std::cout << "  результат:";
        for (int x : copy) std::cout << ' ' << x;
        std::cout << '\n';
    }

    // Меняем стратегию на лету.
    sorter.setStrategy(std::make_unique<draw::StdSort>());
    std::cout << "  стратегия: " << sorter.strategyName() << '\n';
    {
        std::vector<int> copy = data;
        sorter.run(copy);
        std::cout << "  результат:";
        for (int x : copy) std::cout << ' ' << x;
        std::cout << '\n';
    }

    return 0;
}
