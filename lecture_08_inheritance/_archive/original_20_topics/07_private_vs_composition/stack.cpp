#include "stack.h"

namespace stk {

void Stack::push(int value) {
    data_.push_back(value);
}

void Stack::pop() {
    data_.pop_back();
}

int Stack::top() const {
    return data_.back();
}

bool Stack::empty() const noexcept {
    return data_.empty();
}

std::size_t Stack::size() const noexcept {
    return data_.size();
}

}  // namespace stk
