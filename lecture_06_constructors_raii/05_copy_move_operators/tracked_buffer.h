#pragma once

#include <cstddef>
#include <string>

// Класс, владеющий динамическим буфером int, который печатает каждый
// специальный метод. Демонстрирует ПЕРЕГРУЗКУ операторов присваивания
// (копирующего и перемещающего) вместе с парными конструкторами:
//   - копирующий конструктор        T(const T&)         — глубокая копия;
//   - копирующий оператор присваивания T& operator=(const T&);
//   - перемещающий конструктор      T(T&&) noexcept     — забирает владение;
//   - перемещающий оператор присваивания T& operator=(T&&) noexcept;
//   - деструктор.
// Это и есть правило пяти «в деле»: все пять методов продуманы и согласованы.
class TrackedBuffer {
    int* data_;
    std::size_t size_;
    std::string name_;

public:
    // Владеющий конструктор: size элементов, заполненных значением fill.
    TrackedBuffer(std::string name, std::size_t size, int fill = 0);

    // Освобождает new[].
    ~TrackedBuffer();

    // --- Копирование: глубокое, у каждого объекта свой буфер ---
    TrackedBuffer(const TrackedBuffer& other);
    TrackedBuffer& operator=(const TrackedBuffer& other);

    // --- Перемещение: ресурс передаётся, источник остаётся «пустым» ---
    // noexcept обязателен: только тогда std::vector при росте использует
    // перемещение вместо копирования.
    TrackedBuffer(TrackedBuffer&& other) noexcept;
    TrackedBuffer& operator=(TrackedBuffer&& other) noexcept;

    // Обмен «внутренностями» — основа копирующего присваивания
    // через copy-and-swap (сильная гарантия исключений).
    void swap(TrackedBuffer& other) noexcept;

    std::size_t size() const { return size_; }
    const std::string& name() const { return name_; }
    bool empty() const { return data_ == nullptr; }

    // Доступ без проверки границ.
    int operator[](std::size_t index) const { return data_[index]; }
    int& operator[](std::size_t index) { return data_[index]; }

    // Доступ с проверкой границ (бросает std::out_of_range).
    int at(std::size_t index) const;
};
