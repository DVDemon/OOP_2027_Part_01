// Определения функций, объявленных в math_utils.h.
// Это единственная единица трансляции, где они определены (ODR).
#include "math_utils.h"

namespace mu {

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

}  // namespace mu
