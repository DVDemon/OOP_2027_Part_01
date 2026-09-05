// Пример 02: std::map и std::set — ассоциативные контейнеры.
//
// ПЛОХО (ловушка operator[]) — проверка наличия ключа через m[key] молча
// ВСТАВЛЯЕТ элемент со значением по умолчанию:
//
//     std::map<std::string, int> scores;
//     if (scores["Петя"] >= 60) { ... }   // если ключа не было — он появился!
//     scores.size();                      // вырос на 1
//
// ХОРОШО — find()/count() только проверяют и читают, не модифицируя контейнер.
// Здесь же показываем обход map в порядке ключей, at() с исключением и std::set
// для устранения дубликатов.

#include <iostream>
#include <map>
#include <set>
#include <string>

int main() {
    std::cout << "=== вставка и чтение ===\n";
    std::map<std::string, int> scores;
    scores["Аня"] = 88;             // вставка через operator[]
    scores["Борис"] = 95;

    std::cout << "Аня: " << scores["Аня"] << '\n';
    std::cout << "at(): " << scores.at("Борис") << '\n';

    std::cout << "\n=== ловушка operator[]: чтение несуществующего ключа ===\n";
    std::cout << "size до проверки: " << scores.size() << '\n';
    int mark = scores["Петя"];      // "Петя" ВСТАВЛЕН со значением 0!
    std::cout << "mark = " << mark
              << ", size после: " << scores.size() << "  <-- вырос!\n";

    std::cout << "\n=== правильно: find/count не меняют контейнер ===\n";
    std::map<std::string, int> clean{{"Аня", 88}, {"Борис", 95}};
    std::cout << "size до: " << clean.size() << '\n';

    auto it = clean.find("Борис");
    if (it != clean.end())
        std::cout << "find: " << it->first << " = " << it->second << '\n';
    if (clean.find("Петя") == clean.end())
        std::cout << "find(\"Петя\"): ключа нет (end)\n";

    std::cout << "count(\"Аня\") = " << clean.count("Аня")
              << ", count(\"Петя\") = " << clean.count("Петя") << '\n';
    std::cout << "size после: " << clean.size() << "  <-- не изменился\n";

    std::cout << "\n=== at() бросает исключение для отсутствующего ключа ===\n";
    try {
        (void)clean.at("Петя");
    } catch (const std::out_of_range&) {
        std::cout << "at(\"Петя\") бросило std::out_of_range\n";
    }

    std::cout << "\n=== обход map в порядке возрастания ключей ===\n";
    for (const auto& [key, value] : clean)
        std::cout << key << " -> " << value << '\n';

    std::cout << "\n=== std::set: только уникальные ключи ===\n";
    std::set<int> s{3, 1, 3, 2, 1, 2};
    std::cout << "size=" << s.size() << " (дубликаты отброшены)\n";
    for (int x : s)
        std::cout << x << ' ';
    std::cout << "\nсодержит 2? " << (s.count(2) ? "да" : "нет") << '\n';

    return 0;
}
