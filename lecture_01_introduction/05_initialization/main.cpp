// Пример 05: формы инициализации переменных.
#include <iostream>
#include <string>

struct Widget {
    int value{0};   // инициализатор по умолчанию для члена
};

int main() {
    // --- Четыре формы инициализации ---
    int a = 42;     // copy initialization
    int b(42);      // direct initialization
    int c{42};      // direct-list-initialization (фигурная)
    int d = {42};   // copy-list-initialization
    int e{};        // value initialization -> гарантированно 0
    std::cout << "a=" << a << " b=" << b << " c=" << c
              << " d=" << d << " e=" << e << '\n';

    // --- Защита от сужения: фигурная инициализация ловит ошибку ---
    int ok = 3.14;     // copy init: тихое усечение до 3 (предупреждение, но компилируется)
    std::cout << "int ok = 3.14;  -> " << ok << "  (тихое усечение!)\n";
    // int bad{3.14};  // РАСКОММЕНТИРУЙТЕ: ошибка компиляции — сужающее преобразование

    // --- Most vexing parse ---
    // Widget w1();   // это ОБЪЯВЛЕНИЕ функции w1, возвращающей Widget!
    Widget w2{};      // а это создание объекта Widget
    std::cout << "w2.value = " << w2.value << '\n';

    std::string s = "hello";   // copy init для строки
    std::cout << "s = " << s << '\n';

    return 0;
}
