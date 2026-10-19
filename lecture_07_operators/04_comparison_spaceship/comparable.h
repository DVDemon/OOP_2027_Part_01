#pragma once

#include <compare>
#include <cstddef>
#include <string>

// --- Date: оператор <=> по умолчанию ---
// Одна строка "= default" генерирует ВСЕ шесть операторов сравнения.
// Сравнение лексикографическое: сначала year_, потом month_, потом day_.
class Date {
    int year_;
    int month_;
    int day_;
public:
    Date(int y, int m, int d) : year_(y), month_(m), day_(d) {}

    // Генерирует <, <=, >, >=, а также == и != (вместе с дефолтным ==).
    auto operator<=>(const Date&) const = default;
};

// --- CaseInsensitiveString: пользовательский <=> ---
// Когда нужна нестандартная логика сравнения — пишем <=> вручную.
// При пользовательском <=> оператор == нужно определять ОТДЕЛЬНО.
class CaseInsensitiveString {
    std::string data_;

    static char lower(char c) {
        // Приводим к нижнему регистру без зависимости от локали.
        return (c >= 'A' && c <= 'Z') ? static_cast<char>(c - 'A' + 'a') : c;
    }

public:
    explicit CaseInsensitiveString(std::string s) : data_(std::move(s)) {}

    const std::string& str() const { return data_; }

    std::strong_ordering operator<=>(const CaseInsensitiveString& other) const {
        auto it1 = data_.begin();
        auto it2 = other.data_.begin();
        for (; it1 != data_.end() && it2 != other.data_.end(); ++it1, ++it2) {
            char c1 = lower(*it1);
            char c2 = lower(*it2);
            if (c1 < c2) return std::strong_ordering::less;
            if (c1 > c2) return std::strong_ordering::greater;
        }
        // Все общие символы равны — решает длина.
        return data_.size() <=> other.data_.size();
    }

    bool operator==(const CaseInsensitiveString& other) const {
        return (*this <=> other) == std::strong_ordering::equal;
    }
};
