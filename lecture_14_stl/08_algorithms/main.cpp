// Пример 08: стандартные алгоритмы STL с лямбдами.
//
// ПЛОХО (ручной цикл под каждую типовую операцию) — индекс, условие, счётчик,
// накопление; на каждую задачу свой цикл, и в каждом легко ошибиться:
//
//     long long even = 0;
//     for (std::size_t i = 0; i < v.size(); ++i)
//         if (v[i] % 2 == 0) ++even;
//
// ХОРОШО — готовый алгоритм + лямбда: имя алгоритма называет намерение,
// лямбда — только условие или операцию. Алгоритм принимает пару итераторов
// [first, last) и работает с любым контейнером с нужной категорией.

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int score = 0;
};

int grade(int score) {
    if (score >= 90) return 5;
    if (score >= 75) return 4;
    if (score >= 60) return 3;
    return 2;
}

template <class Container>
void dump(const Container& c, const std::string& name) {
    std::cout << name << ": ";
    for (const auto& x : c)
        std::cout << x << ' ';
    std::cout << '\n';
}

int main() {
    std::vector<int> v{5, 3, 8, 1, 4, 6, 2, 9, 7, 10};

    std::cout << "=== 1. Поиск и подсчёт (немодифицирующие) ===\n";
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x > 7; });
    if (it != v.end())
        std::cout << "find_if(x > 7): " << *it << '\n';
    std::cout << "count_if(чётные): "
              << std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; })
              << '\n';
    std::cout << "any_of(x > 9): "
              << std::any_of(v.begin(), v.end(), [](int x) { return x > 9; })
              << ", all_of(x > 0): "
              << std::all_of(v.begin(), v.end(), [](int x) { return x > 0; })
              << ", none_of(x < 0): "
              << std::none_of(v.begin(), v.end(), [](int x) { return x < 0; })
              << '\n';

    auto [mn, mx] = std::minmax_element(v.begin(), v.end());
    std::cout << "min=" << *mn << ", max=" << *mx << '\n';

    std::cout << "\n=== 2. for_each: действие над каждым элементом ===\n";
    auto doubled = v;  // копия, чтобы не портить исходные данные
    std::for_each(doubled.begin(), doubled.end(), [](int& x) { x *= 2; });
    dump(doubled, "удвоенные");

    std::cout << "\n=== 3. transform и copy_if ===\n";
    std::vector<int> squares;
    squares.reserve(v.size());
    std::transform(v.begin(), v.end(), std::back_inserter(squares),
                   [](int x) { return x * x; });
    dump(squares, "квадраты");

    std::vector<int> evens;
    std::copy_if(v.begin(), v.end(), std::back_inserter(evens),
                 [](int x) { return x % 2 == 0; });
    dump(evens, "чётные");

    std::cout << "\n=== 4. remove_if + erase: идиома erase-remove ===\n";
    auto filtered = v;
    auto new_end = std::remove_if(filtered.begin(), filtered.end(),
                                  [](int x) { return x % 2 != 0; });
    std::cout << "size до erase: " << filtered.size()
              << " (remove_if только сдвигает элементы, размер не меняет)\n";
    filtered.erase(new_end, filtered.end());
    dump(filtered, "только чётные");

    std::cout << "\n=== 5. Сортировки ===\n";
    auto sorted = v;
    std::sort(sorted.begin(), sorted.end(),
              [](int x, int y) { return x > y; });
    dump(sorted, "по убыванию");

    std::vector<Student> students{
        {"Аня", 91},   {"Борис", 78}, {"Вера", 55}, {"Глеб", 88},
        {"Дина", 91},  {"Егор", 45},  {"Женя", 66}, {"Зоя", 91}};

    auto by_name = students;
    std::stable_sort(by_name.begin(), by_name.end(),
                     [](const Student& x, const Student& y) {
                         return x.score > y.score;
                     });
    std::cout << "stable_sort по score (равные сохраняют порядок): ";
    for (const auto& s : by_name)
        std::cout << s.name << '(' << s.score << ") ";
    std::cout << '\n';

    auto top = students;
    std::partial_sort(top.begin(), top.begin() + 3, top.end(),
                      [](const Student& x, const Student& y) {
                          if (x.score != y.score) return x.score > y.score;
                          return x.name < y.name;
                      });
    std::cout << "partial_sort top-3: ";
    for (int i = 0; i < 3; ++i)
        std::cout << top[i].name << '(' << top[i].score << ") ";
    std::cout << '\n';

    std::cout << "\n=== 6. accumulate: свёртка ===\n";
    const int sum = std::accumulate(v.begin(), v.end(), 0);
    const int product = std::accumulate(v.begin(), v.end(), 1,
                                        [](int acc, int x) { return acc * x; });
    std::cout << "сумма=" << sum << ", произведение=" << product << '\n';
    const double average = std::accumulate(
        students.begin(), students.end(), 0.0,
        [](double acc, const Student& s) { return acc + s.score; }) /
        static_cast<double>(students.size());
    std::cout << "средний балл=" << average << '\n';

    std::cout << "\n=== 7. partition: разделить по предикату ===\n";
    auto marks = v;
    auto pass_end = std::partition(marks.begin(), marks.end(),
                                   [](int x) { return x >= 5; });
    std::cout << "сдали (>=5): ";
    for (auto p = marks.begin(); p != pass_end; ++p)
        std::cout << *p << ' ';
    std::cout << "| не сдали: ";
    for (auto p = pass_end; p != marks.end(); ++p)
        std::cout << *p << ' ';
    std::cout << '\n';

    std::cout << "\n=== 8. iota: заполнить последовательностью ===\n";
    std::vector<int> seq(5);
    std::iota(seq.begin(), seq.end(), 1);
    dump(seq, "1..5");

    std::cout << "\n=== 9. Прикладная задача: оценки студентов ===\n";
    const auto excellent = std::count_if(
        students.begin(), students.end(),
        [](const Student& s) { return s.score >= 90; });
    std::cout << "набрали >= 90: " << excellent << '\n';

    std::array<int, 4> histogram{};  // оценки 2,3,4,5
    for (int g = 2; g <= 5; ++g) {
        histogram[static_cast<std::size_t>(g - 2)] = static_cast<int>(std::count_if(
            students.begin(), students.end(),
            [g](const Student& s) { return grade(s.score) == g; }));
    }
    for (int g = 2; g <= 5; ++g)
        std::cout << "оценка " << g << ": "
                  << histogram[static_cast<std::size_t>(g - 2)] << '\n';

    return 0;
}
