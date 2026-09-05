// Пример 02: копирующий конструктор и копирующее присваивание (deep copy).
//
// Класс владеет буфером и явно определяет:
//   - копирующий конструктор   -> выделяет НОВЫЙ буфер и копирует данные;
//   - копирующее присваивание  -> освобождает старое, копирует чужое.
// Теперь у каждого объекта свой буфер, double free невозможен.
#include <algorithm>
#include <cstring>
#include <iostream>

class Buffer {
public:
    explicit Buffer(const char* s)
        : size_(std::strlen(s)), data_(new char[size_ + 1]) {
        std::strcpy(data_, s);
    }

    ~Buffer() { delete[] data_; }

    // Копирующий конструктор — глубокая копия.
    Buffer(const Buffer& other)
        : size_(other.size_), data_(new char[other.size_ + 1]) {
        std::strcpy(data_, other.data_);
    }

    // Копирующее присваивание (безопасный порядок: сначала выделяем новое).
    Buffer& operator=(const Buffer& other) {
        if (this == &other) {
            return *this;  // защита от самоприсваивания
        }
        // Сначала выделяем новый буфер: если new бросит, *this не изменится.
        char* new_data = new char[other.size_ + 1];
        std::strcpy(new_data, other.data_);
        // Только теперь освобождаем старое и переключаемся.
        delete[] data_;
        data_ = new_data;
        size_ = other.size_;
        return *this;
    }

    const char* data() const { return data_; }
    const void* address() const { return static_cast<const void*>(data_); }

private:
    std::size_t size_;
    char* data_;
};

int main() {
    Buffer a("Hello");
    Buffer b = a;  // копирующий конструктор -> deep copy

    std::cout << "a: \"" << a.data() << "\" @ " << a.address() << '\n';
    std::cout << "b: \"" << b.data() << "\" @ " << b.address() << '\n';
    std::cout << "Буферы разные? " << std::boolalpha
              << (a.address() != b.address()) << '\n';

    Buffer c("temp");
    c = a;  // копирующее присваивание
    std::cout << "после c = a: c = \"" << c.data() << "\" @ " << c.address()
              << '\n';

    Buffer& alias = c;  // ссылка на c, чтобы безопасно проверить самоприсваивание
    c = alias;          // самоприсваивание — обрабатывается корректно
    std::cout << "после c = c (через ссылку): c = \"" << c.data() << "\"\n";

    return 0;
}
