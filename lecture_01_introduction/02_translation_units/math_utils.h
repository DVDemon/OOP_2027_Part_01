// Заголовочный файл: только ОБЪЯВЛЕНИЯ (declarations).
// Защита от повторного включения — #pragma once.
#pragma once

namespace math_utils {

// Объявление функции (без тела). Определение — в math_utils.cpp.
int add(int a, int b);

// inline-функцию МОЖНО определять прямо в заголовке: её определение
// разрешено повторять в нескольких единицах трансляции (исключение из ODR),
// но все определения обязаны быть идентичными.
inline int square(int x) {
    return x * x;
}

}  // namespace math_utils
