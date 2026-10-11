#pragma once

#include <chrono>
#include <string>

// RAII-таймер: засекает время в конструкторе, печатает длительность в
// деструкторе. Захват «ресурса» (момента старта) — в конструкторе,
// освобождение/использование — в деструкторе.
class ScopedTimer {
    std::string label_;
    std::chrono::steady_clock::time_point start_;

public:
    explicit ScopedTimer(std::string label);
    ~ScopedTimer();

    // Таймер привязан к области видимости — копировать/перемещать нельзя.
    ScopedTimer(const ScopedTimer&) = delete;
    ScopedTimer& operator=(const ScopedTimer&) = delete;

    // Сколько прошло с момента старта, в миллисекундах.
    double elapsed_ms() const;
};
