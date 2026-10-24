// Пример 10: практические рекомендации — иерархия фигур.
#include <iostream>
#include <memory>
#include <vector>

#include "shapes.h"

int main() {
    using namespace shapes;

    // Рекомендация: храните полиморфные объекты через умные указатели,
    // а не по значению — иначе срезка.
    std::vector<std::unique_ptr<Shape>> figures;
    figures.push_back(std::make_unique<Rectangle>(0.0, 0.0, 4.0, 3.0));
    figures.push_back(std::make_unique<Square>(1.0, 1.0, 5.0));

    std::cout << "-- полиморфный обход (без срезки) --\n";
    for (const auto& f : figures) {
        std::cout << f->info() << '\n';  // вызывается нужный name()/area()
    }

    std::cout << "-- срезка при копировании в базу (антипример) --\n";
    Rectangle r{0.0, 0.0, 4.0, 3.0};
    // Shape базовый абстрактный, поэтому "срезать" можно в Rectangle:
    Square sq{0.0, 0.0, 5.0};
    Rectangle sliced = sq;  // СРЕЗКА: name() станет Rectangle, сторона потеряет смысл
    std::cout << "исходный квадрат: " << sq.name()
              << ", после срезки: " << sliced.name() << '\n';

    std::cout << "-- ссылка сохраняет полиморфизм --\n";
    const Shape& ref = r;
    std::cout << ref.info() << '\n';

    return 0;
}
