// Пример 03: префиксный и постфиксный ++ / --.
#include <iostream>

// Счётчик: демонстрирует разницу между ++c и c++.
class Counter {
    int value_;
public:
    explicit Counter(int v = 0) : value_(v) {}

    // --- Префиксный ++c: меняет объект, возвращает ССЫЛКУ ---
    Counter& operator++() {
        ++value_;
        return *this;
    }

    // --- Постфиксный c++: int-заглушка отличает его от префиксного ---
    // Возвращает КОПИЮ объекта ДО изменения.
    Counter operator++(int) {
        Counter old = *this;
        ++(*this);   // переиспользуем префиксный
        return old;
    }

    // --- Аналогично для -- ---
    Counter& operator--() {
        --value_;
        return *this;
    }

    Counter operator--(int) {
        Counter old = *this;
        --(*this);
        return old;
    }

    int value() const { return value_; }
};

int main() {
    Counter c(5);

    // Префиксный: сначала увеличили, потом используем.
    std::cout << "++c -> " << (++c).value() << " (c = " << c.value() << ")\n";

    // Постфиксный: используем старое значение, потом увеличиваем.
    Counter old = c++;
    std::cout << "c++  -> old = " << old.value()
              << ", c = " << c.value() << '\n';

    --c;
    std::cout << "после --c: c = " << c.value() << '\n';

    return 0;
}
