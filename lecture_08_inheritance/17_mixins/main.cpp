// Пример 07: примеси (mixins) через CRTP.
//
// Демонстрирует:
//   * добавление функциональности через множественное наследование от
//     шаблонных примесей Printable<T> и Comparable<T>;
//   * безопасный вид MI: примеси не содержат данных и не образуют ромб;
//   * нулевые рантайм-затраты (статический полиморфизм, без virtual).

#include <iostream>

#include "student.h"

int main() {
    Student a("Alice", 4.5);
    Student b("Bob", 3.8);

    // print() пришёл из Printable<Student>; ему нужен только toString().
    a.print();
    b.print();

    std::cout << std::boolalpha;
    // ==, < определены в Student; >, !=, <=, >= достроены примесью Comparable.
    std::cout << "a == b: " << (a == b) << '\n';
    std::cout << "a != b: " << (a != b) << '\n';
    std::cout << "a >  b: " << (a > b) << '\n';
    std::cout << "a <= b: " << (a <= b) << '\n';

    return 0;
}
