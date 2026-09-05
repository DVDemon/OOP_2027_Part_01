// Пример 08: пространства имён.
#include <iostream>

namespace math {
inline constexpr double pi = 3.14159265358979;
double square(double x) { return x * x; }
}  // namespace math

namespace physics {
inline constexpr double c = 299'792'458.0;  // скорость света, м/с
}  // namespace physics

int main() {
    // Полное квалифицированное имя — всегда однозначно:
    std::cout << "math::pi = " << math::pi << '\n';
    std::cout << "math::square(5) = " << math::square(5) << '\n';
    std::cout << "physics::c = " << physics::c << '\n';

    // using-объявление (предпочтительнее): вносим ровно одно имя.
    using math::square;
    std::cout << "square(3) = " << square(3) << '\n';

    // using-директива вносит ВСЕ имена namespace — допустимо локально в .cpp,
    // но НИКОГДА не в заголовке (загрязняет глобальное пространство имён).
    using namespace math;
    std::cout << "pi (через using namespace) = " << pi << '\n';

    return 0;
}
