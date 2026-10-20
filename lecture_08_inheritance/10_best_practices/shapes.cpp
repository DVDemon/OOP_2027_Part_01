#include "shapes.h"

namespace shapes {

std::string Shape::info() const {
    return name() + " в (" + std::to_string(x_) + ", " + std::to_string(y_) +
           "), площадь=" + std::to_string(area()) +
           ", периметр=" + std::to_string(perimeter());
}

}  // namespace shapes
