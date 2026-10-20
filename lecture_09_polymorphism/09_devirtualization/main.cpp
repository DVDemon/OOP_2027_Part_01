// Пример 09: стоимость виртуальных вызовов и девиртуализация.
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

class Base {
public:
    virtual int compute(int x) const { return x; }
    virtual ~Base() = default;
};

// final у класса: компилятор знает, что потомков нет -> может
// девиртуализировать вызовы и встроить (inline) их.
class Fast final : public Base {
public:
    int compute(int x) const override { return x * 2 + 1; }
};

class Slow : public Base {
public:
    int compute(int x) const override { return x * 3 - 1; }
};

int main() {
    constexpr int kIters = 5'000'000;

    // --- Прямой вызов: тип известен статически, возможна девиртуализация ---
    Fast fast;
    {
        auto t0 = std::chrono::steady_clock::now();
        long long acc = 0;
        for (int i = 0; i < kIters; ++i) {
            acc += fast.compute(i);  // компилятор знает точный тип -> прямой вызов
        }
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "Прямой вызов (девиртуализуемый): acc=" << acc << ", "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
                  << " мс\n";
    }

    // --- Виртуальный вызов через указатель на базовый класс ---
    std::vector<std::unique_ptr<Base>> objs;
    objs.push_back(std::make_unique<Fast>());
    objs.push_back(std::make_unique<Slow>());
    {
        auto t0 = std::chrono::steady_clock::now();
        long long acc = 0;
        for (int i = 0; i < kIters; ++i) {
            // Тип не известен компилятору -> косвенный (виртуальный) вызов.
            acc += objs[i % objs.size()]->compute(i);
        }
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "Виртуальный вызов (через Base*): acc=" << acc << ", "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count()
                  << " мс\n";
    }

    std::cout << "\nВывод: виртуальный вызов добавляет косвенность и мешает\n"
                 "инлайнингу. Если тип известен (или класс/метод помечен final),\n"
                 "компилятор девиртуализирует вызов. Конкретные числа зависят\n"
                 "от компилятора, флагов оптимизации и железа.\n";

    return 0;
}
