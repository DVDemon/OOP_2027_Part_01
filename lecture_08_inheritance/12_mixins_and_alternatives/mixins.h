#pragma once

#include <iostream>
#include <string>
#include <utility>

// Примеси (mixins) без шаблонов: маленькие независимые классы поведения.
// Они не образуют отношение is-a и не несут общей базы, поэтому
// множественное наследование от них безопасно — ромба не возникает.

class Clickable {
public:
    void click() const { std::cout << "клик\n"; }
};

class Hoverable {
public:
    void hover() const { std::cout << "наведение\n"; }
};

// Кнопка подмешивает оба поведения и добавляет собственное состояние.
class Button : public Clickable, public Hoverable {
    std::string label_;

public:
    explicit Button(std::string label) : label_(std::move(label)) {}

    void render() const {
        std::cout << "[" << label_ << "] ";
    }

    const std::string& label() const noexcept { return label_; }
};
