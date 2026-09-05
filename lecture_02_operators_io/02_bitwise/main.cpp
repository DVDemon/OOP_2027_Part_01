// Пример 02: побитовые операторы и битовые флаги.
#include <bitset>
#include <cstdint>
#include <iostream>

int main() {
    // Для побитовых операций используем БЕЗЗНАКОВЫЕ типы:
    // сдвиги и ~ для знаковых чисел ведут себя сюрпризно.
    unsigned int a = 0b1100;  // 12
    unsigned int b = 0b1010;  // 10

    std::cout << "=== Побитовые операторы (4 бита) ===\n";
    std::cout << "a      = " << std::bitset<4>(a) << '\n';
    std::cout << "b      = " << std::bitset<4>(b) << '\n';
    std::cout << "a & b  = " << std::bitset<4>(a & b) << "  (AND)\n";
    std::cout << "a | b  = " << std::bitset<4>(a | b) << "  (OR)\n";
    std::cout << "a ^ b  = " << std::bitset<4>(a ^ b) << "  (XOR)\n";
    std::cout << "~a     = " << std::bitset<8>(~a) << "  (NOT, 8 бит)\n";
    std::cout << "a << 1 = " << std::bitset<8>(a << 1) << "  (сдвиг влево)\n";
    std::cout << "a >> 1 = " << std::bitset<8>(a >> 1) << "  (сдвиг вправо)\n";

    std::cout << "\n=== Битовые флаги (права доступа) ===\n";
    constexpr unsigned int READ = 0b001;
    constexpr unsigned int WRITE = 0b010;
    constexpr unsigned int EXECUTE = 0b100;

    unsigned int permissions = READ | WRITE;  // установить два флага
    std::cout << "READ|WRITE         = " << std::bitset<3>(permissions) << '\n';

    bool can_read = (permissions & READ) != 0;  // проверить флаг
    std::cout << "can_read           = " << std::boolalpha << can_read << '\n';

    permissions |= EXECUTE;   // добавить флаг
    std::cout << "после |= EXECUTE   = " << std::bitset<3>(permissions) << '\n';

    permissions &= ~WRITE;    // сбросить флаг (И с инверсией маски)
    std::cout << "после &= ~WRITE    = " << std::bitset<3>(permissions) << '\n';

    permissions ^= EXECUTE;   // переключить флаг (XOR)
    std::cout << "после ^= EXECUTE   = " << std::bitset<3>(permissions) << '\n';

    std::cout << "\n=== Сдвиг как умножение/деление на степень двойки ===\n";
    unsigned int n = 3;
    std::cout << "3 << 4 = " << (n << 4) << "  (== 3 * 16)\n";
    std::cout << "48 >> 2 = " << (48u >> 2) << "  (== 48 / 4)\n";

    std::cout << "\n=== Опасность приоритета: & ниже, чем == ===\n";
    // ПЛОХО: permissions & READ == 0  ->  permissions & (READ == 0)
    //        потому что == имеет более высокий приоритет, чем &.
    // ХОРОШО: всегда брать побитовую операцию в скобки.
    std::cout << "(permissions & READ) != 0 = "
              << ((permissions & READ) != 0) << '\n';

    return 0;
}
