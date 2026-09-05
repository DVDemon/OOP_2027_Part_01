// Пример 04: операторы сравнения и operator<=> (C++20).
#include <iostream>

#include "comparable.h"

int main() {
    Date d1(2025, 1, 1);
    Date d2(2025, 6, 15);

    // Все шесть операций доступны благодаря одному "<=> = default".
    std::cout << std::boolalpha;
    std::cout << "d1 <  d2 : " << (d1 < d2) << '\n';
    std::cout << "d1 == d2 : " << (d1 == d2) << '\n';
    std::cout << "d1 >= d2 : " << (d1 >= d2) << '\n';

    // Пользовательский <=>: сравнение строк без учёта регистра.
    CaseInsensitiveString a("Hello");
    CaseInsensitiveString b("hello");
    CaseInsensitiveString c("WORLD");

    std::cout << "\"Hello\" == \"hello\" : " << (a == b) << '\n';
    std::cout << "\"Hello\" <  \"WORLD\" : " << (a < c) << '\n';

    return 0;
}
