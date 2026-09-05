#pragma once

#include <cstddef>

// Buffer — простой владелец динамического массива int.
// Демонстрирует идиому copy-and-swap для строгой гарантии в operator=.
class Buffer {
    std::size_t size_;
    int* data_;
public:
    explicit Buffer(std::size_t n = 0);

    Buffer(const Buffer& other);              // копирование (может бросить)
    Buffer(Buffer&& other) noexcept;          // перемещение (не бросает)

    ~Buffer();

    // Единственный оператор присваивания: принимает копию ПО ЗНАЧЕНИЮ.
    // Компилятор сам вызовет copy- или move-конструктор для параметра.
    Buffer& operator=(Buffer other) noexcept;

    // swap не бросает — это ключ к строгой гарантии.
    friend void swap(Buffer& a, Buffer& b) noexcept;

    std::size_t size() const noexcept { return size_; }

    int& at(std::size_t i);
    int  at(std::size_t i) const;
};
