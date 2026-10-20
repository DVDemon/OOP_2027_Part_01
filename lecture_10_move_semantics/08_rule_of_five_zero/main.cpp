// Пример 08: Правило пяти (Rule of Five) и Правило нуля (Rule of Zero).
//
//   Rule of Five: если класс УПРАВЛЯЕТ ресурсом напрямую (сырой указатель),
//                 определите все пять спецметодов: деструктор, копирующий и
//                 перемещающий конструкторы, копирующее и перемещающее
//                 присваивание.
//   Rule of Zero: если класс НЕ управляет ресурсом напрямую (использует
//                 std::string, std::vector, умные указатели), НЕ определяйте
//                 ни одного спецметода — компилятор сгенерирует корректные.
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// --- Rule of Five: класс владеет сырым буфером ---
class Buffer {
public:
    explicit Buffer(std::size_t n)
        : data_(new std::byte[n]{}), size_(n) {}

    ~Buffer() { delete[] data_; }  // 1

    Buffer(const Buffer& o)        // 2
        : data_(new std::byte[o.size_]), size_(o.size_) {
        std::copy(o.data_, o.data_ + size_, data_);
    }

    Buffer& operator=(const Buffer& o) {  // 3
        if (this != &o) {
            auto* tmp = new std::byte[o.size_];
            std::copy(o.data_, o.data_ + o.size_, tmp);
            delete[] data_;
            data_ = tmp;
            size_ = o.size_;
        }
        return *this;
    }

    Buffer(Buffer&& o) noexcept          // 4
        : data_(std::exchange(o.data_, nullptr))
        , size_(std::exchange(o.size_, 0)) {}

    Buffer& operator=(Buffer&& o) noexcept {  // 5
        if (this != &o) {
            delete[] data_;
            data_ = std::exchange(o.data_, nullptr);
            size_ = std::exchange(o.size_, 0);
        }
        return *this;
    }

    std::size_t size() const { return size_; }

private:
    std::byte* data_;
    std::size_t size_;
};

// --- Rule of Zero: ни одного спецметода ---
// Все члены сами управляют своей памятью, компилятор сгенерирует корректные
// copy/move/dtor автоматически.
class Person {
public:
    Person(std::string name, int age) : name_(std::move(name)), age_(age) {}

    const std::string& name() const { return name_; }
    int age() const { return age_; }

private:
    std::string name_;       // сам управляет памятью
    std::vector<int> ids_;   // тоже
    int age_ = 0;
};

int main() {
    std::cout << "--- Rule of Five ---\n";
    Buffer a(16);
    Buffer b = a;             // копирующий конструктор
    Buffer c = std::move(a);  // перемещающий конструктор
    std::cout << "b.size = " << b.size() << ", c.size = " << c.size() << '\n';

    std::cout << "\n--- Rule of Zero ---\n";
    Person p{"Анна", 30};
    Person q = p;             // сгенерированная копия (deep copy строки/вектора)
    Person r = std::move(p);  // сгенерированное перемещение
    std::cout << "q: " << q.name() << ", " << q.age() << '\n';
    std::cout << "r: " << r.name() << ", " << r.age() << '\n';
    std::cout << "Person не определяет НИ ОДНОГО спецметода — и это правильно.\n";

    return 0;
}
