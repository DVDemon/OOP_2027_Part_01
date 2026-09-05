// Пример 04: базовые алгоритмы STL и лямбды.
//
// ПЛОХО (ручной цикл для типовой операции) — «подсчитать чётные» в пять строк:
//
//     long long even = 0;
//     for (std::size_t i = 0; i < v.size(); ++i)
//         if (v[i] % 2 == 0)
//             ++even;
//
// ХОРОШО — готовый алгоритм std::count_if с лямбда-предикатом: одна строка,
// намерение читается из имени алгоритма. Здесь же sort, accumulate, for_each.

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main() {
    std::vector<int> v{5, 3, 8, 1, 4, 6, 2, 9, 7, 10};

    std::cout << "=== std::sort ===\n";
    std::sort(v.begin(), v.end());                 // по возрастанию
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    std::sort(v.rbegin(), v.rend());               // по убыванию, без обращения
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "\n=== std::count_if с лямбда-предикатом ===\n";
    long long even = std::count_if(v.begin(), v.end(),
                                   [](int x) { return x % 2 == 0; });
    std::cout << "чётных: " << even << '\n';

    int threshold = 5;
    long long above = std::count_if(v.begin(), v.end(),
                                    [threshold](int x) { return x > threshold; });
    std::cout << "больше " << threshold << ": " << above << '\n';

    std::cout << "\n=== std::accumulate: сумма, среднее, произведение ===\n";
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "сумма: " << sum << '\n';
    std::cout << "среднее: " << static_cast<double>(sum) / v.size() << '\n';
    int prod = std::accumulate(v.begin(), v.end(), 1,
                               [](int a, int b) { return a * b; });
    std::cout << "произведение: " << prod << '\n';

    std::cout << "\n=== std::for_each: действие над каждым элементом ===\n";
    std::for_each(v.begin(), v.end(), [](int& x) { x *= 2; });  // модифицируем
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "\n=== лямбда-компаратор: сортировка строк по длине ===\n";
    std::vector<std::string> words{"яблоко", "груша", "слива", "абрикос"};
    std::sort(words.begin(), words.end(),
              [](const std::string& a, const std::string& b) {
                  return a.size() < b.size();
              });
    for (const auto& w : words) std::cout << w << ' ';
    std::cout << '\n';

    return 0;
}
