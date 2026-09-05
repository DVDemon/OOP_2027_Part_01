// Пример 04: идиома copy-and-swap.
//
// Один оператор присваивания, принимающий аргумент ПО ЗНАЧЕНИЮ, решает сразу
// две задачи:
//   - exception safety: копия делается ДО изменения *this;
//   - самоприсваивание: работает корректно без явной проверки this == &other.
// Обмен (swap) — операция без исключений (noexcept).
#include <cstring>
#include <iostream>
#include <utility>

class Text {
public:
    Text(const char* s = "")
        : size_(std::strlen(s)), data_(new char[size_ + 1]) {
        std::strcpy(data_, s);
    }

    ~Text() { delete[] data_; }

    // Копирующий конструктор — глубокая копия.
    Text(const Text& other)
        : size_(other.size_), data_(new char[other.size_ + 1]) {
        std::strcpy(data_, other.data_);
    }

    // Перемещающий конструктор — нужен, чтобы copy-and-swap обрабатывал и move.
    Text(Text&& other) noexcept : size_(other.size_), data_(other.data_) {
        other.data_ = nullptr;
        other.size_ = 0;
    }

    // Дружественный swap — обмен полей, не бросает исключений.
    friend void swap(Text& a, Text& b) noexcept {
        using std::swap;
        swap(a.size_, b.size_);
        swap(a.data_, b.data_);
    }

    // Copy-and-swap: аргумент передаётся ПО ЗНАЧЕНИЮ (копия уже сделана).
    // Один оператор обслуживает и копирующее, и перемещающее присваивание.
    Text& operator=(Text other) {
        swap(*this, other);  // меняемся с копией; старое уйдёт с other
        return *this;
    }

    const char* data() const { return data_; }

private:
    std::size_t size_;
    char* data_;
};

int main() {
    Text a("Hello");
    Text b("temp");

    b = a;  // копирующее присваивание (аргумент скопирован в параметр)
    std::cout << "после b = a: b = \"" << b.data() << "\"\n";

    Text c("World");
    c = std::move(a);  // перемещающее присваивание (тот же operator=)
    std::cout << "после c = std::move(a): c = \"" << c.data() << "\"\n";

    Text& alias = b;
    b = alias;  // самоприсваивание — корректно, без явной проверки
    std::cout << "после b = b (через ссылку): b = \"" << b.data() << "\"\n";

    return 0;
}
