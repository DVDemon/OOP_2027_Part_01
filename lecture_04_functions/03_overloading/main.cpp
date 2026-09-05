// Пример 03: перегрузка функций.
// Несколько функций с одним именем различаются СПИСКОМ ПАРАМЕТРОВ.
#include <iostream>
#include <string>

// Перегрузка по количеству параметров.
int max_of(int a, int b) {
    return (a > b) ? a : b;
}

int max_of(int a, int b, int c) {
    return max_of(max_of(a, b), c);
}

// Перегрузка по типу параметров.
double max_of(double a, double b) {
    return (a > b) ? a : b;
}

// Перегрузка по квалификатору const: для неконстантного и константного аргумента
// выбираются разные версии.
void process(std::string& s) {
    std::cout << "неконстантная версия: " << s << '\n';
}

void process(const std::string& s) {
    std::cout << "константная версия:   " << s << '\n';
}

int main() {
    std::cout << "max_of(3, 7)       = " << max_of(3, 7) << '\n';        // int,int
    std::cout << "max_of(3, 7, 5)    = " << max_of(3, 7, 5) << '\n';     // int,int,int
    std::cout << "max_of(3.14, 2.71) = " << max_of(3.14, 2.71) << '\n';  // double,double

    std::string s = "hello";
    process(s);                       // lvalue -> неконстантная версия
    process(std::string("temp"));     // rvalue -> const& версия

    // Тип возвращаемого значения НЕ участвует в разрешении перегрузки:
    //   int    foo(int);
    //   double foo(int);  // ОШИБКА: совпадает с первой по параметрам

    return 0;
}
