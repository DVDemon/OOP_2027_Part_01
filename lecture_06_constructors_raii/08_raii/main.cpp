// Пример 07: идиома RAII (Resource Acquisition Is Initialization).
// Ресурс захватывается в конструкторе, освобождается в деструкторе —
// автоматически и корректно даже при исключении.
#include <iostream>
#include <stdexcept>

#include "timer.h"

// Чистая вычислительная нагрузка, чтобы таймеру было что измерять.
long long busy_work(int n) {
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += i % 7;
    }
    return sum;
}

void measured() {
    ScopedTimer t("measured");  // захват: старт таймера
    long long s = busy_work(2'000'000);
    std::cout << "  результат busy_work = " << s << '\n';
    // Выход из функции -> деструктор t -> печать длительности
}

void measured_with_exception() {
    ScopedTimer t("with_exception");  // захват
    busy_work(500'000);
    throw std::runtime_error("сбой в середине работы");
    // Несмотря на исключение, при раскрутке стека деструктор t вызовется
}

int main() {
    std::cout << "--- обычное выполнение ---\n";
    measured();

    std::cout << "--- выход через исключение ---\n";
    try {
        measured_with_exception();
    } catch (const std::exception& e) {
        std::cout << "  поймано исключение: " << e.what() << '\n';
    }

    std::cout << "--- конец main ---\n";
    return 0;
}
