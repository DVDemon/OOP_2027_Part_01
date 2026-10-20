// Пример 01: std::vector — динамический массив.
//
// ПЛОХО (избыточные перераспределения) — добавляем много элементов без reserve,
// вектор каждый раз при достижении capacity копирует весь буфер в новый:
//
//     std::vector<int> v;
//     for (int i = 0; i < 1'000'000; ++i)
//         v.push_back(i);   // ~20 перераспределений всего буфера
//
// ХОРОШО — заранее резервируем память (size() остаётся 0, но capacity уже есть),
// вставки не перераспределяют буфер. Так же показываем size vs capacity,
// push_back vs emplace_back и доступ по индексу за O(1).

#include <iostream>
#include <vector>

struct Point {
    int x, y;
    explicit Point(int a = 0, int b = 0) : x(a), y(b) {}
};

int main() {
    std::cout << "=== size vs capacity ===\n";
    std::vector<int> v;
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
        std::cout << "size=" << v.size() << "  capacity=" << v.capacity() << '\n';
    }

    std::cout << "\n=== доступ по индексу: O(1), как у массива ===\n";
    std::cout << "v[0]=" << v[0] << "  v[9]=" << v[9] << '\n';

    std::cout << "\n=== reserve: заполняем заранее выделенную память ===\n";
    std::vector<int> w;
    w.reserve(1'000'000);
    for (int i = 0; i < 1'000'000; ++i)
        w.push_back(i);
    std::cout << "size=" << w.size() << "  capacity=" << w.capacity()
              << "  (capacity не превысил зарезервированного значения)\n";

    std::cout << "\n=== push_back vs emplace_back ===\n";
    std::vector<Point> pts;
    pts.push_back(Point{1, 2});      // временный объект создан, потом скопирован
    pts.emplace_back(3, 4);          // Point{3,4} собран сразу внутри вектора
    for (const auto& p : pts)
        std::cout << "(" << p.x << ", " << p.y << ")\n";

    std::cout << "\n=== shrink_to_fit: capacity подстраивается под size ===\n";
    v.shrink_to_fit();
    std::cout << "size=" << v.size() << "  capacity=" << v.capacity() << '\n';

    return 0;
}
