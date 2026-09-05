// Пример 10: пользовательские литералы и канонические формы операторов.
#include <iostream>

#include "money.h"

int main() {
    using namespace std::literals;  // не обязательно, но показывает практику

    // Пользовательские литералы делают код выразительным.
    Money price = 100_rub;       // целочисленный литерал
    Money discount = 49.90_rub;  // дробный литерал

    std::cout << "цена    = " << price << '\n';
    std::cout << "скидка  = " << discount << '\n';

    // Арифметика (operator-) и сравнение (<=>) — канонические формы.
    Money total = price - discount;
    std::cout << "к оплате = " << total << '\n';

    std::cout << std::boolalpha;
    std::cout << "total > 0 руб.   : " << (total > 0_rub) << '\n';
    std::cout << "bool(total)      : " << static_cast<bool>(total) << '\n';
    std::cout << "bool(0_rub)      : " << static_cast<bool>(0_rub) << '\n';

    return 0;
}
