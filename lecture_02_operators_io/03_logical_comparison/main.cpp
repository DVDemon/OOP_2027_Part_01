// Пример 03: логические операторы и операторы сравнения.
#include <iostream>

// Функция с побочным эффектом — чтобы увидеть short-circuit «вживую».
bool expensive_check(const char* name, bool result) {
    std::cout << "  [вычисляется " << name << "]\n";
    return result;
}

int main() {
    std::cout << std::boolalpha;

    std::cout << "=== Логические операторы ===\n";
    bool a = true, b = false;
    std::cout << "a && b = " << (a && b) << '\n';  // false
    std::cout << "a || b = " << (a || b) << '\n';  // true
    std::cout << "!a     = " << (!a) << '\n';      // false

    std::cout << "\n=== Операторы сравнения ===\n";
    int x = 10, y = 20;
    std::cout << "x == y : " << (x == y) << '\n';  // false
    std::cout << "x != y : " << (x != y) << '\n';  // true
    std::cout << "x <  y : " << (x < y) << '\n';   // true
    std::cout << "x >= y : " << (x >= y) << '\n';  // false

    std::cout << "\n=== Short-circuit для && ===\n";
    // Правый операнд НЕ вычисляется, если левый уже определил результат.
    std::cout << "false && expensive_check:\n";
    bool r1 = expensive_check("left", false) && expensive_check("right", true);
    std::cout << "результат = " << r1 << " (right не вычислялся)\n";

    std::cout << "\n=== Short-circuit для || ===\n";
    std::cout << "true || expensive_check:\n";
    bool r2 = expensive_check("left", true) || expensive_check("right", false);
    std::cout << "результат = " << r2 << " (right не вычислялся)\n";

    std::cout << "\n=== Практика: безопасный доступ по указателю ===\n";
    int value = 42;
    int* ptr = &value;
    // ХОРОШО: благодаря short-circuit *ptr читается только если ptr != nullptr.
    if (ptr != nullptr && *ptr > 0) {
        std::cout << "*ptr = " << *ptr << " (доступ безопасен)\n";
    }
    ptr = nullptr;
    if (ptr != nullptr && *ptr > 0) {
        std::cout << "не сюда\n";
    } else {
        std::cout << "ptr == nullptr: разыменование не произошло\n";
    }

    std::cout << "\n=== Ловушка: = вместо == ===\n";
    int n = 5;
    // ПЛОХО: if (n = 0) — это присваивание, условие всегда вычисляется по значению.
    // ХОРОШО: сравнение через ==.
    if (n == 5) {
        std::cout << "n действительно равно 5\n";
    }

    return 0;
}
