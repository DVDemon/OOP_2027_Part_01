// Пример 09: std::map и std::set — ассоциативные контейнеры на дереве.
//
// ПЛОХО (проверка наличия ключа через operator[]) — m[key] при отсутствии ключа
// молча ВСТАВЛЯЕТ элемент со значением по умолчанию:
//
//     std::map<std::string, int> scores;
//     if (scores["Петя"] >= 60) { ... }   // если ключа не было — он появился!
//     scores.size();                       // вырос на 1
//
// ХОРОШО — find()/count() только проверяют и читают, не модифицируя контейнер.
// map внутри — красно-чёрное дерево: узлы, как у list, лежат отдельно, но
// упорядочены по ключу; поиск/вставка/удаление — O(log n).

#include <cstddef>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>

int main() {
    std::cout << "=== 1. Вставка и чтение ===\n";
    std::map<std::string, int> scores;
    scores["Аня"] = 88;    // вставка через operator[]
    scores["Борис"] = 95;
    scores.insert({"Вера", 72});  // insert не перезаписывает существующий ключ
    std::cout << "Аня: " << scores["Аня"] << '\n';
    std::cout << "at(): " << scores.at("Борис") << '\n';

    std::cout << "\n=== 2. Ловушка operator[] ===\n";
    const std::size_t before = scores.size();
    const int mark = scores["Петя"];  // "Петя" ВСТАВЛЕН со значением 0!
    std::cout << "mark=" << mark << ", size: " << before << " -> "
              << scores.size() << "  <-- ключ появился\n";

    std::cout << "\n=== 3. Правильно: find/count не меняют контейнер ===\n";
    std::map<std::string, int> clean{{"Аня", 88}, {"Борис", 95}};
    const std::size_t clean_before = clean.size();

    auto it = clean.find("Борис");
    if (it != clean.end())
        std::cout << "find: " << it->first << " = " << it->second << '\n';
    if (clean.find("Петя") == clean.end())
        std::cout << "find(\"Петя\"): ключа нет (вернулся end())\n";

    std::cout << "count(\"Аня\")=" << clean.count("Аня")
              << ", count(\"Петя\")=" << clean.count("Петя") << '\n';
    std::cout << "size: " << clean_before << " -> " << clean.size()
              << "  <-- не изменился\n";

    std::cout << "\n=== 4. at() бросает исключение ===\n";
    try {
        (void)clean.at("Петя");
    } catch (const std::out_of_range&) {
        std::cout << "at(\"Петя\") бросило std::out_of_range\n";
    }

    std::cout << "\n=== 5. Обход — в порядке возрастания ключей ===\n";
    for (const auto& [key, value] : clean)
        std::cout << key << " -> " << value << '\n';

    std::cout << "\n=== 6. std::set: только уникальные ключи ===\n";
    std::set<int> s{3, 1, 3, 2, 1, 2};
    std::cout << "size=" << s.size() << " (дубликаты отброшены)\n";
    for (int x : s)
        std::cout << x << ' ';
    std::cout << "\nсодержит 2? " << (s.count(2) ? "да" : "нет") << '\n';

    std::cout << "\n=== 7. Сложность ===\n";
    std::cout << "map/set: поиск, вставка, удаление — O(log n), элементы упорядочены\n";
    std::cout << "vector:  поиск по значению — O(n), зато доступ по индексу — O(1)\n";

    return 0;
}
