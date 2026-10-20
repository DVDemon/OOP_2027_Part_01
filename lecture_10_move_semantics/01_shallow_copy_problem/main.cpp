// Пример 01: проблема поверхностного копирования (shallow copy).
//
// Класс владеет динамическим буфером через указатель. Если положиться на
// сгенерированный компилятором копирующий конструктор, он скопирует ЗНАЧЕНИЕ
// указателя — два объекта будут владеть одним буфером (double free).
//
// Чтобы продемонстрировать проблему БЕЗОПАСНО (без реального краша), мы НЕ
// удаляем буфер в деструкторе, а лишь печатаем адрес и явно показываем, что
// два объекта указывают на одну и ту же память.
#include <cstring>
#include <iostream>

// NaiveString с поверхностным копированием (специально без deep copy).
class NaiveString {
public:
    explicit NaiveString(const char* s)
        : size_(std::strlen(s)), data_(new char[size_ + 1]) {
        std::strcpy(data_, s);
    }

    // ВНИМАНИЕ: копирующий конструктор и присваивание НЕ определены, поэтому
    // компилятор сгенерирует поверхностные версии:
    //   NaiveString(const NaiveString& o) : size_(o.size_), data_(o.data_) {}
    // То есть data_ просто копируется как указатель.

    // Деструктор намеренно НЕ вызывает delete[], чтобы пример не падал при
    // double free. В реальном классе деструктор был бы { delete[] data_; }
    // и два разрушения одного буфера дали бы неопределённое поведение.
    ~NaiveString() = default;

    const char* data() const { return data_; }
    const void* address() const { return static_cast<const void*>(data_); }

    // Освободить буфер вручную (так как деструктор этого не делает).
    void release() { delete[] data_; data_ = nullptr; }

private:
    std::size_t size_;
    char* data_;
};

int main() {
    NaiveString a("Hello");
    NaiveString b = a;  // поверхностное копирование!

    std::cout << "a.data = \"" << a.data() << "\", буфер по адресу "
              << a.address() << '\n';
    std::cout << "b.data = \"" << b.data() << "\", буфер по адресу "
              << b.address() << '\n';

    if (a.address() == b.address()) {
        std::cout << "ПРОБЛЕМА: a и b владеют ОДНИМ буфером.\n";
        std::cout << "С деструктором { delete[] data_; } это вызвало бы "
                     "double free (UB).\n";
    }

    // Освобождаем буфер ровно один раз (через a). Освобождать b нельзя —
    // это и есть та самая двойная очистка.
    a.release();

    std::cout << "\nРешение — глубокое копирование (deep copy), "
                 "см. пример 02.\n";
    return 0;
}
