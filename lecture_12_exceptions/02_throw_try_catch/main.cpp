// Пример 02: синтаксис throw / try / catch.
//
// Показываем порядок catch-блоков (от конкретного к общему) и catch(...).
#include <iostream>
#include <stdexcept>

double divide(double a, double b) {
    if (b == 0.0)
        throw std::invalid_argument("Деление на ноль");
    return a / b;
}

// Вспомогательная функция: ловит исключения от divide и печатает результат.
void try_divide(double a, double b) {
    try {
        double result = divide(a, b);
        std::cout << a << " / " << b << " = " << result << '\n';
    }
    // Порядок важен: сначала наиболее конкретный тип...
    catch (const std::invalid_argument& e) {
        std::cout << "Ошибка аргумента: " << e.what() << '\n';
    }
    // ...затем более общий базовый класс...
    catch (const std::exception& e) {
        std::cout << "Общая ошибка: " << e.what() << '\n';
    }
    // ...и наконец «ловушка» для всего остального.
    catch (...) {
        std::cout << "Неизвестное исключение\n";
    }
}

int main() {
    try_divide(10.0, 2.0);   // успех
    try_divide(10.0, 0.0);   // invalid_argument
    return 0;
}
