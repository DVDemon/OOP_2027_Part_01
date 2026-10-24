#pragma once

#include <cstddef>
#include <vector>

namespace stk {

// Вариант 1: ПРИВАТНОЕ наследование от std::vector.
// Отношение "реализован посредством" (implemented-in-terms-of):
// StackPrivate не является vector, но использует его реализацию.
// Все public/protected-члены vector стали private — наружу не протекают.
class StackPrivate : private std::vector<int> {
public:
    void push(int value);
    void pop();
    int top() const;
    bool empty() const noexcept;
    std::size_t size() const noexcept;
};

// Вариант 2: КОМПОЗИЦИЯ — предпочтительный вариант того же отношения.
// Вектор хранится полем, и наружу виден только интерфейс стека.
class StackComposed {
    std::vector<int> data_;

public:
    void push(int value);
    void pop();
    int top() const;
    bool empty() const noexcept;
    std::size_t size() const noexcept;
};

}  // namespace stk
