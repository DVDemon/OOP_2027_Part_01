// Пример 05: частотный анализ слов — map + vector + sort + лямбды.
//
// ПЛОХО (подсчёт частот вручную) — линейный поиск каждого слова в накопленном
// списке даёт O(n^2) на n уникальных слов и много кода для рукописного сравнения:
//
//     // паралелльные массивы «слово, частота» + ручной поиск на каждое слово
//
// ХОРОШО — std::map сам обеспечивает уникальность ключей и поиск за O(log n):
// ++freq[word] вставляет новый ключ со значением 0 и инкрементирует; для уже
// встречавшегося слова — просто инкремент. Сортировку по частоте делает std::sort
// с лямбда-компаратором после переноса пар в вектор.

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

int main() {
    // «Текст» — просто список слов (в реальной программе его читают из файла).
    const std::vector<std::string> text = {
        "apple", "banana", "cherry", "apple", "banana",
        "apple", "date",   "banana", "apple"
    };

    // Шаг 1. Частоты слов: map даёт O(n log n) и устраняет дубликаты ключей.
    std::map<std::string, int> freq;
    for (const auto& word : text)
        ++freq[word];

    std::cout << "=== все пары «слово, частота» (map отсортирован по ключу) ===\n";
    for (const auto& [word, count] : freq)
        std::cout << word << ": " << count << '\n';

    // Шаг 2. Переносим пары в вектор: map сортируется по КЛЮЧУ, а нам нужен
    // порядок по частоте, поэтому работаем с вектором пар.
    std::vector<std::pair<std::string, int>> items(freq.begin(), freq.end());

    // Шаг 3. Сортировка по убыванию частоты (при равенстве — по слову).
    std::sort(items.begin(), items.end(),
              [](const auto& a, const auto& b) {
                  if (a.second != b.second)
                      return a.second > b.second;
                  return a.first < b.first;
              });

    // Шаг 4. Топ-3 самых частых слов.
    const int top = 3;
    std::cout << "\n=== топ-" << top << " самых частых слов ===\n";
    for (int i = 0; i < top && i < static_cast<int>(items.size()); ++i)
        std::cout << items[i].first << ": " << items[i].second << '\n';

    return 0;
}
