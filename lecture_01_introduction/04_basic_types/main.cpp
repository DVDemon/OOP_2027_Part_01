// Пример 04: базовые типы, литералы, sizeof.
#include <cstdint>
#include <iostream>

int main() {
    // --- Целочисленные типы и их размеры ---
    std::cout << "sizeof(char)      = " << sizeof(char)      << '\n';
    std::cout << "sizeof(short)     = " << sizeof(short)     << '\n';
    std::cout << "sizeof(int)       = " << sizeof(int)       << '\n';
    std::cout << "sizeof(long)      = " << sizeof(long)      << '\n';
    std::cout << "sizeof(long long) = " << sizeof(long long) << '\n';
    std::cout << "sizeof(bool)      = " << sizeof(bool)      << "  (>=1 байт)\n";

    // --- Типы фиксированного размера (рекомендуются для портируемости) ---
    std::int8_t  i8  = 127;
    std::int32_t i32 = 2'147'483'647;     // разделитель разрядов ' (C++14)
    std::int64_t i64 = 9'223'372'036'854'775'807LL;
    std::cout << "int8_t=" << static_cast<int>(i8)
              << " int32_t=" << i32 << " int64_t=" << i64 << '\n';

    // --- Литералы в разных системах счисления ---
    int dec = 42;        // десятичный
    int oct = 052;       // ВНИМАНИЕ: ведущий 0 -> восьмеричный! (= 42)
    int hex = 0x2A;      // шестнадцатеричный (= 42)
    int bin = 0b101010;  // двоичный, C++14 (= 42)
    std::cout << "dec=" << dec << " oct(052)=" << oct
              << " hex(0x2A)=" << hex << " bin=" << bin << '\n';

    // --- Плавающая точка ---
    float       f  = 3.14f;
    double      d  = 3.141592653;
    long double ld = 3.14L;
    double avogadro = 6.022e23;   // научная нотация
    std::cout << "float=" << f << " double=" << d
              << " long double=" << static_cast<double>(ld)
              << " avogadro=" << avogadro << '\n';

    // --- Символы ---
    char c = 'A';
    std::cout << "char 'A' as int = " << static_cast<int>(c) << '\n';

    return 0;
}
