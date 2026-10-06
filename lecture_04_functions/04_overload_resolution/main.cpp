// Пример 04: алгоритм разрешения перегрузки и приоритет преобразований.
#include <iostream>

// Три перегрузки, различающиеся типом параметра.
void foo(int x)    { std::cout << "foo(int):    " << x << '\n'; }
void foo(double x) { std::cout << "foo(double): " << x << '\n'; }
void foo(long x)   { std::cout << "foo(long):   " << x << '\n'; }


auto foo_foo = [](auto value) -> int {
    if(true) 
        return 10;
    return 0.0;
};

int main() {
    
    // Приоритет преобразований (от лучшего к худшему):
    //   1) точное совпадение
    //   2) promotion (short->int, float->double, bool->int)
    //   3) стандартное преобразование (int<->double и т.п.)
    //   4) пользовательское преобразование

    foo(42);     // точное совпадение -> foo(int)
    foo(3.14);   // точное совпадение -> foo(double)
    foo(42L);    // точное совпадение -> foo(long)
    foo(3.14f);  // promotion float->double -> foo(double)

  
    std::cout << foo_foo(32) << foo_foo("Hello") << std::endl;

    short s = 7;
    foo(s);      // promotion short->int -> foo(int)

    // foo(42u);  // ОШИБКА: ambiguous!
    //            // unsigned int -> int и unsigned int -> long одинаково хороши,
    //            // ни одна перегрузка не «лучше» другой.



    char c = 'A';
    foo(c);      // promotion char->int -> foo(int) (печатает 65)

    return 0;
}
