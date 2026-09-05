// Пример 09: range-for со ссылками и категории значений (lvalue, prvalue, xvalue).
#include <iostream>
#include <string>
#include <utility>
#include <vector>

int main() {
    int numbers[5] = {1, 2, 3, 4, 5};

    // --- Range-for: копия vs ссылка vs const-ссылка ---
    std::cout << "--- Range-for ---\n";

    // По ссылке: можем менять элементы на месте.
    for (auto& n : numbers) {
        n *= 2;  // удваиваем каждый элемент
    }

    // По const-ссылке: только чтение, без копий (предпочтительно для вывода).
    std::cout << "после удвоения:";
    for (const auto& n : numbers) {
        std::cout << ' ' << n;  // 2 4 6 8 10
    }
    std::cout << '\n';

    // По значению (auto n) была бы копия каждого элемента — лишняя работа
    // для больших объектов; здесь для int не критично.

    // --- Категории значений ---
    std::cout << "\n--- Категории значений ---\n";

    int x = 10;          // x — lvalue (имеет устойчивый адрес)
    int* p = &x;         // взять адрес можно только у lvalue
    std::cout << "lvalue x: можно взять адрес = "
              << static_cast<void*>(p) << '\n';

    int y = x + 1;       // (x + 1) — prvalue (временный результат)
    std::cout << "prvalue (x + 1) -> y = " << y << '\n';

    int& lref = x;       // lvalue-ссылка к lvalue — OK
    // int& bad = x + 1; // ОШИБКА: lvalue-ссылка не привязывается к prvalue
    lref = 20;
    std::cout << "lvalue-ссылка изменила x -> " << x << '\n';

    const int& cref = x + 1;  // const-lvalue-ссылка продлевает жизнь prvalue
    std::cout << "const-ссылка к prvalue: " << cref << '\n';

    int&& rref = x + 1;       // rvalue-ссылка к prvalue
    std::cout << "rvalue-ссылка к prvalue: " << rref << '\n';

    std::string s = "hello";
    std::string moved = std::move(s);  // std::move(s) — xvalue (готов к перемещению)
    std::cout << "xvalue после move: moved = " << moved << '\n';

    return 0;
}
