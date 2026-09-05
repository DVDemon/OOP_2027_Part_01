// Пример 09: sizeof, выравнивание и padding.
#include <cstddef>
#include <iostream>

// Плохой порядок полей: char, double, char -> много padding.
struct Bad {
    char a;    // 1 байт + 7 байт padding (чтобы double встал на адрес, кратный 8)
    double b;  // 8 байт
    char c;    // 1 байт + 7 байт padding (до размера, кратного alignof = 8)
};

// Хороший порядок: крупные поля раньше -> меньше дыр.
struct Good {
    double b;  // 8 байт
    char a;    // 1 байт
    char c;    // 1 байт + 6 байт padding
};

// Пустой класс всё равно занимает >= 1 байт: гарантия уникальности адреса.
struct Empty {};

// Управление выравниванием: alignas задаёт более строгое выравнивание.
struct alignas(16) AlignedData {
    float values[4];  // 16 байт, выровнено на границу 16
};

int main() {
    std::cout << "sizeof(char)   = " << sizeof(char)   << '\n';
    std::cout << "sizeof(double) = " << sizeof(double) << '\n';

    std::cout << "\nsizeof(Bad)  = " << sizeof(Bad)
              << "  alignof(Bad)  = " << alignof(Bad)  << '\n';
    std::cout << "sizeof(Good) = " << sizeof(Good)
              << "  alignof(Good) = " << alignof(Good) << '\n';
    std::cout << "  -> те же поля, но другой порядок экономит "
              << (sizeof(Bad) - sizeof(Good)) << " байт на каждый объект\n";

    std::cout << "\nsizeof(Empty) = " << sizeof(Empty)
              << "  (>= 1: уникальный адрес)\n";

    std::cout << "\nsizeof(AlignedData)  = " << sizeof(AlignedData)
              << "  alignof(AlignedData) = " << alignof(AlignedData) << '\n';

    // Смещения полей в Bad: видно, где компилятор вставил дыры.
    std::cout << "\noffsetof(Bad, a) = " << offsetof(Bad, a) << '\n';
    std::cout << "offsetof(Bad, b) = " << offsetof(Bad, b) << '\n';
    std::cout << "offsetof(Bad, c) = " << offsetof(Bad, c) << '\n';

    return 0;
}
