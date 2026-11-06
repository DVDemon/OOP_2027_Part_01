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
    if (other.data_ != nullptr) {
        std::strcpy(data_, other.data_);
    } else {
        data_[0] = '\0';  // копирование moved-from объекта даёт пустую строку
    }
}

// 4. Перемещающий конструктор — крадёт буфер и оставляет источник валидным.
String::String(String&& other) noexcept
    : data_(std::exchange(other.data_, nullptr))
    , size_(std::exchange(other.size_, 0))
    , capacity_(std::exchange(other.capacity_, 0)) {
    // Источник остаётся валидной ПУСТОЙ строкой (data_ == nullptr, size_ == 0).
    // Никакой аллокации: move обязан быть дешёвым и noexcept — выделение памяти
    // здесь нарушило бы noexcept (new может бросить) и смысл перемещения.
    // c_str(), operator+= и operator== корректно обрабатывают nullptr.
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
        new_data[0] = '\0';
        if (data_ != nullptr) {
            std::strcpy(new_data, data_);  // data_ может быть nullptr после move
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
    if (other.data_ != nullptr) {
        std::strcat(data_, other.data_);  // other может быть moved-from (nullptr)
    }
    size_ = new_size;
    return *this;
}

// --- Свободные/дружественные операторы ---

String operator+(String lhs, const String& rhs) {
    lhs += rhs;  // lhs — копия (по значению), модифицируем её
    return lhs;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    return os << s.c_str();  // c_str() вернёт "" для moved-from (data_ == nullptr)
}

bool operator==(const String& a, const String& b) {
    if (a.size_ != b.size_) return false;
    if (a.size_ == 0) return true;  // обе пустые (в т.ч. moved-from с nullptr)
    return std::strcmp(a.data_, b.data_) == 0;
}

bool operator!=(const String& a, const String& b) { return !(a == b); }

}  // namespace lec07
