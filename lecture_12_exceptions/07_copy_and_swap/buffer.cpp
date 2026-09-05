#include "buffer.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

Buffer::Buffer(std::size_t n)
    : size_(n), data_(n ? new int[n]{} : nullptr) {}

Buffer::Buffer(const Buffer& other)
    : size_(other.size_),
      data_(other.size_ ? new int[other.size_] : nullptr) {
    std::copy(other.data_, other.data_ + size_, data_);
}

Buffer::Buffer(Buffer&& other) noexcept
    : size_(other.size_), data_(other.data_) {
    other.size_ = 0;
    other.data_ = nullptr;
}

Buffer::~Buffer() {
    delete[] data_;
}

// other уже создан (копированием или перемещением) до входа в тело функции.
// Если копирование параметра бросило — мы сюда даже не попали, *this не тронут.
// swap не бросает, поэтому присваивание целиком даёт строгую гарантию.
Buffer& Buffer::operator=(Buffer other) noexcept {
    swap(*this, other);
    return *this;
    // other (теперь со старым содержимым *this) разрушится здесь.
}

void swap(Buffer& a, Buffer& b) noexcept {
    using std::swap;
    swap(a.size_, b.size_);
    swap(a.data_, b.data_);
}

int& Buffer::at(std::size_t i) {
    if (i >= size_)
        throw std::out_of_range("Buffer::at: индекс вне диапазона");
    return data_[i];
}

int Buffer::at(std::size_t i) const {
    if (i >= size_)
        throw std::out_of_range("Buffer::at: индекс вне диапазона");
    return data_[i];
}
