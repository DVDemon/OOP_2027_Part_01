// Пример 10: DynamicArray — конструкторы лекции и связь с правилом пяти.
// Демонстрация в консоли; автоматические проверки — в tests.cpp.
#include <iostream>

#include "dynamic_array.h"

void print(const char* label, const DynamicArray& arr) {
    std::cout << label << " (size=" << arr.size()
              << ", capacity=" << arr.capacity() << "): ";
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::cout << "--- виды конструкторов ---\n";
    DynamicArray empty;                 // по умолчанию
    DynamicArray reserved(4);           // explicit от capacity
    DynamicArray filled(3, 7);          // делегирующий (count, value)
    DynamicArray fromList{1, 2, 3, 4};  // из initializer_list
    print("empty", empty);
    print("reserved", reserved);
    print("filled", filled);
    print("fromList", fromList);

    std::cout << "--- push_back с ростом capacity ---\n";
    for (int i = 0; i < 5; ++i) {
        empty.push_back(i * 10);
    }
    print("empty после push_back", empty);

    std::cout << "--- глубокая копия ---\n";
    DynamicArray copy = fromList;  // копирующий конструктор
    copy[0] = 100;                 // меняем копию
    print("оригинал fromList", fromList);
    print("копия copy", copy);
    std::cout << "копия независима: fromList[0]=" << fromList[0]
              << ", copy[0]=" << copy[0] << '\n';

    std::cout << "--- at() с проверкой границ ---\n";
    try {
        std::cout << "filled.at(10) = " << filled.at(10) << '\n';
    } catch (const std::out_of_range& e) {
        std::cout << "поймано: " << e.what() << '\n';
    }

    return 0;
}
