// Пример 06: лямбда-выражения — захват, замыкание, внутреннее устройство.
//
// ПЛОХО (передача состояния через глобальную переменную или отдельный функтор):
// глобальная переменная создаёт скрытую связь и мешает повторному использованию,
// а функтор ради одной строки приходится объявлять в отдельном месте:
//
//     int g_threshold = 5;
//     bool above_global(int x) { return x > g_threshold; }
//
// ХОРОШО — лямбда определяет функцию и её состояние прямо в месте вызова:
//
//     int threshold = 5;
//     auto above = [threshold](int x) { return x > threshold; };
//
// Внутри лямбда — это замыкание: безымянный класс с полями-захватами и
// operator(). Захват по значению копирует переменную в поле, по ссылке — хранит
// ссылку. Отсюда правила: [=] безопаснее по времени жизни, [&] — быстрее, но
// ссылка не должна пережить переменную.

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

// Функтор — обычный класс с operator(). Лямбда компилируется примерно в такой же.
struct IsEven {
    bool operator()(int x) const {
        return x % 2 == 0;
    }
};

int main() {
    std::cout << "=== 1. Синтаксис: [захват](параметры) { тело } ===\n";
    auto is_even = [](int x) { return x % 2 == 0; };
    std::cout << "is_even(4)=" << is_even(4) << ", is_even(5)=" << is_even(5) << '\n';

    std::cout << "\n=== 2. Захват по значению: копия на момент создания ===\n";
    int threshold = 5;
    auto above = [threshold](int x) { return x > threshold; };
    threshold = 100;  // лямбда хранит копию 5, а не ссылку
    std::cout << "threshold изменён на 100, above(6)=" << above(6)
              << " (копия 5 внутри замыкания)\n";

    std::cout << "\n=== 3. Захват по ссылке: видно изменения ===\n";
    int calls = 0;
    auto counted = [&calls](int x) {
        ++calls;
        return x * 2;
    };
    counted(1);
    counted(2);
    std::cout << "counted вызвана " << calls << " раза\n";

    std::cout << "\n=== 4. mutable: состояние замыкания можно менять ===\n";
    auto counter = [n = 0]() mutable { return ++n; };
    std::cout << "counter(): " << counter() << ", " << counter() << '\n';
    auto copy = counter;  // копия замыкания копирует и состояние
    std::cout << "copy(): " << copy() << " — копия начала с n=2\n";

    std::cout << "\n=== 5. Смешанный захват [=, &b] ===\n";
    int a = 1, b = 2;
    auto mix = [=, &b]() { b += a; };  // a — копия, b — ссылка
    mix();
    std::cout << "после mix(): a=" << a << ", b=" << b << '\n';

    std::cout << "\n=== 6. Размер замыкания = размер захваченных полей ===\n";
    auto no_capture = [](int x) { return x; };
    std::cout << "sizeof(без захвата) = " << sizeof(no_capture) << '\n';
    std::cout << "sizeof([threshold]) = " << sizeof(above) << '\n';
    std::cout << "sizeof([n=0])       = " << sizeof(counter) << '\n';

    std::cout << "\n=== 7. Лямбда и функтор для алгоритма эквивалентны ===\n";
    std::vector<int> v{1, 4, 6, 3, 8, 2};
    std::cout << "count_if(IsEven{}) = "
              << std::count_if(v.begin(), v.end(), IsEven{}) << '\n';
    std::cout << "count_if(лямбда)   = "
              << std::count_if(v.begin(), v.end(), is_even) << '\n';
    std::cout << "имя типа лямбды: " << typeid(is_even).name()
              << " — компилятор сгенерировал безымянный класс\n";

    std::cout << "\n=== 8. Лямбда-компаратор прямо в sort ===\n";
    std::sort(v.begin(), v.end(), [](int x, int y) { return x > y; });
    std::cout << "по убыванию: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "\n=== 9. constexpr-лямбда и немедленный вызов ===\n";
    constexpr auto square = [](int x) constexpr { return x * x; };
    static_assert(square(5) == 25);
    std::cout << "square(5)=" << square(5)
              << ", IIFE: " << [] { return 42; }() << '\n';

    std::cout << "\n=== 10. Опасность: захват по ссылке живёт дольше объекта ===\n";
    std::cout << "если вернуть из функции [&local] — ссылка останется на мёртвую\n"
                 "переменную; безопасно вернуть [local] (копию) или [x = local]\n";

    return 0;
}
