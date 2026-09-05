#pragma once

namespace rec {

// --- Прямая рекурсия ---
// Факториал: базовый случай n <= 1, рекурсивный случай n * factorial(n-1).
long long factorial(int n);

// --- Хвостовая рекурсия ---
// Рекурсивный вызов — последняя операция; аккумулятор переносит результат.
// Компилятор (с -O2) может превратить такой вызов в цикл.
long long factorial_tail(int n, long long acc = 1);

// --- Взаимная рекурсия ---
// Объявления нужны, чтобы каждая функция «видела» другую.
bool is_even(unsigned n);
bool is_odd(unsigned n);

}  // namespace rec
