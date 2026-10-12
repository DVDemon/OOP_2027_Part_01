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
//   - перемещающий конструктор (перенос владения, noexcept);
//   - деструктор;
//   - оператор присваивания через идиому copy-and-swap (обрабатывает
//     и копирование, и перемещение — отдельный move-assignment не нужен).
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

    // Конструктор перемещения — забирает ресурсы из source.
    DynamicArray(DynamicArray&& other) noexcept;

    // Обменивает содержимое с other за O(1) без исключений.
    // Основа copy-and-swap: обмен «сырыми» ресурсами, а не копирование.
    void swap(DynamicArray& other) noexcept;

    // Оператор присваивания (copy-and-swap).
    DynamicArray& operator=(DynamicArray other);

    // Или два отдельных оператора
    // DynamicArray& operator=(const DynamicArray& other);
    // DynamicArray& operator=(DynamicArray&& other) noexcept;

    void push_back(int value);

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    // Доступ с проверкой границ (бросает std::out_of_range).
    int at(std::size_t index) const;

    // Доступ без проверки.
    int operator[](std::size_t index) const { return data_[index]; }
    int& operator[](std::size_t index) { return data_[index]; }
};

// Свободный swap для ADL: позволяет generic-коду и идиоме copy-and-swap
// находить обмен без приведения к std::swap над целыми объектами.
void swap(DynamicArray& a, DynamicArray& b) noexcept;
