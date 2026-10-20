// Пример 10: демонстрация учебного класса String.
//
// Объединяет все идеи лекции: глубокое копирование, перемещение, copy-and-swap,
// noexcept-move, перегруженные операторы. Тесты — в tests.cpp (Google Test).
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

#include "lec_string.h"

int main() {
    using lec07::String;

    String s1("Hello");
    String s2(", World");

    String s3 = s1 + s2;  // operator+ -> копия + перемещение
    std::cout << "s3 = " << s3 << " (size = " << s3.size() << ")\n";

    String s4 = s1;  // копирующий конструктор (глубокая копия)
    s4[0] = 'h';
    std::cout << "s1 = " << s1 << " (оригинал не изменился)\n";
    std::cout << "s4 = " << s4 << " (изменена копия)\n";

    String s5 = std::move(s3);  // перемещающий конструктор
    std::cout << "s5 = " << s5 << " (перемещён из s3)\n";
    std::cout << "s3 после move = \"" << s3
              << "\" (валидное пустое состояние)\n";

    s4 = s2;  // присваивание по значению (copy-and-swap)
    std::cout << "s4 после s4 = s2: " << s4 << '\n';

    std::cout << "\ns1 == s2 ? " << std::boolalpha << (s1 == s2) << '\n';
    std::cout << "s2 == s4 ? " << (s2 == s4) << '\n';

    std::cout << "\nis_nothrow_move_constructible<String> = "
              << std::is_nothrow_move_constructible_v<String> << '\n';

    std::cout << "\nString в std::vector (перемещается при реаллокации):\n";
    std::vector<String> v;
    v.reserve(1);
    v.push_back(String("one"));
    v.push_back(String("two"));  // реаллокация -> перемещение (move noexcept)
    for (const String& s : v) std::cout << "  " << s << '\n';

    return 0;
}
