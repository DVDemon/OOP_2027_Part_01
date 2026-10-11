// Пример 11: перегрузка копирующих и перемещающих операторов.
// Класс TrackedBuffer печатает каждый специальный метод, поэтому в консоли
// видно, ЧТО именно вызывается: копия, перемещение или присваивание.
#include <iostream>
#include <utility>
#include <vector>

#include "tracked_buffer.h"

void print(const char* label, const TrackedBuffer& b) {
    std::cout << label << " \"" << b.name() << "\" (size=" << b.size() << "): ";
    for (std::size_t i = 0; i < b.size(); ++i) {
        std::cout << b[i] << ' ';
    }
    std::cout << '\n';
}

int main() {
    std::cout << "=== 1. Копирующий конструктор (глубокая копия) ===\n";
    {
        TrackedBuffer original("orig", 3, 7);
        TrackedBuffer copy = original;  // TrackedBuffer(const TrackedBuffer&)
        copy[0] = 100;
        print("original", original);
        print("copy    ", copy);
        std::cout << "  копия независима: orig[0]=" << original[0]
                  << ", copy[0]=" << copy[0] << "\n";
    }

    std::cout << "\n=== 2. Копирующее присваивание ===\n";
    {
        TrackedBuffer a("a", 2, 1);
        TrackedBuffer b("b", 4, 9);
        b = a;  // operator=(const TrackedBuffer&)
        print("после b = a, a", a);
        print("после b = a, b", b);

        // Самоприсваивание безопасно (копия — обмен — освобождение).
        TrackedBuffer& alias = b;
        b = alias;
        print("после самоприсваивания b", b);
    }

    std::cout << "\n=== 3. Перемещающий конструктор ===\n";
    {
        TrackedBuffer src("src", 3, 5);
        TrackedBuffer dst = std::move(src);  // TrackedBuffer(TrackedBuffer&&)
        print("src (перемещён)", src);
        print("dst            ", dst);
    }

    std::cout << "\n=== 4. Перемещающее присваивание ===\n";
    {
        TrackedBuffer x("x", 2, 1);
        TrackedBuffer y("y", 5, 2);
        x = std::move(y);  // operator=(TrackedBuffer&&)
        print("x", x);
        print("y (перемещён)", y);
    }

    std::cout << "\n=== 5. Почему noexcept важен: рост std::vector ===\n";
    {
        std::vector<TrackedBuffer> vec;
        vec.reserve(0);  // без резерва — увидим перевыделения
        for (int i = 0; i < 3; ++i) {
            std::cout << "  push_back #" << i << ":\n";
            vec.push_back(TrackedBuffer("v" + std::to_string(i), 1, i));
        }
        std::cout << "  vector size = " << vec.size() << '\n';
        // При перевыделении буфера vector элементы переносятся ПЕРЕМЕЩЕНИЕМ,
        // потому что конструктор перемещения помечен noexcept.
    }
    std::cout << "=== выход из main: уничтожение вектора ===\n";

    return 0;
}
