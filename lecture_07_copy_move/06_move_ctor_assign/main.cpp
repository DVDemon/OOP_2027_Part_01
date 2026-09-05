// Пример 06: перемещающий конструктор и перемещающее присваивание.
//
// Перемещение «крадёт» ресурс у источника вместо его копирования:
//   - копируется указатель (дёшево), а не данные;
//   - источник обнуляется и остаётся в валидном, но неопределённом состоянии.
// Счётчики позволяют увидеть, сколько раз выделялась память.
#include <cstring>
#include <iostream>
#include <utility>

class Resource {
public:
    explicit Resource(const char* s)
        : size_(std::strlen(s)), data_(new char[size_ + 1]) {
        std::strcpy(data_, s);
        ++allocations_;
    }

    ~Resource() { delete[] data_; }

    // Копирующий конструктор — выделяет новую память (дорого).
    Resource(const Resource& other)
        : size_(other.size_), data_(new char[other.size_ + 1]) {
        std::strcpy(data_, other.data_);
        ++allocations_;
        std::cout << "  [копирование] выделен новый буфер\n";
    }

    // Перемещающий конструктор — крадёт буфер, новой памяти не выделяет.
    Resource(Resource&& other) noexcept
        : size_(other.size_), data_(other.data_) {
        other.data_ = nullptr;  // обнуляем источник
        other.size_ = 0;
        std::cout << "  [перемещение] буфер украден, без выделения\n";
    }

    // Перемещающее присваивание.
    Resource& operator=(Resource&& other) noexcept {
        if (this != &other) {
            delete[] data_;        // освободить свой ресурс
            data_ = other.data_;   // украсть чужой
            size_ = other.size_;
            other.data_ = nullptr;  // обнулить источник
            other.size_ = 0;
        }
        return *this;
    }

    // Копирующее присваивание (для полноты Rule of Five).
    Resource& operator=(const Resource& other) {
        if (this != &other) {
            char* new_data = new char[other.size_ + 1];
            std::strcpy(new_data, other.data_);
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
            ++allocations_;
        }
        return *this;
    }

    const char* data() const { return data_ ? data_ : "<пусто>"; }
    static int allocations() { return allocations_; }

private:
    std::size_t size_;
    char* data_;
    static inline int allocations_ = 0;
};

int main() {
    Resource a("Hello");
    std::cout << "Создание копии b из a:\n";
    Resource b = a;  // копирующий конструктор
    std::cout << "Создание c из std::move(a):\n";
    Resource c = std::move(a);  // перемещающий конструктор

    std::cout << "\nb = \"" << b.data() << "\"\n";
    std::cout << "c = \"" << c.data() << "\"\n";
    std::cout << "a после move = \"" << a.data()
              << "\" (валидное, но неопределённое состояние)\n";

    Resource d("temp");
    d = std::move(b);  // перемещающее присваивание
    std::cout << "\nd после d = std::move(b): \"" << d.data() << "\"\n";

    std::cout << "\nВсего выделений памяти: " << Resource::allocations()
              << " (перемещения память не выделяют)\n";
    return 0;
}
