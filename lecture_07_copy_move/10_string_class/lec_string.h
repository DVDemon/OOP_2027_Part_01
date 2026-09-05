#pragma once

#include <cstddef>
#include <iosfwd>

namespace lec07 {

// Учебный класс String с управлением динамической памятью.
// Демонстрирует полное правило пяти, идиому copy-and-swap и noexcept-move.
class String {
public:
    // --- Конструкторы ---
    String();                 // пустая строка
    String(const char* s);    // из C-строки (намеренно НЕ explicit: удобство в примерах)

    // --- Правило пяти ---
    ~String();                                   // 1. деструктор
    String(const String& other);                 // 2. копирующий конструктор
    String(String&& other) noexcept;             // 4. перемещающий конструктор
    // 3 и 5: одно присваивание по значению (copy-and-swap) обслуживает оба.
    String& operator=(String other) noexcept;

    // --- Обмен (для copy-and-swap) ---
    friend void swap(String& a, String& b) noexcept;

    // --- Методы ---
    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }
    const char* c_str() const { return data_; }
    bool empty() const { return size_ == 0; }

    char& operator[](std::size_t i) { return data_[i]; }
    char operator[](std::size_t i) const { return data_[i]; }

    String& operator+=(const String& other);

    // --- Свободные/дружественные операторы ---
    friend String operator+(String lhs, const String& rhs);
    friend std::ostream& operator<<(std::ostream& os, const String& s);
    friend bool operator==(const String& a, const String& b);
    friend bool operator!=(const String& a, const String& b);

private:
    char* data_;
    std::size_t size_;
    std::size_t capacity_;  // вместимость без учёта завершающего '\0'
};

}  // namespace lec07
