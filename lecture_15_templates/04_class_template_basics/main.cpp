// Пример 01: Простейший шаблон класса.
//
// Демонстрирует:
//   * синтаксис template<typename T> class;
//   * хранение значения любого типа;
//   * методы get/set.

#include <iostream>
#include <string>
#include <typeinfo>

template <typename T>
class Box {
    T value_;
public:
    explicit Box(const T& value) : value_(value) {}

    void set(const T& value) { value_ = value; }
    const T& get() const { return value_; }

    void print() const {
        std::cout << "Box<" << typeid(T).name() << ">: " << value_ << "\n";
    }

    // operator<< для вложенных Box
    friend std::ostream& operator<<(std::ostream& os, const Box& b) {
        os << "Box(" << b.value_ << ")";
        return os;
    }
};

int main() {
    std::cout << "=== Box<int> ===\n";
    Box<int> intBox(42);
    intBox.print();
    intBox.set(100);
    std::cout << "get() = " << intBox.get() << "\n";

    std::cout << "\n=== Box<double> ===\n";
    Box<double> dblBox(3.14159);
    dblBox.print();

    std::cout << "\n=== Box<std::string> ===\n";
    Box<std::string> strBox("Hello, Templates!");
    strBox.print();

    // Box с пользовательским типом
    std::cout << "\n=== Box<Box<int>> ===\n";
    Box<Box<int>> nestedBox(intBox);
    nestedBox.print();

    return 0;
}
