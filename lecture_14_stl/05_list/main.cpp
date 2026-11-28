// Пример 05: std::list — двусвязный список, узлы в куче.
//
// ПЛОХО (list там, где нужен vector) — у list нет произвольного доступа, каждый
// узел лежит отдельно, обход прыгает по памяти и промахивается мимо кэша:
//
//     std::list<int> l = ...;
//     l[3];                       // не компилируется: оператора [] нет
//     std::sort(l.begin(), l.end());  // не компилируется: нужен random access
//
// ХОРОШО — list выбирают, когда важны стабильные адреса элементов и вставка/
// удаление/перенос узлов за O(1): указатель на элемент остаётся валидным при
// вставке и удалении соседей, а splice переносит узлы между списками без копий.
// Сортировка — member-функция l.sort(), она переставляет узлы, а не значения.

#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

template <class Container>
void dump(const Container& c, const std::string& name) {
    std::cout << name << " [" << c.size() << "]: ";
    for (const auto& x : c)
        std::cout << x << ' ';
    std::cout << '\n';
}

int main() {
    std::cout << "=== 1. Узлы: адреса элементов не рядом ===\n";
    std::list<int> l{1, 2, 3};
    std::cout << "sizeof(list<int>) = " << sizeof(l)
              << " (у объекта — служебные указатели; элементы — отдельные узлы)\n";
    for (auto it = l.begin(); it != l.end(); ++it)
        std::cout << "  " << *it << " -> " << static_cast<const void*>(&*it) << '\n';
    std::cout << "адреса элементов не идут подряд — это разные блоки в куче\n";

    std::cout << "\n=== 2. Указатели на элементы стабильны ===\n";
    int* p = &l.back();  // указывает на 3
    std::cout << "p -> " << *p << '\n';
    l.push_front(0);  // новый узел
    l.push_back(4);   // новый узел
    std::cout << "после push_front/push_back: p -> " << *p
              << " (тот же узел, указатель не инвалидирован)\n";

    std::cout << "\n=== 3. Нет operator[], но есть итераторы и advance ===\n";
    auto third = l.begin();
    std::advance(third, 2);
    std::cout << "третий элемент: " << *third << '\n';

    std::cout << "\n=== 4. erase инвалидирует только удалённый ===\n";
    l.pop_front();  // удалили 0
    std::cout << "после pop_front: p -> " << *p << " (узел 3 жив)\n";

    std::cout << "\n=== 5. splice: перенос узлов за O(1), без копий ===\n";
    std::list<int> other{10, 20};
    int* q = &other.front();  // 10
    auto pos = l.begin();
    std::advance(pos, 2);      // перед элементом 3
    l.splice(pos, other);      // узлы 10, 20 переехали в l
    dump(l, "l после splice");
    dump(other, "other после splice");
    std::cout << "q -> " << *q << " (узел переехал, адрес тот же)\n";

    std::cout << "\n=== 6. Сортировка и remove_if — member-функции ===\n";
    l.sort();  // переставляет узлы; итераторы и указатели остаются валидными
    dump(l, "после sort");
    std::cout << "p -> " << *p << " (после sort указатель всё ещё валиден)\n";

    l.remove_if([](int x) { return x % 2 == 0; });  // удаляет чётные
    dump(l, "после remove_if(чётные)");
    std::cout << "p -> " << *p << " (3 нечётное, узел жив)\n";

    std::cout << "\n=== 7. Цена list: память и кэш ===\n";
    std::cout << "vector<int>: элементы подряд, один блок; list<int>: узел =\n"
                 "значение + два указателя (~24 байта на int) + отдельная аллокация\n";
    std::vector<int> v{1, 2, 3};
    std::cout << "sizeof(vector)=" << sizeof(v) << ", sizeof(list)=" << sizeof(l) << '\n';
    std::cout << "поэтому list — не «быстрее для вставок вообще»: на обходе и\n"
                 "поиске vector обычно выигрывает; list силён в splice и стабильных адресах\n";

    return 0;
}
