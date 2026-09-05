// Пример 08: указатели на функции и std::function.
#include <functional>
#include <iostream>

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

// Тип: указатель на функцию (int, int) -> int.
using BinaryOp = int (*)(int, int);

int apply(BinaryOp op, int x, int y) {
    return op(x, y);
}

int main() {
    // --- Указатель на функцию ---
    BinaryOp operation = add;  // имя функции неявно преобразуется в указатель
    std::cout << "operation(3, 4) = " << operation(3, 4) << '\n';  // 7
    operation = mul;
    std::cout << "operation(3, 4) = " << operation(3, 4) << '\n';  // 12

    std::cout << "apply(add, 10, 20) = " << apply(add, 10, 20) << '\n';  // 30
    std::cout << "apply(mul, 10, 20) = " << apply(mul, 10, 20) << '\n';  // 200

    // Массив указателей на функции.
    BinaryOp ops[] = {add, mul};
    std::cout << "массив ops: ";
    for (auto op : ops) {
        std::cout << op(5, 3) << ' ';  // 8 15
    }
    std::cout << '\n';

    // --- std::function: хранит любой вызываемый объект ---
    std::function<int(int, int)> f = add;  // обычная функция
    std::cout << "f(3, 4) = " << f(3, 4) << '\n';  // 7

    f = [](int a, int b) { return a - b; };  // лямбда
    std::cout << "f(10, 3) = " << f(10, 3) << '\n';  // 7

    int factor = 10;
    std::function<int(int)> scaled = [factor](int x) { return x * factor; };  // захват
    std::cout << "scaled(5) = " << scaled(5) << '\n';  // 50

    // Проверка на пустоту.
    std::function<void()> empty;
    if (!empty) {
        std::cout << "empty пуста\n";
    }

    return 0;
}
