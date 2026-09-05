// Пример 08: условные конструкции if (включая init-if) и switch.
#include <iostream>
#include <string>

enum class Color { Red, Green, Blue };

const char* color_name(Color c) {
    switch (c) {
        case Color::Red:   return "Красный";
        case Color::Green: return "Зелёный";
        case Color::Blue:  return "Синий";
    }
    return "Неизвестный";  // на случай некорректного значения
}

// Классификация символа со ВНИМАНИЕМ к намеренному fall-through.
const char* classify(char ch) {
    switch (ch) {
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
            return "гласная";  // несколько case подряд — это нормально
        case '0':
            std::cout << "  (ноль — частный случай цифры)\n";
            [[fallthrough]];   // ЯВНО разрешаем провалиться в следующий case
        case '1':
        case '2':
            return "цифра";
        default:
            return "прочее";
    }
}

int main() {
    std::cout << "=== if / else if / else ===\n";
    int score = 85;
    if (score >= 90) {
        std::cout << "Отлично\n";
    } else if (score >= 75) {
        std::cout << "Хорошо\n";
    } else if (score >= 60) {
        std::cout << "Удовлетворительно\n";
    } else {
        std::cout << "Неудовлетворительно\n";
    }

    std::cout << "\n=== Enum class ===\n";
    Color a {Color::Red};
    std::cout << "Color = " << color_name(a) << "\n";


    std::cout << "\n=== Range for ===\n";

    for(char c : "abs012#")
     std::cout << "char " << c << " " << classify(c) << "\n";

    std::cout << "\n=== switch с инициализатором (C++17) ===\n";
    switch (int val = score % 3; val) {
        case 0:  std::cout << "остаток 0\n"; break;
        case 1:  std::cout << "остаток 1\n"; break;
        default: std::cout << "остаток 2\n"; break;
    }

    return 0;
}
