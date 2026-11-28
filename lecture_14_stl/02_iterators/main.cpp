// Пример 02: итераторы — единый интерфейс доступа к любому контейнеру.
//
// ПЛОХО (обход «по внутренностям» контейнера) — цикл по индексу работает только
// для vector/array; для list его уже не написать, а границы легко перепутать:
//
//     for (std::size_t i = 0; i < v.size(); ++i) use(v[i]);   // только vector
//
// ХОРОШО — пара итераторов [begin, end) даёт один и тот же код обхода для
// vector, array, list и map. Категория итератора определяет, что контейнер
// умеет: vector/array — произвольный доступ (random access), list — только
// шаги вперёд-назад (bidirectional). Отсюда и ограничения алгоритмов:
// std::sort требует random access, поэтому для list есть member-функция sort().

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <type_traits>
#include <vector>

// Категория итератора известна на этапе компиляции — печатаем её через if constexpr.
template <class It>
constexpr const char* category_name() {
    using cat = typename std::iterator_traits<It>::iterator_category;
    if constexpr (std::is_same_v<cat, std::random_access_iterator_tag>)
        return "random access";
    else if constexpr (std::is_same_v<cat, std::bidirectional_iterator_tag>)
        return "bidirectional";
    else if constexpr (std::is_same_v<cat, std::forward_iterator_tag>)
        return "forward";
    else
        return "input";
}

// Один и тот же шаблон обходит любой контейнер.
template <class Container>
void dump(const Container& c, const std::string& name) {
    std::cout << name << " (" << category_name<decltype(c.begin())>() << "): ";
    for (auto it = c.begin(); it != c.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';
}

int main() {
    std::cout << "=== 1. Классический итераторный цикл ===\n";
    std::vector<int> v{5, 3, 8, 1};
    for (auto it = v.begin(); it != v.end(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    std::cout << "\n=== 2. range-for — синтаксический сахар над ним ===\n";
    for (const auto& x : v)
        std::cout << x << ' ';
    std::cout << '\n';
    std::cout << "for (const auto& x : v)  ==  for (auto it = v.begin(); it != v.end(); ++it)\n";

    std::cout << "\n=== 3. Полуоткрытый интервал [begin, end) ===\n";
    std::vector<int> empty;
    std::cout << "пустой контейнер: begin() == end()? "
              << (empty.begin() == empty.end() ? "да" : "нет") << '\n';
    std::cout << "end() - begin() == size()? "
              << (static_cast<std::size_t>(v.end() - v.begin()) == v.size()
                      ? "да" : "нет")
              << '\n';

    std::cout << "\n=== 4. Категории итераторов ===\n";
    std::array<int, 3> a{1, 2, 3};
    std::list<int> l{3, 1, 2};
    dump(v, "vector");
    dump(a, "array ");
    dump(l, "list  ");

    std::cout << "\n=== 5. Цена категории: distance и advance ===\n";
    auto v_it = v.begin();
    auto l_it = l.begin();
    std::cout << "vector: distance(begin, end) = " << std::distance(v.begin(), v.end())
              << " — O(1), итераторы можно вычитать\n";
    std::cout << "list:   distance(begin, end) = " << std::distance(l.begin(), l.end())
              << " — O(n), шагов по узлам\n";

    std::advance(v_it, 2);  // random access: сразу
    std::advance(l_it, 2);  // bidirectional: два шага
    std::cout << "третий элемент: vector=" << *v_it << ", list=" << *l_it << '\n';

    std::cout << "\n=== 6. Алгоритм требует категорию ===\n";
    std::sort(v.begin(), v.end());
    std::cout << "std::sort(vector) — можно: random access\n";
    // std::sort(l.begin(), l.end());   // ошибка компиляции: list — bidirectional
    l.sort();  // у list своя сортировка — перестановка узлов
    std::cout << "std::sort(list) — нельзя, у list есть member sort(): ";
    for (int x : l) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "\n=== 7. Реверсивные итераторы ===\n";
    std::vector<int> r{1, 2, 3};
    for (auto it = r.rbegin(); it != r.rend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    std::cout << "\n=== 8. Что инвалидирует итераторы ===\n";
    std::cout << "array : ничего — размер фиксирован\n";
    std::cout << "vector: реаллокация (push_back сверх capacity) и insert/erase —\n";
    std::cout << "        все итераторы и указатели на старый буфер\n";
    std::cout << "list  : только итератор/указатель на удалённый элемент\n";

    std::cout << "\n=== 9. const_iterator: обход без изменения ===\n";
    for (auto it = r.cbegin(); it != r.cend(); ++it)
        std::cout << *it << ' ';
    std::cout << '\n';

    return 0;
}
