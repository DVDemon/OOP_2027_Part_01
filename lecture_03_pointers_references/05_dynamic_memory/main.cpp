// Пример 05: динамическая память new/delete, new[]/delete[].
// Демонстрирует базовые операции и ручной динамический массив (dyn::*).
#include <cstddef>
#include <iostream>

#include "dynamic_array.h"

static void print_array(const int* array, std::size_t size) {
    std::cout << '[';
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << array[i];
        if (i + 1 < size) std::cout << ", ";
    }
    std::cout << "]\n";
}

int main() {
    // --- Один объект: new / delete ---
    int* p = new int(42);
    std::cout << "*p = " << *p << '\n';
    delete p;  // парное освобождение

    // --- Массив: new[] / delete[] ---
    int* arr = new int[5]{10, 20, 30, 40, 50};
    std::cout << "массив: ";
    print_array(arr, 5);
    delete[] arr;  // именно delete[], не delete!

    // Value-initialization: гарантированные нули.
    int* zeroed = new int[3]{};
    std::cout << "zeroed: ";
    print_array(zeroed, 3);
    delete[] zeroed;

    // --- Ручной динамический массив (практическое задание) ---
    std::cout << "\n--- dyn::* ---\n";
    std::size_t size = 3;
    int* data = dyn::create_array(size);  // {0, 0, 0}
    for (std::size_t i = 0; i < size; ++i) data[i] = static_cast<int>(i + 1);
    std::cout << "создан:   ";
    print_array(data, size);  // [1, 2, 3]

    data = dyn::insert_at(data, size, 1, 99);
    std::cout << "вставка:  ";
    print_array(data, size);  // [1, 99, 2, 3]

    data = dyn::remove_at(data, size, 0);
    std::cout << "удаление: ";
    print_array(data, size);  // [99, 2, 3]

    data = dyn::resize_array(data, size, 5);
    size = 5;
    std::cout << "resize:   ";
    print_array(data, size);  // [99, 2, 3, 0, 0]

    dyn::destroy_array(data);  // обязательно освобождаем
    return 0;
}
