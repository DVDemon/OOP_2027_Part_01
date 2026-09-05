// Пример 07: санитайзеры (-fsanitize). Этот код КОРРЕКТЕН и проходит ASan/UBSan.
// В README показано, как тот же код с ошибкой ловится санитайзером.
#include <cstddef>
#include <iostream>

int main() {
    constexpr std::size_t n = 10;
    int* arr = new int[n]{};

    // Корректный доступ строго в пределах [0, n).
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = static_cast<int>(i * i);
    }

    std::cout << "Сумма квадратов 0..9 = ";
    long sum = 0;
    for (std::size_t i = 0; i < n; ++i) {
        sum += arr[i];  // нет выхода за границу -> ASan молчит
    }
    std::cout << sum << '\n';

    delete[] arr;  // нет утечки -> LeakSanitizer молчит

    std::cout << "Соберите с -DENABLE_SANITIZERS=ON и запустите — ошибок нет.\n";
    std::cout << "В README показано, какую диагностику дал бы код с ошибкой.\n";
    return 0;
}
