#include "timer.h"

#include <iostream>
#include <utility>

ScopedTimer::ScopedTimer(std::string label)
    : label_(std::move(label)), start_(std::chrono::steady_clock::now()) {
    std::cout << "[" << label_ << "] таймер запущен\n";
}

ScopedTimer::~ScopedTimer() {
    std::cout << "[" << label_ << "] завершено за " << elapsed_ms()
              << " мс\n";
}

double ScopedTimer::elapsed_ms() const {
    const auto now = std::chrono::steady_clock::now();
    const std::chrono::duration<double, std::milli> diff = now - start_;
    return diff.count();
}
