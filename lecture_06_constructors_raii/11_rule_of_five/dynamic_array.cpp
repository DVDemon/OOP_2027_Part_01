#include "dynamic_array.h"

#include <algorithm>
#include <stdexcept>

DynamicArray::DynamicArray() : data_(nullptr), size_(0), capacity_(0) {}

DynamicArray::DynamicArray(std::size_t capacity)
    : data_(capacity > 0 ? new int[capacity]{} : nullptr),
      size_(0),
      capacity_(capacity) {}

// Делегируем к DynamicArray(capacity), затем заполняем.
DynamicArray::DynamicArray(std::size_t count, int value)
    : DynamicArray(count) {
    for (std::size_t i = 0; i < count; ++i) {
        data_[i] = value;
    }
    size_ = count;
}

DynamicArray::DynamicArray(std::initializer_list<int> values)
    : DynamicArray(values.size()) {
    std::copy(values.begin(), values.end(), data_);
    size_ = values.size();
}

// Глубокая копия: выделяем собственный буфер и копируем элементы.
DynamicArray::DynamicArray(const DynamicArray& other)
    : data_(other.capacity_ > 0 ? new int[other.capacity_]{} : nullptr),
      size_(other.size_),
      capacity_(other.capacity_) {
    std::copy(other.data_, other.data_ + other.size_, data_);
}

DynamicArray::~DynamicArray() {
    delete[] data_;
}

void DynamicArray::push_back(int value) {
    if (size_ == capacity_) {
        const std::size_t new_capacity = capacity_ == 0 ? 1 : capacity_ * 2;
        int* new_data = new int[new_capacity]{};
        std::copy(data_, data_ + size_, new_data);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
    data_[size_] = value;
    ++size_;
}

int DynamicArray::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("DynamicArray::at: индекс вне диапазона");
    }
    return data_[index];
}
