// Пример 04: четыре комбинации const с указателями и nullptr.
#include <iostream>

// Перегрузки для демонстрации однозначности nullptr.
void overloaded(int value) {
    std::cout << "вызвана overloaded(int) = " << value << '\n';
}
void overloaded(int* ptr) {
    std::cout << "вызвана overloaded(int*), null? "
              << (ptr == nullptr ? "да" : "нет") << '\n';
}

int main() {
    int x = 10;
    int y = 20;

    // 1. Обычный указатель: можно менять и значение, и адрес.
    int* p1 = &x;
    *p1 = 100;  // OK
    p1 = &y;    // OK
    std::cout << "p1 -> " << *p1 << '\n';

    // 2. Указатель на const: нельзя менять значение через указатель.
    const int* p2 = &x;
    // *p2 = 5;   // ОШИБКА компиляции
    p2 = &y;      // OK — сам указатель можно перенаправить
    std::cout << "p2 -> " << *p2 << '\n';

    // 3. const-указатель: нельзя перенаправить.
    int* const p3 = &x;
    *p3 = 7;    // OK — значение менять можно
    // p3 = &y;  // ОШИБКА компиляции
    std::cout << "p3 -> " << *p3 << '\n';

    // 4. const-указатель на const: ничего нельзя менять.
    const int* const p4 = &x;
    // *p4 = 1;  // ОШИБКА
    // p4 = &y;  // ОШИБКА
    std::cout << "p4 -> " << *p4 << '\n';

    // --- nullptr ---
    std::cout << "\n--- nullptr ---\n";
    int* empty = nullptr;  // типобезопасный нулевой указатель (C++11)
    if (empty == nullptr) {
        std::cout << "empty пуст\n";
    }

    // nullptr имеет собственный тип std::nullptr_t -> перегрузка однозначна.
    overloaded(nullptr);  // вызовет overloaded(int*), а не overloaded(int)

    return 0;
}
