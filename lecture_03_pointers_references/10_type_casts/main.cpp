// Пример 10: приведение типов — неявное, static_cast, const_cast, reinterpret_cast.
#include <cstddef>
#include <iostream>

int main() {
    // --- Неявные преобразования ---
    std::cout << "--- Неявные преобразования ---\n";
    int i = 42;
    double d = i;  // int -> double (расширение, безопасно)
    std::cout << "int -> double: " << d << '\n';

    double pi = 3.99;
    int truncated_implicit = pi;  // double -> int (сужение, дробная часть теряется)
    std::cout << "double -> int (неявно): " << truncated_implicit << '\n';

    // --- static_cast: явное, документированное, проверяемое преобразование ---
    std::cout << "\n--- static_cast ---\n";
    int truncated = static_cast<int>(pi);  // явно показываем намерение «округлить вниз»
    std::cout << "static_cast<int>(3.99) = " << truncated << '\n';

    void* vp = &pi;
    double* dp = static_cast<double*>(vp);  // void* -> T*
    std::cout << "через void* и обратно: " << *dp << '\n';

    // --- const_cast: снятие/добавление const ---
    std::cout << "\n--- const_cast ---\n";
    int value = 7;
    const int* cptr = &value;            // указатель на const, но объект НЕ const
    int* mutable_ptr = const_cast<int*>(cptr);
    *mutable_ptr = 99;                   // OK: исходный объект value не const
    std::cout << "после const_cast: value = " << value << '\n';
    // Если бы value был объявлен как const int, запись через mutable_ptr -> UB.

    // --- reinterpret_cast: побитовая реинтерпретация (низкоуровневый код) ---
    std::cout << "\n--- reinterpret_cast ---\n";
    int x = 0x01020304;
    auto* bytes = reinterpret_cast<unsigned char*>(&x);
    std::cout << "байты int (зависят от порядка байт):";
    for (std::size_t b = 0; b < sizeof(int); ++b) {
        std::cout << ' ' << static_cast<int>(bytes[b]);
    }
    std::cout << '\n';

    return 0;
}
