#include "safe_arith.h"

namespace arith {

std::optional<int> safe_divide(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a / b;
}

std::optional<int> safe_mod(int a, int b) {
    if (b == 0) {
        return std::nullopt;
    }
    return a % b;
}

int floor_mod(int a, int b) {
    int r = a % b;
    // Если знак остатка не совпадает со знаком делителя — корректируем.
    if ((r != 0) && ((r < 0) != (b < 0))) {
        r += b;
    }
    return r;
}

}  // namespace arith
