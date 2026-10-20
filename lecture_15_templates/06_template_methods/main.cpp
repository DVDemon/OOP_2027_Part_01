// Пример 03: Методы-шаблоны внутри шаблонного класса.
//
// Демонстрирует:
//   * метод-шаблон, не зависящий от параметра класса;
//   * конвертирующий конструктор;
//   * присваивание между разными инстанцированиями.

#include <iostream>
#include <typeinfo>

template <typename T>
class Value {
    T value_;
public:
    explicit Value(const T& v) : value_(v) {}

    // Конвертирующий конструктор-шаблон
    template <typename U>
    Value(const Value<U>& other) : value_(static_cast<T>(other.get())) {}

    // Метод-шаблон
    template <typename U>
    Value<T>& add(const Value<U>& other) {
        value_ += static_cast<T>(other.get());
        return *this;
    }

    const T& get() const { return value_; }

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, const Value<U>& v);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Value<T>& v) {
    os << "Value<" << typeid(T).name() << ">(" << v.value_ << ")";
    return os;
}

int main() {
    std::cout << "=== Методы-шаблоны ===\n";

    Value<int> vi(42);
    Value<double> vd(3.14);

    std::cout << "vi = " << vi << "\n";
    std::cout << "vd = " << vd << "\n";

    // Вызов метода-шаблона
    vi.add(vd);
    std::cout << "vi.add(vd) = " << vi << "\n";

    // Конвертирующий конструктор
    Value<double> vd2(vi);
    std::cout << "vd2(vi) = " << vd2 << "\n";

    return 0;
}
