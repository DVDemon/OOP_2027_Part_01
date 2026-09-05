// Пример 08: lvalue-ссылки, указатели vs ссылки, константные ссылки.
#include <iostream>

// Передача по ссылке: функция меняет оригинал, без копии и без разыменования.
void increment(int& value) {
    ++value;  // меняем сам аргумент
}

// Передача по const-ссылке: без копии, без права на изменение.
void print(const int& value) {
    std::cout << "value = " << value << '\n';
}

int main() {
    int x = 42;
    int& ref = x;  // ref — псевдоним (alias) для x

    std::cout << "--- Ссылка как псевдоним ---\n";
    std::cout << "x   = " << x << ", ref = " << ref << '\n';
    ref = 100;  // меняем x через ссылку (без * как у указателя)
    std::cout << "после ref = 100: x = " << x << '\n';

    // Ссылка и переменная имеют один адрес.
    std::cout << "&x   = " << static_cast<void*>(&x) << '\n';
    std::cout << "&ref = " << static_cast<void*>(&ref) << " (тот же адрес)\n";

    std::cout << "\n--- Передача по ссылке ---\n";
    increment(x);  // x изменится
    std::cout << "после increment(x): x = " << x << '\n';
    print(x);

    // --- Константная ссылка может быть привязана к временному (rvalue) ---
    const int& cref = 42;  // продлевает жизнь временного объекта
    std::cout << "\nconst int& cref = 42; cref = " << cref << '\n';
    // int& bad = 42;       // ОШИБКА: lvalue-ссылка не привязывается к rvalue
    // cref = 7;            // ОШИБКА: через const-ссылку менять нельзя

    return 0;
}
