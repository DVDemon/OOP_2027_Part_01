#include "car.h"

namespace vehicle {

std::string Engine::ignite() const {
    return "двигатель запущен (" + std::to_string(power_) + " л.с.)";
}

std::string Car::start() const {
    return model_ + ": " + engine_.ignite();
}

int Car::horsepower() const noexcept {
    return engine_.power();
}

}  // namespace vehicle
