#include "tracked_buffer.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <utility>

TrackedBuffer::TrackedBuffer(std::string name, std::size_t size, int fill)
    : data_(size > 0 ? new int[size]{} : nullptr),
      size_(size),
      name_(std::move(name)) {
    std::fill(data_, data_ + size_, fill);
    std::cout << "  TrackedBuffer(\"" << name_ << "\", " << size_
              << ") — конструктор\n";
}

// Деструктор вызывается и для «пустого» (перемещённого) объекта —
// delete[] nullptr безопасен.
TrackedBuffer::~TrackedBuffer() {
    std::cout << "  ~TrackedBuffer(\"" << name_ << "\") — деструктор ("
              << size_ << " эл.)\n";
    delete[] data_;
}

// Копирующий конструктор: собственный буфер + почленная копия элементов.
TrackedBuffer::TrackedBuffer(const TrackedBuffer& other)
    : data_(other.size_ > 0 ? new int[other.size_]{} : nullptr),
      size_(other.size_),
      name_(other.name_ + "(копия)") {
    std::copy(other.data_, other.data_ + other.size_, data_);
    std::cout << "  TrackedBuffer(const TrackedBuffer&) — КОПИРУЮЩИЙ "
                 "конструктор из \""
              << other.name_ << "\"\n";
}

// Копирующее присваивание через copy-and-swap:
//   1) делаем полную копию other (может бросить — исходный объект цел);
//   2) обмениваемся с копией;
//   3) временный объект уничтожается и освобождает старый ресурс.
// Сильная гарантия исключений + корректность при самоприсваивании.
TrackedBuffer& TrackedBuffer::operator=(const TrackedBuffer& other) {
    std::cout << "  operator=(const TrackedBuffer&) — КОПИРУЮЩЕЕ "
                 "присваивание: \""
              << name_ << "\" <- \"" << other.name_ << "\"\n";
    if (this != &other) {
        TrackedBuffer tmp(other);  // глубокая копия
        swap(tmp);                 // обмен; tmp освободит старый буфер
    }
    return *this;
}

// Перемещающий конструктор: забираем указатель, источник обнуляем,
// чтобы его деструктор ничего не освободил.
TrackedBuffer::TrackedBuffer(TrackedBuffer&& other) noexcept
    : data_(std::exchange(other.data_, nullptr)),
      size_(std::exchange(other.size_, 0)),
      name_(std::move(other.name_)) {
    other.name_ = "(перемещён)";
    std::cout << "  TrackedBuffer(TrackedBuffer&&) — ПЕРЕМЕЩАЮЩИЙ "
                 "конструктор\n";
}

// Перемещающее присваивание: освобождаем свой ресурс и забираем чужой.
// Проверка this != &other защищает от самоперемещения.
TrackedBuffer& TrackedBuffer::operator=(TrackedBuffer&& other) noexcept {
    std::cout << "  operator=(TrackedBuffer&&) — ПЕРЕМЕЩАЮЩЕЕ присваивание: "
                 "\""
              << name_ << "\" <- \"" << other.name_ << "\"\n";
    if (this != &other) {
        delete[] data_;  // освобождаем текущий ресурс
        data_ = std::exchange(other.data_, nullptr);
        size_ = std::exchange(other.size_, 0);
        name_ = std::move(other.name_);
        other.name_ = "(перемещён)";
    }
    return *this;
}

void TrackedBuffer::swap(TrackedBuffer& other) noexcept {
    using std::swap;
    swap(data_, other.data_);
    swap(size_, other.size_);
    swap(name_, other.name_);
}

int TrackedBuffer::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("TrackedBuffer::at: индекс вне диапазона");
    }
    return data_[index];
}
