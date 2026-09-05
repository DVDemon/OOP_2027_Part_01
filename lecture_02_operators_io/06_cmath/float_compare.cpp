#include "float_compare.h"

#include <cmath>

namespace mathx {

bool almost_equal(double a, double b, double eps) {
    return std::abs(a - b) < eps;
}

double hypotenuse(double x, double y) {
    return std::hypot(x, y);
}

}  // namespace mathx
