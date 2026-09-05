#pragma once

#include <compare>
#include <cstdint>
#include <iostream>

// Деньги, хранящиеся в копейках (целое число) — чтобы избежать ошибок
// округления double. Демонстрирует канонические формы операторов вместе:
// +=/+, сравнение через <=>, вывод <<, explicit operator bool,
// и пользовательский литерал _rub.
class Money {
    std::int64_t cents_;  // сумма в копейках
public:
    constexpr explicit Money(std::int64_t cents = 0) : cents_(cents) {}

    constexpr std::int64_t cents() const { return cents_; }

    // Составной -> бинарный (канонический приём).
    constexpr Money& operator+=(const Money& rhs) {
        cents_ += rhs.cents_;
        return *this;
    }
    constexpr Money& operator-=(const Money& rhs) {
        cents_ -= rhs.cents_;
        return *this;
    }

    friend constexpr Money operator+(Money lhs, const Money& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend constexpr Money operator-(Money lhs, const Money& rhs) {
        lhs -= rhs;
        return lhs;
    }

    // Сравнение: один <=> заменяет все шесть операторов.
    auto operator<=>(const Money&) const = default;

    // explicit operator bool: ненулевая сумма.
    constexpr explicit operator bool() const { return cents_ != 0; }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        os << (m.cents_ / 100) << '.';
        std::int64_t k = m.cents_ % 100;
        if (k < 10) os << '0';
        return os << k << " руб.";
    }
};

// --- Пользовательские литералы (должны начинаться с _) ---
// 100.0_rub -> Money(10000 копеек)
constexpr Money operator""_rub(long double rubles) {
    return Money(static_cast<std::int64_t>(rubles * 100));
}
// 100_rub (целочисленный литерал) -> Money(10000 копеек)
constexpr Money operator""_rub(unsigned long long rubles) {
    return Money(static_cast<std::int64_t>(rubles * 100));
}
