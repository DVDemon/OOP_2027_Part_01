#include "stack.h"

namespace stk {

// --- StackPrivate: приватное наследование ---------------------------------

void StackPrivate::push(int value) {
    push_back(value);  // доступно, потому что член vector стал private-членом
}

void StackPrivate::pop() {
    pop_back();
}

int StackPrivate::top() const {
    return back();
}

bool StackPrivate::empty() const noexcept {
    return std::vector<int>::empty();
}

std::size_t StackPrivate::size() const noexcept {
    return std::vector<int>::size();
}

// --- StackComposed: композиция --------------------------------------------

void StackComposed::push(int value) {
    data_.push_back(value);
}

void StackComposed::pop() {
    data_.pop_back();
}

int StackComposed::top() const {
    return data_.back();
}

bool StackComposed::empty() const noexcept {
    return data_.empty();
}

std::size_t StackComposed::size() const noexcept {
    return data_.size();
}

}  // namespace stk
