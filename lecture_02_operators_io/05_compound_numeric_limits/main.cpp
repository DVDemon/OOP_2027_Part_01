// Пример 05: составные операторы присваивания и std::numeric_limits.
#include <iostream>
#include <limits>

int main() {
    std::cout << "=== Составные операторы присваивания ===\n";
    int x = 10;
    std::cout << "x = 10\n";
    x += 5;  std::cout << "x += 5   -> " << x << '\n';  // 15
    x -= 3;  std::cout << "x -= 3   -> " << x << '\n';  // 12
    x *= 2;  std::cout << "x *= 2   -> " << x << '\n';  // 24
    x /= 4;  std::cout << "x /= 4   -> " << x << '\n';  // 6
    x %= 4;  std::cout << "x %= 4   -> " << x << '\n';  // 2
    x <<= 2; std::cout << "x <<= 2  -> " << x << '\n';  // 8
    x >>= 1; std::cout << "x >>= 1  -> " << x << '\n';  // 4
    x &= 0b110; std::cout << "x &= 0b110 -> " << x << '\n';  // 4
    x |= 0b001; std::cout << "x |= 0b001 -> " << x << '\n';  // 5

    std::cout << "\n=== std::numeric_limits ===\n";
    std::cout << "int min:     " << std::numeric_limits<int>::min() << '\n';
    std::cout << "int max:     " << std::numeric_limits<int>::max() << '\n';
    std::cout << "int digits:  " << std::numeric_limits<int>::digits << "  (бит мантиссы)\n";
    std::cout << "double max:  " << std::numeric_limits<double>::max() << '\n';
    std::cout << "double eps:  " << std::numeric_limits<double>::epsilon() << '\n';
    std::cout << "double inf:  " << std::numeric_limits<double>::infinity() << '\n';

    std::cout << "\n=== NaN не равен сам себе ===\n";
    double nan_val = std::numeric_limits<double>::quiet_NaN();
    std::cout << "NaN == NaN : " << std::boolalpha << (nan_val == nan_val)
              << "  (всегда false!)\n";

    std::cout << "\n=== Переполнение знакового int — UB ===\n";
    int big = std::numeric_limits<int>::max();
    std::cout << "int max = " << big << '\n';
    // ПЛОХО: big += 1; — неопределённое поведение для signed.
    // ХОРОШО: проверить ДО операции, не приведёт ли она к переполнению.
    int addend = 1;
    if (big > std::numeric_limits<int>::max() - addend) {
        std::cout << "сложение переполнит int — операция отклонена\n";
    } else {
        big += addend;
        std::cout << "результат = " << big << '\n';
    }

    std::cout << "\n=== unsigned переполняется предсказуемо (wraparound) ===\n";
    unsigned int u = std::numeric_limits<unsigned int>::max();
    std::cout << "unsigned max = " << u << '\n';
    u += 1;  // определено стандартом: обнуляется
    std::cout << "после += 1   = " << u << "  (обёртка по модулю 2^N)\n";

    return 0;
}
