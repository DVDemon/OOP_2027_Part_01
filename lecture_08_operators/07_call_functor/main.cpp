// Пример 07: оператор вызова () — функторы (функциональные объекты).
#include <algorithm>
#include <iostream>
#include <vector>

// Функтор: хранит состояние (offset_) между вызовами.
class Adder {
    int offset_;
public:
    explicit Adder(int offset) : offset_(offset) {}
    int operator()(int x) const { return x + offset_; }
};

// Функтор-предикат: проверяет попадание в диапазон.
class InRange {
    double lo_;
    double hi_;
public:
    InRange(double lo, double hi) : lo_(lo), hi_(hi) {}
    bool operator()(double x) const { return x >= lo_ && x <= hi_; }
};

int main() {
    // Функтор как обычная функция, но с собственным состоянием.
    Adder add5(5);
    std::cout << "add5(10) = " << add5(10) << '\n';

    // Передаём функтор в алгоритм STL.
    std::vector<int> v = {1, 2, 3, 4, 5};
    std::transform(v.begin(), v.end(), v.begin(), Adder(10));
    std::cout << "после Adder(10): ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    // Предикат с состоянием (lo, hi) для count_if.
    std::vector<double> data = {1.5, 3.7, 2.1, 8.0, 4.2};
    auto count = std::count_if(data.begin(), data.end(), InRange(2.0, 5.0));
    std::cout << "значений в [2.0, 5.0]: " << count << '\n';

    return 0;
}
