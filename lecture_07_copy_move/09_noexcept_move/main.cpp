// Пример 09: noexcept для move-операций.
//
// std::vector при реаллокации перемещает элементы ТОЛЬКО если перемещающий
// конструктор помечен noexcept. Иначе (ради строгой гарантии исключений)
// вектор будет КОПИРОВАТЬ элементы. Этот пример показывает разницу на двух
// почти одинаковых классах: один с noexcept-move, другой — без.
#include <iostream>
#include <type_traits>
#include <vector>

// move-конструктор noexcept -> vector будет перемещать при реаллокации.
class Good {
public:
    Good() = default;
    Good(const Good&) { std::cout << "  Good: копирование\n"; }
    Good(Good&&) noexcept { std::cout << "  Good: перемещение\n"; }
    Good& operator=(const Good&) = default;
    Good& operator=(Good&&) noexcept = default;
};

// move-конструктор БЕЗ noexcept -> vector будет копировать при реаллокации.
class Bad {
public:
    Bad() = default;
    Bad(const Bad&) { std::cout << "  Bad: копирование\n"; }
    Bad(Bad&&) { std::cout << "  Bad: перемещение\n"; }  // нет noexcept!
    Bad& operator=(const Bad&) = default;
    Bad& operator=(Bad&&) = default;
};

int main() {
    std::cout << std::boolalpha;
    std::cout << "is_nothrow_move_constructible<Good> = "
              << std::is_nothrow_move_constructible_v<Good> << '\n';
    std::cout << "is_nothrow_move_constructible<Bad>  = "
              << std::is_nothrow_move_constructible_v<Bad> << "\n\n";

    std::cout << "Good в vector (реаллокация перемещает):\n";
    {
        std::vector<Good> v;
        v.reserve(1);
        v.emplace_back();  // ёмкость 1
        v.emplace_back();  // реаллокация: перемещение существующего элемента
    }

    std::cout << "\nBad в vector (реаллокация КОПИРУЕТ):\n";
    {
        std::vector<Bad> v;
        v.reserve(1);
        v.emplace_back();
        v.emplace_back();  // реаллокация: копирование вместо перемещения!
    }

    std::cout << "\nВывод: noexcept на move-конструкторе позволяет контейнерам "
                 "перемещать, а не копировать.\n";
    return 0;
}
