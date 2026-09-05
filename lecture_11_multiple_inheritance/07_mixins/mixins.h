#pragma once

// Примеси (mixins) через CRTP (Curiously Recurring Template Pattern).
//
// Примесь добавляет функциональность производному классу, не создавая
// отношения is-a и без рантайм-затрат (нет виртуальных вызовов).
// Базовый шаблон статически приводит *this к производному типу T.

#include <iostream>
#include <ostream>

// Примесь печати: требует у T метод toString().
template <typename T>
class Printable {
public:
    void print(std::ostream& os = std::cout) const {
        os << static_cast<const T&>(*this).toString() << '\n';
    }
};

// Примесь сравнения: требует у T операторы == и <,
// и достраивает на их основе !=, >, <=, >=.
template <typename T>
class Comparable {
public:
    bool operator!=(const T& other) const {
        return !(static_cast<const T&>(*this) == other);
    }
    bool operator>(const T& other) const {
        return other < static_cast<const T&>(*this);
    }
    bool operator<=(const T& other) const {
        return !(static_cast<const T&>(*this) > other);
    }
    bool operator>=(const T& other) const {
        return !(static_cast<const T&>(*this) < other);
    }
};
