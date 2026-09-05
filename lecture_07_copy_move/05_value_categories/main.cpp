// Пример 05: rvalue-ссылки (T&&) и категории значений.
//
// Категории выражений в C++:
//
//             expression
//             /        \
//        glvalue       rvalue
//        /     \       /     \
//    lvalue   xvalue        prvalue
//
//   lvalue  — имеет имя, можно взять адрес: int x; объект s;
//   prvalue — чистое временное значение: 42, s1 + s2, Foo();
//   xvalue  — «истекающее» значение: std::move(x), результат вызова T&&.
//
// rvalue-ссылка T&& привязывается к временным объектам (prvalue/xvalue) и
// позволяет перегрузить функцию для них отдельно от lvalue.
#include <iostream>
#include <string>
#include <utility>

// Перегрузки по категории аргумента.
void process(const std::string& s) {
    std::cout << "lvalue-ссылка: \"" << s << "\"\n";
}

void process(std::string&& s) {
    std::cout << "rvalue-ссылка: \"" << s << "\"\n";
}

std::string make() { return "temporary"; }  // возвращает prvalue

int main() {
    std::string a = "named";

    process(a);            // lvalue   -> const std::string&
    process("literal");    // prvalue  -> std::string&&  (создаётся временный)
    process(make());       // prvalue  -> std::string&&
    process(std::move(a));  // xvalue   -> std::string&&  (a приведён к rvalue)

    std::cout << "\n--- Адрес можно взять только у lvalue ---\n";
    int x = 10;
    std::cout << "&x = " << static_cast<const void*>(&x) << "  (x — lvalue)\n";
    // int* p = &(x + 1);   // ОШИБКА: нельзя взять адрес prvalue (x + 1)
    // int* q = &std::move(x); // ОШИБКА: нельзя взять адрес xvalue

    std::cout << "\n--- rvalue-ссылка САМА является lvalue ---\n";
    int&& r = 42;  // r привязана к временному; но имя r — это lvalue
    std::cout << "r = " << r << ", &r = " << static_cast<const void*>(&r)
              << "  (у r есть имя -> r это lvalue)\n";

    return 0;
}
