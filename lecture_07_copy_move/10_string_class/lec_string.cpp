#include "lec_string.h"

#include <algorithm>
#include <cstring>
#include <ostream>
#include <utility>

namespace lec07 {

// --- Конструкторы ---

String::String() : data_(new char[1]{'\0'}), size_(0), capacity_(0) {}

String::String(const char* s)
    : data_(nullptr), size_(std::strlen(s)), capacity_(size_) {
    data_ = new char[capacity_ + 1];
    std::strcpy(data_, s);
}

// --- Правило пяти ---

String::~String() { delete[] data_; }

// 2. Копирующий конструктор — глубокая копия.
String::String(const String& other)
    : data_(new char[other.capacity_ + 1])
    , size_(other.size_)
    , capacity_(other.capacity_) {
    std::strcpy(data_, other.data_);
}

// 4. Перемещающий конструктор — крадёт буфер и оставляет источник валидным.
String::String(String&& other) noexcept
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
    , capacity_(std::exchange(other.capacity_, 0)) {
    // Источник должен остаться в валидном состоянии (пустая строка).
    other.data_ = new char[1]{'\0'};
}

// 3 и 5. Присваивание по значению (copy-and-swap).
// Параметр other создаётся копирующим ИЛИ перемещающим конструктором —
// один оператор обслуживает оба случая.
String& String::operator=(String other) noexcept {
    swap(*this, other);
    return *this;
}

// --- Обмен ---

void swap(String& a, String& b) noexcept {
    using std::swap;
    swap(a.data_, b.data_);
    swap(a.size_, b.size_);
    swap(a.capacity_, b.capacity_);
}

// --- Методы ---

String& String::operator+=(const String& other) {
    const std::size_t new_size = size_ + other.size_;
    if (new_size > capacity_) {
        const std::size_t new_capacity = new_size * 2;
        char* new_data = new char[new_capacity + 1];
        std::strcpy(new_data, data_);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
    std::strcat(data_, other.data_);
    size_ = new_size;
    return *this;
}

// --- Свободные/дружественные операторы ---

String operator+(String lhs, const String& rhs) {
    lhs += rhs;  // lhs — копия (по значению), модифицируем её
    return lhs;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.data_;
}

bool operator==(const String& a, const String& b) {
    return a.size_ == b.size_ && std::strcmp(a.data_, b.data_) == 0;
}

bool operator!=(const String& a, const String& b) { return !(a == b); }

}  // namespace lec07
