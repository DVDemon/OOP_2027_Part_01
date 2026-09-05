#pragma once

#include <cstddef>
#include <initializer_list>

// Динамический массив int, владеющий памятью через new[]/delete[].
// Демонстрирует конструкторы из лекции и связь с правилом пяти:
//   - конструктор по умолчанию;
//   - explicit-конструктор от capacity;
//   - конструктор из initializer_list;
//   - делегирующий конструктор (count, value);
//   - копирующий конструктор (глубокая копия);
//   - деструктор;
//   - перемещение пока удалено (=delete) — реализация в следующей лекции.
class DynamicArray {
    int* data_;
    std::size_t size_;
    std::size_t capacity_;

public:
    // Пустой массив.
    DynamicArray();

    // Резервирует capacity элементов, size остаётся 0.
    explicit DynamicArray(std::size_t capacity);

    // Делегирует к DynamicArray(count), затем заполняет value (size = count).
    DynamicArray(std::size_t count, int value);

    // Массив из списка инициализации: {1, 2, 3}.
    DynamicArray(std::initializer_list<int> values);

    // Копирующий конструктор — глубокая копия.
    DynamicArray(const DynamicArray& other);

    // Деструктор — освобождает new[].
    ~DynamicArray();

    // Перемещение пока запрещаем (тема следующей лекции — правило пяти).
    DynamicArray(DynamicArray&&) = delete;
    DynamicArray& operator=(DynamicArray&&) = delete;
    // Присваивание копированием тоже отложим (нужно правило пяти целиком).
    DynamicArray& operator=(const DynamicArray&) = delete;

    void push_back(int value);

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    // Доступ с проверкой границ (бросает std::out_of_range).
    int at(std::size_t index) const;

    // Доступ без проверки.
    int operator[](std::size_t index) const { return data_[index]; }
    int& operator[](std::size_t index) { return data_[index]; }
};
