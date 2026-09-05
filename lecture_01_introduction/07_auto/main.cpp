// Пример 07: вывод типа auto.
#include <iostream>
#include <string>


int main() {
    auto x = 42;                    // int
    auto y = 3.14;                  // double
    auto z = "hello";               // const char*
    auto s = std::string("hello");  // std::string

    std::cout << "x=" << x << " y=" << y << " z=" << z << " s=" << s << '\n';

    return 0;
}
