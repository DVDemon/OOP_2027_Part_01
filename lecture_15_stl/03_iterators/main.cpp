// Пример 03: итераторы — единый способ обхода любого контейнера.
//
// ПЛОХО (обход по индексу) — для std::vector ещё работает, но привязан к его
// устройству: недоступен для map/set, легко ошибиться в границах цикла:
//
//     for (int i = 0; i < v.size(); ++i) use(v[i]);   // vector-only
//
// ХОРОШО — begin()/end()/++/* дают один и тот же код обхода для vector, map,
// set и любого другого контейнера. range-for — просто более короткая запись
// итераторного цикла.

#include <cstddef>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

template <typename Container>
void dump(const Container& c) {
    // Один шаблон обходит любой контейнер, у которого есть begin()/end().
    for (auto it = c.begin(); it != c.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';
}

int main() {
    std::cout << "=== итераторный цикл по vector ===\n";
    std::vector<int> v{5, 3, 8, 1};
    for (auto it = v.begin(); it != v.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    std::cout << "=== range-for: сахар над итераторным циклом ===\n";
    for (const auto& x : v)          // то же самое, что цикл выше
        std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "=== изменение элементов через ссылку ===\n";
    for (auto& x : v)                // без & копии бы не изменили исходные данные
        x *= 10;
    dump(v);

    std::cout << "=== один и тот же шаблон обходит map ===\n";
    std::map<std::string, int> scores{{"Аня", 88}, {"Борис", 95}};
    for (auto it = scores.begin(); it != scores.end(); ++it)
        std::cout << it->first << " -> " << it->second << '\n';

    std::cout << "=== и set ===\n";
    std::set<int> s{3, 1, 2};
    dump(s);

    std::cout << "=== полуоткрытый интервал [begin, end) ===\n";
    // end() указывает «за последний» элемент: у пустого контейнера begin == end,
    // поэтому цикл for(it = begin; it != end; ++it) безопасен даже для пустого.
    std::vector<int> empty;
    std::cout << "для пустого вектора begin == end? "
              << (empty.begin() == empty.end() ? "да" : "нет") << '\n';
    // Длина интервала [begin, end) в точности равна size() (для random-access
    // итераторов это выражается вычитанием итераторов).
    std::cout << "end() - begin() == size()? "
              << (static_cast<std::size_t>(v.end() - v.begin()) == v.size()
                      ? "да" : "нет") << '\n';

    return 0;
}
