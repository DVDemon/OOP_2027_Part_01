// Пример 11: конструктор с std::initializer_list.
//
// std::initializer_list<T> — лёгкий «вид» на временный массив, который
// компилятор строит из списка в фигурных скобках { ... }. Он даёт только
// begin()/end()/size(), а элементы доступны ТОЛЬКО на чтение. Это самый
// удобный способ сконструировать контейнер сразу набором значений:
//
//   DynArray a{10, 20, 30};   // initializer_list -> копируем в свой буфер
//
// Важно: класс по-прежнему владеет буфером, поэтому правило пяти никто не
// отменяет — конструктор из il просто дополняет набор способов создания.
#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <utility>

class DynArray {
public:
    // Конструктор из std::initializer_list: узнаём размер и копируем элементы.
    DynArray(std::initializer_list<int> il)
        : data_(new int[il.size()]), size_(il.size()) {
        std::copy(il.begin(), il.end(), data_);
    }

    // Конструктор «n элементов» (значение по умолчанию — нули).
    explicit DynArray(std::size_t n) : data_(new int[n]{}), size_(n) {}

    // --- Правило пяти (владение буфером требует полного набора) ---
    ~DynArray() { delete[] data_; }                       // 1

    DynArray(const DynArray& other)                       // 2
        : data_(new int[other.size_]), size_(other.size_) {
        std::copy(other.data_, other.data_ + size_, data_);
    }

    DynArray& operator=(const DynArray& other) {          // 3
        if (this != &other) {
            int* tmp = new int[other.size_];
            std::copy(other.data_, other.data_ + other.size_, tmp);
            delete[] data_;
            data_ = tmp;
            size_ = other.size_;
        }
        return *this;
    }

    DynArray(DynArray&& other) noexcept                   // 4
        : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    DynArray& operator=(DynArray&& other) noexcept {      // 5
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    std::size_t size() const { return size_; }
    int operator[](std::size_t i) const { return data_[i]; }
    int& operator[](std::size_t i) { return data_[i]; }

private:
    int* data_;
    std::size_t size_;
};

std::ostream& operator<<(std::ostream& os, const DynArray& a) {
    os << '{';
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (i) os << ", ";
        os << a[i];
    }
    return os << '}';
}

int main() {
    DynArray a{10, 20, 30, 40};  // конструктор из initializer_list
    std::cout << "a{10,20,30,40} = " << a << "  (size = " << a.size() << ")\n";

    DynArray b = {1, 2, 3};      // copy-list-initialization — тоже il
    std::cout << "b = {1,2,3}   = " << b << '\n';

    DynArray c(4);               // конструктор «n элементов» (нули)
    std::cout << "c(4)         = " << c << '\n';

    // Подвох: DynArray{5} — это initializer_list из ОДНОГО элемента,
    // а DynArray(5) — «5 элементов». Скобки vs фигурные скобки имеют значение.
    DynArray one_element{5};
    DynArray five_elements(5);
    std::cout << "DynArray{5}  = " << one_element << "  (один элемент)\n";
    std::cout << "DynArray(5)  = " << five_elements << "  (пять нулей)\n";

    DynArray d = a;              // копирование — буферы независимы
    d[0] = -1;
    std::cout << "\nкопия независима: a = " << a << ", d = " << d << '\n';

    DynArray e = std::move(d);   // перемещение — источник опустошён
    std::cout << "после move: e = " << e << ", d.size() = " << d.size() << '\n';

    return 0;
}
