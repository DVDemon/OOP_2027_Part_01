// Пример 10: частотный анализ слов — всё вместе.
//
// ПЛОХО (частоты вручную) — параллельные массивы «слово, частота» и линейный
// поиск каждого слова в накопленном списке: O(n^2) на n уникальных слов плюс
// ручное сравнение строк:
//
//     std::vector<std::string> words;  std::vector<int> counts;
//     for (const auto& w : text) {
//         auto it = std::find(words.begin(), words.end(), w);  // O(n)
//         if (it == words.end()) { words.push_back(w); counts.push_back(1); }
//         else ++counts[it - words.begin()];
//     }
//
// ХОРОШО — map сам обеспечивает уникальность ключей и поиск за O(log n):
// ++freq[word] вставляет новый ключ со значением 0 и инкрементирует; для уже
// встречавшегося слова — просто инкремент. Сортировку по частоте делает std::sort
// с лямбда-компаратором после переноса пар в вектор.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

int main() {
    // «Текст» — строка; слова достаём итераторами потока (в реальной программе
    // так же читают файл: std::ifstream + istream_iterator).
    std::istringstream input(
        "apple banana cherry apple banana "
        "apple date banana apple elderberry cherry");

    const std::vector<std::string> words(
        std::istream_iterator<std::string>{input},
        std::istream_iterator<std::string>{});

    std::cout << "=== 1. Слова из текста ===\n";
    for (const auto& w : words)
        std::cout << w << ' ';
    std::cout << "\nвсего слов: " << words.size() << '\n';

    // Шаг 1. Частоты: map даёт O(n log n) и устраняет дубликаты ключей.
    std::map<std::string, int> freq;
    for (const auto& word : words)
        ++freq[word];

    std::cout << "\n=== 2. Пары «слово, частота» (map отсортирован по ключу) ===\n";
    for (const auto& [word, count] : freq)
        std::cout << word << ": " << count << '\n';

    // Шаг 2. Пары переносим в вектор: map сортируется по КЛЮЧУ, а нужен
    // порядок по частоте.
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
    std::cout << "\n=== 3. Топ-" << top << " самых частых слов ===\n";
    for (int i = 0; i < top && i < static_cast<int>(items.size()); ++i)
        std::cout << items[static_cast<std::size_t>(i)].first << ": "
                  << items[static_cast<std::size_t>(i)].second << '\n';

    return 0;
}
