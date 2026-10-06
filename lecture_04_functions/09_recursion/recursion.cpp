// Определения рекурсивных функций.
#include "recursion.h"

namespace rec {

long long factorial(int n) {
    if (n <= 1) return 1;          // базовый случай
    return n * factorial(n - 1);   // рекурсивный случай
}

long long factorial_tail(int n, long long acc) {
    if (n <= 1) return acc;                       // базовый случай
    return factorial_tail(n - 1, n * acc);        // хвостовой вызов
}

bool is_even(unsigned n) {
    if (n == 0) return true;
    return is_odd(n - 1);
}

bool is_odd(unsigned n) {
    if (n == 0) return false;
    return is_even(n - 1);
}

}  // namespace rec
