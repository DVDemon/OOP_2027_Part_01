// Пример 07: шаблонные (generic) лямбды и функции, принимающие лямбду.
//
// ПЛОХО (дублировать лямбду под каждый тип) — одна и та же логика копируется
// для int, double, string; при добавлении типа нужно не забыть ещё одну копию:
//
//     auto twice_int    = [](int x)    { return x + x; };
//     auto twice_double = [](double x) { return x + x; };
//     auto twice_string = [](std::string x) { return x + x; };
//
// ХОРОШО — generic-лямбда с параметром auto: компилятор генерирует отдельный
// шаблонный operator() для каждого типа. Алгоритмы STL — тоже шаблонные
// функции, поэтому лямбда подставляется в них как обычный тип-параметр.

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

// Шаблонная функция принимает любой callable: лямбду, функтор, указатель.
template <class F>
auto apply_twice(F f, int x) {
    return f(f(x));
}

// Шаблонная функция + лямбда-преобразование над контейнером.
template <class T, class F>
std::vector<T> transformed(const std::vector<T>& src, F f) {
    std::vector<T> out;
    out.reserve(src.size());
    std::transform(src.begin(), src.end(), std::back_inserter(out), f);
    return out;
}

int main() {
    std::cout << "=== 1. Generic-лямбда: один код — много типов ===\n";
    auto twice = [](auto x) { return x + x; };
    std::cout << "twice(21)          = " << twice(21) << '\n';
    std::cout << "twice(1.5)         = " << twice(1.5) << '\n';
    std::cout << "twice(string(\"ab\")) = " << twice(std::string("ab")) << '\n';
    std::cout << "для каждого типа инстанцируется свой operator()<T>\n";

    std::cout << "\n=== 2. C++20: явный список параметров шаблона ===\n";
    auto same = []<class T>(const T& a, const T& b) { return a == b; };
    std::cout << "same(1, 1)=" << same(1, 1)
              << ", same(1, 2)=" << same(1, 2) << '\n';

    std::cout << "\n=== 3. Generic-компаратор для sort ===\n";
    std::vector<std::string> words{"яблоко", "груша", "слива", "абрикос"};
    std::sort(words.begin(), words.end(),
              [](const auto& x, const auto& y) { return x.size() < y.size(); });
    for (const auto& w : words)
        std::cout << w << ' ';
    std::cout << '\n';

    std::cout << "\n=== 4. Шаблонная функция принимает лямбду ===\n";
    std::cout << "apply_twice(+10, 1) = "
              << apply_twice([](int x) { return x + 10; }, 1) << '\n';
    std::cout << "apply_twice(twice, 3) = " << apply_twice(twice, 3) << '\n';

    std::cout << "\n=== 5. Лямбда как преобразование контейнера ===\n";
    std::vector<int> nums{1, 2, 3};
    auto squares = transformed(nums, [](int x) { return x * x; });
    std::cout << "квадраты: ";
    for (int x : squares) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "\n=== 6. std::function: стирание типа ===\n";
    std::vector<std::function<int(int)>> pipeline;
    pipeline.push_back([](int x) { return x + 1; });
    pipeline.push_back([](int x) { return x * 2; });
    int value = 5;
    for (const auto& step : pipeline)
        value = step(value);
    std::cout << "5 -> (+1) -> (*2) = " << value << '\n';

    std::cout << "\n=== 7. Цена std::function ===\n";
    std::cout << "sizeof(lambda)        = " << sizeof(twice) << '\n';
    std::cout << "sizeof(std::function) = " << sizeof(std::function<int(int)>) << '\n';
    std::cout << "std::function хранит callable в обёртке со стиранием типа —\n"
                 "для горячего кода предпочтительнее шаблон/auto, для хранения — function\n";

    std::cout << "\n=== 8. std::function хранит и захваченное состояние ===\n";
    int base = 100;
    std::function<int(int)> add_base = [base](int x) { return x + base; };
    std::cout << "add_base(1) = " << add_base(1) << '\n';

    return 0;
}
