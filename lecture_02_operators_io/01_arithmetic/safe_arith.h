#pragma once

#include <optional>

namespace arith {

// Целочисленное деление с защитой от деления на ноль.
// Возвращает std::nullopt, если делитель равен нулю.
std::optional<int> safe_divide(int a, int b);

// Остаток от деления с защитой от деления на ноль.
std::optional<int> safe_mod(int a, int b);

// «Истинный» математический остаток (модуль), всегда неотрицательный
// для положительного делителя. В отличие от оператора %, который
// для отрицательных чисел даёт отрицательный остаток.
int floor_mod(int a, int b);

}  // namespace arith
