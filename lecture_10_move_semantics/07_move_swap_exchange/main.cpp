// Пример 07: std::move, std::swap через move, std::exchange.
//
//   std::move(x)        -> static_cast<T&&>(x): НЕ перемещает, лишь РАЗРЕШАЕТ
//                          перемещение, приводя x к rvalue.
//   std::swap(a, b)     -> обмен через 3 move-операции (а не 3 копирования).
//   std::exchange(x, v) -> присваивает x = v и ВОЗВРАЩАЕТ старое значение x.
#include <iostream>
#include <string>
#include <utility>

int main() {
    std::cout << "--- std::move ---\n";
    std::string a = "Hello";
    std::string b = std::move(a);  // перемещающий конструктор std::string
    std::cout << "b = \"" << b << "\"\n";
    std::cout << "a после move = \"" << a
              << "\" (valid-but-unspecified, обычно пусто)\n";

    std::cout << "\n--- std::swap через move ---\n";
    std::string x = "first";
    std::string y = "second";
    std::cout << "до swap:  x=\"" << x << "\" y=\"" << y << "\"\n";
    std::swap(x, y);  // внутри: 1 move-конструирование + 2 move-присваивания
    std::cout << "после swap: x=\"" << x << "\" y=\"" << y << "\"\n";

    std::cout << "\n--- std::exchange ---\n";
    int counter = 5;
    int old = std::exchange(counter, 10);  // counter = 10, вернёт старое (5)
    std::cout << "old = " << old << ", counter = " << counter << '\n';

    // Типичное применение: обнулить источник и получить его прежнее значение
    // одним выражением (удобно в перемещающем конструкторе).
    std::string src = "resource";
    std::string moved = std::exchange(src, std::string{});  // забрать и очистить
    std::cout << "moved = \"" << moved << "\", src = \"" << src
              << "\" (очищен через exchange)\n";

    return 0;
}
