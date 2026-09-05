// Пример 09: циклы for, range-based for, while, do-while.
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

int main() {
    std::cout << "=== Классический for ===\n";
    for (int i = 0; i < 5; ++i) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== for с двумя переменными (оператор запятая) ===\n";
    for (int i = 0, j = 10; i < j; ++i, --j) {
        std::cout << i << ':' << j << "  ";
    }
    std::cout << '\n';

    std::cout << "\n=== Range-based for ===\n";
    std::vector<std::string> names{"Алиса", "Борис", "Вера"};

    // ПЛОХО: for (auto name : names) — копирует каждую строку.
    // ХОРОШО: const auto& — без копирования, без изменения.
    for (const auto& name : names) {
        std::cout << name << ' ';
    }
    std::cout << '\n';

    // По ссылке — когда нужно менять элементы.
    for (auto& name : names) {
        name += "!";
    }
    for (const auto& name : names) {
        std::cout << name << ' ';
    }
    std::cout << '\n';

    std::cout << "\n=== while: проверка ДО тела ===\n";
    int n = 1;
    while (n <= 1024) {
        std::cout << n << ' ';
        n *= 2;
    }
    std::cout << '\n';

    std::cout << "\n=== do-while: тело хотя бы один раз ===\n";
    // Здесь условие сразу ложно, но тело всё равно выполнится один раз.
    int count = 100;
    do {
        std::cout << "выполнено один раз, count=" << count << '\n';
        ++count;
    } while (count < 100);

    std::cout << "\n=== Вложенные циклы: таблица умножения ===\n";
    for (int i = 1; i <= 5; ++i) {
        for (int j = 1; j <= 5; ++j) {
            std::cout << std::setw(4) << i * j;
        }
        std::cout << '\n';
    }

    return 0;
}
