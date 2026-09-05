#include "calculator.h"

#include <limits>

namespace calc {

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

double divide(double a, double b) {
    if (b == 0.0) {
        return std::numeric_limits<double>::infinity();
    }
    return a / b;
}

// factorial определена как constexpr прямо в заголовке.

}  // namespace calc
