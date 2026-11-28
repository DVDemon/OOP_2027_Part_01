// Пример 03: std::array — статический массив фиксированного размера.
//
// ПЛОХО (C-массив) — не знает своего размера, при передаче в функцию
// превращается в указатель, не копируется по значению, не сравнивается:
//
//     int a[5] = {5, 3, 8, 1, 4};
//     std::cout << sizeof(a) / sizeof(a[0]);   // только в той же функции
//     void f(int a[]);                          // внутри sizeof(a) == sizeof(int*)
//
// ХОРОШО — std::array<int, 5>: размер известен на этапе компиляции, элементы
// лежат прямо в объекте (локальный array — на стеке, без кучи), копируется и
// сравнивается как обычное значение, работает с алгоритмами STL.
//
// Важно: «на стеке» — это про локальную переменную. Если array объявить
// глобальным или static, он окажется в статической памяти, но всё равно не в куче.

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <string>
#include <tuple>

// Размер приходит как параметр шаблона — функция знает его без sizeof-трюков.
template <class T, std::size_t N>
void print(const std::array<T, N>& a, const std::string& name) {
    std::cout << name << " [N=" << N << "]: ";
    for (const auto& x : a)
        std::cout << x << ' ';
    std::cout << '\n';
}

int main() {
    std::cout << "=== 1. array живёт на стеке, кучи нет ===\n";
    std::array<int, 5> a{5, 3, 8, 1, 4};
    int local = 0;
    std::cout << "sizeof(a) = " << sizeof(a)
              << " == 5 * sizeof(int) = " << 5 * sizeof(int) << '\n';
    std::cout << "адрес объекта a   (стек): " << static_cast<const void*>(&a) << '\n';
    std::cout << "адрес a.data()    (стек): " << static_cast<const void*>(a.data()) << '\n';
    std::cout << "адрес local       (стек): " << static_cast<const void*>(&local) << '\n';
    std::cout << "рядом с local — элементы лежат в кадре стека, отдельной кучи нет\n";

    std::cout << "\n=== 2. Размер — часть типа, известен компилятору ===\n";
    static_assert(std::tuple_size_v<decltype(a)> == 5);
    constexpr std::array<int, 3> ca{1, 2, 3};
    static_assert(ca.size() == 3);
    static_assert(std::get<1>(ca) == 2);  // индекс известен на этапе компиляции
    print(a, "a");

    std::cout << "\n=== 3. Value-семантика: копия, сравнение ===\n";
    std::array<int, 5> b = a;  // полная копия, элементы скопированы
    b[0] = 100;
    std::cout << "a[0]=" << a[0] << ", b[0]=" << b[0] << " — копия независима\n";
    std::cout << "a == b? " << (a == b ? "да" : "нет") << '\n';

    std::cout << "\n=== 4. Алгоритмы STL работают как с vector ===\n";
    std::sort(a.begin(), a.end());
    print(a, "после sort");
    std::cout << "сумма: " << std::accumulate(a.begin(), a.end(), 0) << '\n';
    std::cout << "минимум: " << *std::min_element(a.begin(), a.end()) << '\n';

    std::cout << "\n=== 5. Доступ: [] без проверки, at() с исключением ===\n";
    std::cout << "a[0] = " << a[0] << '\n';
    try {
        std::cout << a.at(10) << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "a.at(10) бросило std::out_of_range\n";
    }

    std::cout << "\n=== 6. CTAD: размер выводится из инициализатора ===\n";
    std::array deduced{10, 20, 30};  // std::array<int, 3>
    static_assert(std::tuple_size_v<decltype(deduced)> == 3);
    print(deduced, "deduced");

    std::cout << "\n=== 7. Стек не резиновый ===\n";
    using Big = std::array<int, 1'000'000>;
    std::cout << "sizeof(std::array<int, 1'000'000>) = " << sizeof(Big)
              << " байт (~4 МБ): такой объект на стеке — риск переполнения\n";
    std::cout << "для больших массивов берут std::vector (данные в куче)\n";

    return 0;
}
