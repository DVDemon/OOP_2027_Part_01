// Пример 01: модель памяти процесса — стек, куча, статическая память, код.
// Печатаем адреса объектов из разных сегментов и сравниваем их.
#include <iostream>

// Глобальная переменная — статическая память (сегмент данных).
int global_var = 10;

// Функция — её код лежит в сегменте кода (read-only).
void some_function() {}

void demo() {
    int local_var = 20;            // стек
    static int static_var = 30;    // статическая память (инициализируется один раз)
    int* heap_var = new int(40);   // указатель на стеке, данные — в куче

    std::cout << "--- Адреса объектов разных сегментов ---\n";
    std::cout << "Сегмент кода (функция):    " << reinterpret_cast<const void*>(&some_function) << '\n';
    std::cout << "Статическая (global_var):  " << static_cast<const void*>(&global_var) << '\n';
    std::cout << "Статическая (static_var):  " << static_cast<const void*>(&static_var) << '\n';
    std::cout << "Куча (heap_var):           " << static_cast<const void*>(heap_var) << '\n';
    std::cout << "Стек (local_var):          " << static_cast<const void*>(&local_var) << '\n';

    delete heap_var;  // куча: время жизни контролирует программист
}

int main() {
    std::cout << "Стек растёт вниз, куча — вверх; адреса различаются на порядки.\n\n";
    demo();

    // Стек: быстрое выделение/освобождение, но размер ограничен (обычно 1-8 МБ).
    // Куча: гибкий размер, но медленнее и требует ручного управления.
    std::cout << "\nРазмер указателя не зависит от типа данных:\n";
    std::cout << "sizeof(int*)    = " << sizeof(int*) << '\n';
    std::cout << "sizeof(double*) = " << sizeof(double*) << '\n';
    return 0;
}
