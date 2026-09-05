// Пример 02: передача параметров — по значению, по ссылке, по указателю, по const&.
#include <iostream>
#include <string>

// По значению: функция получает КОПИЮ, оригинал не меняется.
void increment_by_value(int x) {
    x += 1;  // меняется только локальная копия
    std::cout << "  внутри функции x = " << x << " (это копия)\n";
}

// По ссылке: функция работает с оригиналом, изменения видны вызывающему.
void increment_by_ref(int& x) {
    x += 1;
}

// По указателю: как ссылка, но может быть nullptr — нужна проверка.
void increment_by_ptr(int* x) {
    if (x != nullptr) {
        *x += 1;
    }
}

// По const&: чтение без копирования; модификация запрещена компилятором.
// Идеальный способ передачи «тяжёлых» объектов (строк, векторов).
void print(const std::string& s) {
    std::cout << s;
    // s += "!";  // ОШИБКА: попытка изменить const-ссылку
}

int main() {
    int a = 10;
    increment_by_value(a);
    std::cout << "после по-значению:  a = " << a << '\n';  // 10

    increment_by_ref(a);
    std::cout << "после по-ссылке:    a = " << a << '\n';  // 11

    increment_by_ptr(&a);
    std::cout << "после по-указателю: a = " << a << '\n';  // 12

    increment_by_ptr(nullptr);  // безопасно — функция проверяет nullptr
    std::cout << "после nullptr:      a = " << a << '\n';  // 12

    std::string name = "Hello";
    std::cout << "const& печать: ";
    print(name);                // без копирования
    print(", ");
    print("World\n");           // rvalue тоже привязывается к const&

    return 0;
}
