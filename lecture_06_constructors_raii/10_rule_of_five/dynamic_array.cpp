#include "dynamic_array.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <utility>

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
    std::cout << "del" << '\n';
    delete[] data_;
}

// Конструктор перемещения: забирает ресурсы из source, обнуляя его.
DynamicArray::DynamicArray(DynamicArray&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    std::cout << "move ctor\n";
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

// Обмен ресурсами за O(1). Copy-and-swap обменивается полями-владельцами,
// а НЕ вызывает operator= — иначе std::swap(*this, other) ушёл бы в
// присваивание и получилась бы бесконечная рекурсия.
void DynamicArray::swap(DynamicArray& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

// Оператор присваивания (copy-and-swap). Параметр other уже построен: для lvalue-аргумента это
// глубокая копия, для rvalue — перемещение. Обмениваемся с ним ресурсами;
// при выходе временный other освобождает прежнее содержимое *this.
// Проверка на самоприсваивание не нужна: other никогда не совпадает с *this.
DynamicArray& DynamicArray::operator=(DynamicArray other) {
    std::cout << "copy and swap \n";
    swap(other);
    return *this;
}

// Оператор перемещения
// DynamicArray& DynamicArray::operator=(DynamicArray&& other) noexcept {
//     std::cout << "move \n";
//     if (this != &other) {
//             delete[] data_;
//             data_ = other.data_;
//             size_ = other.size_;
//             capacity_ = other.capacity_;

//             other.data_ = nullptr;
//             other.size_ = 0;
//             other.capacity_ = 0;
//         }
//     return *this;
// }


// DynamicArray&  DynamicArray::operator=(const DynamicArray& other) {
//     if(this != &other) {
//        data_ = other.capacity_ > 0 ? new int[other.capacity_]{} : nullptr;
//        size_ = other.size_;
//        capacity_ = other.capacity_;

//     }

//     return *this;
// }

// Определение свободного swap (объявлен в заголовке) — для ADL.
void swap(DynamicArray& a, DynamicArray& b) noexcept {
    a.swap(b);
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
