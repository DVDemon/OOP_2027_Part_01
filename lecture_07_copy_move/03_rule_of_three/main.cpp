// Пример 03: правило трёх (Rule of Three).
//
// Если классу нужен ХОТЯ БЫ ОДИН из трёх специальных методов:
//   1) деструктор,
//   2) копирующий конструктор,
//   3) копирующее присваивание,
// то почти наверняка нужны ВСЕ ТРИ. Класс IntArray владеет динамическим
// массивом, поэтому определяет все три согласованно.
#include <algorithm>
#include <iostream>

class IntArray {
public:
    explicit IntArray(std::size_t n) : data_(new int[n]{}), size_(n) {}

    // 1. Деструктор.
    ~IntArray() { delete[] data_; }

    // 2. Копирующий конструктор.
    IntArray(const IntArray& other)
        : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    // 3. Копирующее присваивание.
    IntArray& operator=(const IntArray& other) {
        if (this != &other) {
            int* new_data = new int[other.size_];  // сначала выделяем
            std::copy(other.data_, other.data_ + other.size_, new_data);
            delete[] data_;                         // потом освобождаем
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }

    std::size_t size() const { return size_; }
    int& operator[](std::size_t i) { return data_[i]; }
    int operator[](std::size_t i) const { return data_[i]; }

private:
    int* data_;
    std::size_t size_;
};

int main() {
    IntArray a(3);
    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    IntArray b = a;  // копирующий конструктор
    b[0] = 99;       // меняем копию

    std::cout << "a: ";
    for (std::size_t i = 0; i < a.size(); ++i) std::cout << a[i] << ' ';
    std::cout << "\nb: ";
    for (std::size_t i = 0; i < b.size(); ++i) std::cout << b[i] << ' ';
    std::cout << "\n(копия независима: a[0] не изменился)\n";

    IntArray c(1);
    c = a;  // копирующее присваивание
    std::cout << "c после c = a: ";
    for (std::size_t i = 0; i < c.size(); ++i) std::cout << c[i] << ' ';
    std::cout << '\n';

    return 0;
}
