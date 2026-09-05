// Пример 11: неявные и явные преобразования типов (только базовые типы).
#include <iostream>
#include <typeinfo>

int main() {
    std::cout << "=== Неявные преобразования ===\n";

    std::cout << "\n-- Расширение (int -> double) --\n";
    int i = 42;
    double d = i;            // безопасно: значение не теряется
    std::cout << "int 42  -> double " << d << '\n';

    std::cout << "\n-- Сужение (double -> int): потеря дробной части --\n";
    double pi = 3.99;
    int truncated = pi;      // неявно: отбрасывается дробная часть -> 3
    std::cout << "double 3.99 -> int " << truncated
              << "  (дробная часть потеряна, тип int)\n";

    std::cout << "\n-- Integer promotion: short -> int перед арифметикой --\n";
    short s = 300;                      // помещается в short
    auto sum = s + 1;                   // НЕ short: оба операнда продвигаются в int
    // typeid(...).name() печатает «искажённое» имя типа: "i" = int, "d" = double.
    std::cout << "short + int  имеет тип "
              << typeid(sum).name() << ", значение " << sum << '\n';

    std::cout << "\n-- Usual arithmetic conversions: int -> double --\n";
    int a = 5;
    double b = 2.5;
    auto c = a + b;                     // int + double -> double
    std::cout << "5 + 2.5  имеет тип "
              << typeid(c).name() << ", значение " << c
              << "  (\"d\" = double)\n";
    // Целочисленное деление: 7 / 2 == 3, а 7 / 2.0 == 3.5
    std::cout << "7 / 2     = " << 7 / 2     << '\n';
    std::cout << "7 / 2.0   = " << 7 / 2.0   << '\n';

    std::cout << "\n-- bool: int -> bool (0 -> false, всё остальное -> true) --\n";
    bool b0 = 0;      // false
    bool b1 = 1;      // true
    bool b42 = 42;    // true (не «42», а true!)
    std::cout << std::boolalpha
              << "bool(0)  = " << b0  << '\n'
              << "bool(1)  = " << b1  << '\n'
              << "bool(42) = " << b42 << '\n';
    std::cout << "true  в int: " << static_cast<int>(true)  << '\n';
    std::cout << "false в int: " << static_cast<int>(false) << '\n';

    std::cout << "\n-- char: участвует в арифметике как целый --\n";
    char ch = 'A';                       // 65 в ASCII
    std::cout << "'A' + 1  = " << ch + 1
              << "  (int 66, а НЕ символ 'B')\n";

    std::cout << "\n=== Опасности неявных преобразований ===\n";

    std::cout << "\n-- signed <-> unsigned --\n";
    // Отрицательный int превращается в огромное unsigned (по модулю 2^N).
    int neg = -1;
    unsigned int u = neg;
    std::cout << "unsigned(-1) = " << u << '\n';
    // Классическая ловушка: -1 < 1u — ложь, т.к. -1 неявно приводится к
    // unsigned (4294967295 < 1 == false). Приводим явно, чтобы не ловить
    // предупреждение -Wsign-compare, но семантика неявного сравнения та же.
    std::cout << "(-1 < 1u)    = " << std::boolalpha
              << (static_cast<unsigned int>(neg) < 1u) << '\n';

    std::cout << "\n-- Сужение с потерей данных: double -> int --\n";
    double big = 3.7e9;                  // больше INT_MAX
    int overflow = big;                  // значение уже не помещается в int
    std::cout << "double 3.7e9 -> int: " << overflow
              << "  (результат неопределён по значению)\n";

    std::cout << "\n-- Сужение при присваивании: int -> short --\n";
    int big_int = 70000;
    short small = big_int;               // обрезается до 4464 (по модулю 2^16)
    std::cout << "int 70000 -> short: " << small << '\n';

    std::cout << "\n=== {} -инициализация запрещает сужающие преобразования ===\n";
    int safe = {42};                     // ок: не сужающее
    double to_double{42};                // ок: расширение
    std::cout << "safe = " << safe << ", to_double = " << to_double << '\n';
    // ПЛОХО (не компилируется): int bad{3.9};      // narrowing!
    // ПЛОХО (не компилируется): short bad2{70000}; // narrowing!

    std::cout << "\n=== Явные преобразования (C++ casts) ===\n";

    std::cout << "\n-- static_cast: числовые типы --\n";
    double pi2 = 3.14159;
    int t1 = static_cast<int>(pi2);      // 3 — явное сужение
    int t2 = static_cast<int>(pi2 * 100); // 314 — часто нужен именно такой шаг
    std::cout << "static_cast<int>(3.14159)     = " << t1 << '\n';
    std::cout << "static_cast<int>(3.14159*100) = " << t2 << '\n';

    std::cout << "\n-- static_cast: char <-> int --\n";
    char c2 = 'B';
    std::cout << "int('B')     = " << static_cast<int>(c2) << '\n';
    std::cout << "char(66)     = '" << static_cast<char>(66) << "'\n";

    std::cout << "\n-- static_cast: double -> float (и обратно) --\n";
    float f = static_cast<float>(pi2);   // сужение double -> float
    std::cout << "float(3.14159) = " << f << '\n';

    std::cout << "\n-- static_cast: числа -> bool --\n";
    std::cout << "static_cast<bool>(0)  = " << static_cast<bool>(0)  << '\n';
    std::cout << "static_cast<bool>(42) = " << static_cast<bool>(42) << '\n';

    std::cout << "\n-- static_cast: unsigned <-> signed --\n";
    unsigned int u2 = static_cast<unsigned int>(-1);
    int back = static_cast<int>(u2);
    std::cout << "static_cast<unsigned>(-1) = " << u2
              << ", обратно -> " << back << '\n';

    std::cout << "\n-- C-style cast: избегайте --\n";
    // (int)pi2 работает, но в сложных случаях это один из *_cast —
    // непонятно какой. Именованные cast'ы читаются однозначно.
    int t3 = (int)pi2;                   // то же, что static_cast, но менее явно
    std::cout << "(int)3.14159 = " << t3
              << "  (лучше: static_cast<int>(...))\n";

    std::cout << "\n=== Явные преобразования для решения задач ===\n";
    // Взять целую и дробную части числа.
    double value = 123.456;
    int whole = static_cast<int>(value);                       // 123
    double frac = value - static_cast<double>(whole);          // 0.456
    std::cout << value << ": целая часть " << whole
              << ", дробная " << frac << '\n';

    // Округление с помощью static_cast (вместо теряющей <cmath>).
    double x = 2.7;
    int rounded_down = static_cast<int>(x);       // 2
    int rounded_up   = static_cast<int>(x + 0.5); // 3
    std::cout << "static_cast<int>(2.7)      = " << rounded_down << '\n';
    std::cout << "static_cast<int>(2.7+0.5)  = " << rounded_up   << '\n';

    return 0;
}
