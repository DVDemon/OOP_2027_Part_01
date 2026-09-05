#pragma once

namespace mathx {

// Сравнение двух double с абсолютным допуском.
// НЕ сравнивайте числа с плавающей точкой через == — используйте эту функцию.
bool almost_equal(double a, double b, double eps = 1e-9);

// Гипотенуза по теореме Пифагора (через std::hypot, устойчиво к переполнению).
double hypotenuse(double x, double y);

}  // namespace mathx
