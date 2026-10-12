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

    print("empty", empty);
    print("filled", filled);


    std::cout << "copy operatio: \n";
    empty = filled;

    print("empty", empty);
    print("filled", filled);

    std::cout << "move operation: \n";
    empty =  {1,2,3,4}; // из initializer_list
    print("empty", empty);



    return 0;
}
