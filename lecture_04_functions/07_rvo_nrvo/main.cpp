// Пример 07: возврат по значению, RVO и NRVO.
// Тип Tracker печатает, когда его копируют или перемещают, — так видно,
// сработала ли оптимизация (тогда лишних копий/перемещений не будет).
#include <iostream>
#include <string>
#include <utility>

struct Tracker {
    std::string name;

    explicit Tracker(std::string n) : name(std::move(n)) {
        std::cout << "  конструктор: " << name << '\n';
    }
    Tracker(const Tracker& o) : name(o.name) {
        std::cout << "  КОПИЯ: " << name << '\n';
    }
    Tracker(Tracker&& o) noexcept : name(std::move(o.name)) {
        std::cout << "  ПЕРЕМЕЩЕНИЕ: " << name << '\n';
    }
};

// RVO (гарантирована с C++17): возврат prvalue создаётся сразу в вызывающем коде.
Tracker make_prvalue() {
    return Tracker("prvalue");  // ни копии, ни перемещения
}

// NRVO: именованная локальная переменная. Не гарантирована стандартом,
// но реализуется всеми основными компиляторами.
Tracker make_named() {
    Tracker t("named");
    return t;  // обычно без копии/перемещения (NRVO)
}

// Анти-пример: std::move у локальной переменной ЗАПРЕЩАЕТ NRVO
// и навязывает перемещение. Компилятор сам предупреждает об этом
// (-Wpessimizing-move) — локально глушим, чтобы показать поведение в выводе.
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpessimizing-move"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpessimizing-move"
#endif
Tracker make_with_move() {
    Tracker t("moved");
    return std::move(t);  // ПЛОХО: вместо NRVO будет перемещение
}
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

int main() {
    std::cout << "make_prvalue():\n";
    Tracker a = make_prvalue();

    std::cout << "make_named():\n";
    Tracker b = make_named();

    std::cout << "make_with_move():\n";
    Tracker c = make_with_move();

    std::cout << "итог: " << a.name << ", " << b.name << ", " << c.name << '\n';
    return 0;
}
