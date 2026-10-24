#pragma once

#include <cstddef>
#include <vector>

namespace stk {

// Хороший вариант: стек через КОМПОЗИЦИЮ.
// Мы скрываем std::vector полностью и публикуем только интерфейс стека.
class Stack {
    std::vector<int> data_;  // композиция: "реализован посредством" vector

public:
    void push(int value);
    void pop();
    int top() const;
    bool empty() const noexcept;
    std::size_t size() const noexcept;
};

}  // namespace stk
