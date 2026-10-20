// Пример 02: Шаблон стека.
//
// Демонстрирует:
//   * реализацию шаблонного класса Stack<T>;
//   * методы push, pop, top, empty, size;
//   * использование с разными типами.

#include <iostream>
#include <format>
#include <vector>
#include <stdexcept>

template <typename T>
class Stack {
    std::vector<T> data_;
public:
    void push(const T& value) {
        data_.push_back(value);
    }

    void pop() {
        if (empty()) throw std::out_of_range("Stack is empty");
        data_.pop_back();
    }

    const T& top() const {
        if (empty()) throw std::out_of_range("Stack is empty");
        return data_.back();
    }

    bool empty() const { return data_.empty(); }
    size_t size() const { return data_.size(); }

    void print() const {
        std::cout << "Stack (bottom→top): [";
        for (size_t i = 0; i < data_.size(); ++i) {
            std::cout << data_[i];
            if (i + 1 < data_.size()) std::cout << ", ";
        }
        std::cout << "]\n";
    }
};

int main() {
    std::cout << "=== Stack<int> ===\n";
    Stack<int> intStack;
    intStack.push(10);
    intStack.push(20);
    intStack.push(30);
    intStack.print();
    std::cout << std::format("top: {}, size: {}\n", intStack.top(), intStack.size());

    intStack.pop();
    intStack.print();
    std::cout << std::format("top после pop: {}\n", intStack.top());

    std::cout << "\n=== Stack<std::string> ===\n";
    Stack<std::string> strStack;
    strStack.push("C++");
    strStack.push("Templates");
    strStack.push("POCO");
    strStack.print();

    return 0;
}
