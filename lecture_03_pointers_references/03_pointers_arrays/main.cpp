// Пример 03: указатели и массивы. Имя массива «распадается» в указатель.
#include <cstddef>
#include <iostream>

// Плохой вариант: при передаче массива теряется его размер (decay в указатель).
void print_decayed(int* data, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        std::cout << data[i] << ' ';
    }
    std::cout << '\n';
}

// Хороший вариант: ссылка на массив сохраняет размер на этапе компиляции.
template <std::size_t N>
void print_by_ref(int (&data)[N]) {
    for (auto i : data) {
        std::cout << i << ' ';
    }
    std::cout << '\n';
}

int main() {
    int arr[5] = {1, 2, 3, 4, 5};

    std::cout << "--- Эквивалентные способы доступа ---\n";
    std::cout << "arr[2]     = " << arr[2] << '\n';        // 3
    std::cout << "*(arr + 2) = " << *(arr + 2) << '\n';    // 3
    std::cout << "2[arr]     = " << 2[arr] << '\n';        // 3, т.к. a[b] == *(a + b)

    std::cout << "\n--- Имя массива распадается в указатель ---\n";
    std::cout << "sizeof(arr) = " << sizeof(arr)
              << " байт (весь массив)\n";
    int* p = arr;  // неявное преобразование к указателю на первый элемент
    std::cout << "sizeof(p)   = " << sizeof(p)
              << " байт (только указатель)\n";

    std::cout << "\nПередача с потерей размера (нужно передать size отдельно):\n";
    print_decayed(arr, 5);

    std::cout << "Передача по ссылке (размер выводится автоматически):\n";
    print_by_ref(arr);

    return 0;
}
