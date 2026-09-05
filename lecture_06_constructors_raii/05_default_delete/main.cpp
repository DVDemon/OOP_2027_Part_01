// Пример 05: =default, =delete и контроль автогенерации спец-методов.
#include <iostream>
#include <utility>

// Класс, копирование которого запрещено, а перемещение — разрешено.
class Buffer {
    int* data_;
    std::size_t size_;

public:
    explicit Buffer(std::size_t size)
        : data_(new int[size]{}), size_(size) {
        std::cout << "Buffer(" << size_ << ") выделил память\n";
    }

    // Просим компилятор сгенерировать деструктор по умолчанию недостаточно —
    // нужен ручной, так как управляем new[]. Покажем ниже явно.
    ~Buffer() {
        delete[] data_;
        std::cout << "~Buffer() освободил память (" << size_ << ")\n";
    }

    // Запрещаем копирование: глубокую копию здесь не делаем.
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    // Разрешаем перемещение, реализуем вручную (нельзя =default из-за указателя:
    // default переместил бы указатель без обнуления исходного).
    Buffer(Buffer&& other) noexcept
        : data_(std::exchange(other.data_, nullptr)),
          size_(std::exchange(other.size_, 0)) {
        std::cout << "Buffer(Buffer&&) перемещение\n";
    }
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
        }
        return *this;
    }

    std::size_t size() const { return size_; }
};

// Класс-агрегат: явно просим стандартные спец-методы через =default.
class Pod {
    int x_ = 0;
    int y_ = 0;

public:
    Pod() = default;                          // конструктор по умолчанию
    Pod(int x, int y) : x_(x), y_(y) {}
    Pod(const Pod&) = default;                // копирование — стандартное
    Pod& operator=(const Pod&) = default;
    int x() const { return x_; }
    int y() const { return y_; }
};

int main() {
    std::cout << "--- =delete: копирование запрещено ---\n";
    Buffer b1(4);
    // Buffer b2 = b1;       // ОШИБКА: копирующий конструктор удалён
    Buffer b2 = std::move(b1);  // OK: перемещение
    std::cout << "b2.size() = " << b2.size()
              << ", b1.size() = " << b1.size() << " (опустошён)\n";

    std::cout << "--- =default: стандартное копирование ---\n";
    Pod p1(1, 2);
    Pod p2 = p1;  // стандартное почленное копирование
    std::cout << "p2 = (" << p2.x() << ", " << p2.y() << ")\n";

    return 0;
}
