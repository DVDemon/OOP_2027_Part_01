// Пример 04: std::vector — динамический массив в куче.
//
// ПЛОХО (хранить итератор/указатель через рост) — адрес буфера меняется, и
// старый итератор указывает в освобождённую память:
//
//     auto it = v.begin();
//     v.push_back(42);              // capacity могла закончиться
//     std::cout << *it;             // неопределённое поведение
//
// ПЛОХО (рост без reserve) — много перераспределений и копирований/перемещений:
//     std::vector<int> v;
//     for (int i = 0; i < 1'000'000; ++i) v.push_back(i);
//
// ХОРОШО — помнить про capacity, заранее reserve(), не хранить итераторы через
// вставки; после реаллокации брать новые begin()/data(). Рост амортизированно
// O(1) именно потому, что буфер увеличивается в несколько раз, а не на 1.

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Тип, который считает копирования и перемещения: видно, что при реаллокации
// vector перемещает элементы, а не копирует (move-конструктор noexcept).
struct Tracked {
    static inline int copies = 0;
    static inline int moves = 0;

    int value = 0;

    explicit Tracked(int v) : value(v) {}
    Tracked(const Tracked& other) : value(other.value) { ++copies; }
    Tracked(Tracked&& other) noexcept : value(other.value) { ++moves; }

    Tracked& operator=(const Tracked&) = default;
    Tracked& operator=(Tracked&&) noexcept = default;
};

int main() {
    std::cout << "=== 1. Объект на стеке, буфер в куче ===\n";
    std::vector<int> v{1, 2, 3};
    int local = 0;
    std::cout << "адрес объекта v       (стек): " << static_cast<const void*>(&v) << '\n';
    std::cout << "адрес local           (стек): " << static_cast<const void*>(&local) << '\n';
    std::cout << "адрес буфера v.data() (куча): "
              << static_cast<const void*>(v.data()) << '\n';

    std::cout << "\n=== 2. Рост: size и capacity ===\n";
    std::vector<int> g;
    for (int i = 0; i < 10; ++i) {
        g.push_back(i);
        std::cout << "size=" << g.size() << " capacity=" << g.capacity()
                  << (g.size() == g.capacity() ? "  <- буфер заполнен" : "") << '\n';
    }
    std::cout << "capacity растёт кратно (обычно x1.5–x2), поэтому push_back\n"
                 "амортизированно O(1), а не O(n)\n";

    std::cout << "\n=== 3. Реаллокация перемещает элементы ===\n";
    Tracked::copies = Tracked::moves = 0;
    std::vector<Tracked> t;
    t.reserve(1);
    for (int i = 0; i < 5; ++i)
        t.emplace_back(i);
    std::cout << "после 5 emplace_back: copies=" << Tracked::copies
              << ", moves=" << Tracked::moves
              << " (элементы перенесены, не скопированы)\n";

    std::cout << "\n=== 4. Инвалидация: адрес буфера до и после ===\n";
    std::vector<int> inv{1, 2, 3};
    const auto old_addr = reinterpret_cast<std::uintptr_t>(inv.data());
    const auto old_capacity = inv.capacity();
    inv.push_back(4);
    const auto new_addr = reinterpret_cast<std::uintptr_t>(inv.data());
    std::cout << "capacity: " << old_capacity << " -> " << inv.capacity() << '\n';
    std::cout << "буфер: " << (old_addr == new_addr ? "тот же" : "НОВЫЙ")
              << " — старые итераторы и указатели инвалидированы\n";

    std::cout << "\n=== 5. reserve: память под все элементы сразу ===\n";
    std::vector<int> r;
    r.reserve(1000);
    const auto cap_before = r.capacity();
    for (int i = 0; i < 1000; ++i)
        r.push_back(i);
    std::cout << "capacity после reserve: " << cap_before
              << ", после 1000 push_back: " << r.capacity()
              << " (ни одного перераспределения)\n";

    std::cout << "\n=== 6. push_back vs emplace_back ===\n";
    Tracked::copies = Tracked::moves = 0;
    std::vector<Tracked> a;
    a.reserve(1);
    a.push_back(Tracked{1});  // временный объект + перемещение в вектор
    std::cout << "push_back(Tracked{1}): copies=" << Tracked::copies
              << ", moves=" << Tracked::moves << '\n';

    Tracked::copies = Tracked::moves = 0;
    std::vector<Tracked> b;
    b.reserve(1);
    b.emplace_back(1);  // объект сразу строится в буфере
    std::cout << "emplace_back(1):       copies=" << Tracked::copies
              << ", moves=" << Tracked::moves << '\n';

    std::cout << "\n=== 7. Вставка и удаление в середине — O(n) ===\n";
    std::vector<int> m{10, 20, 30, 40};
    m.insert(m.begin() + 1, 15);  // сдвиг хвоста вправо
    m.erase(m.begin() + 2);       // сдвиг хвоста влево
    std::cout << "после insert/erase: ";
    for (int x : m) std::cout << x << ' ';
    std::cout << '\n';

    std::cout << "\n=== 8. at() проверяет границы, [] — нет ===\n";
    try {
        std::cout << m.at(100) << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "m.at(100) бросило std::out_of_range\n";
    }

    std::cout << "\n=== 9. shrink_to_fit: вернуть лишнюю память ===\n";
    std::vector<int> s;
    s.reserve(100);
    for (int i = 0; i < 10; ++i) s.push_back(i);
    std::cout << "до shrink_to_fit: size=" << s.size()
              << ", capacity=" << s.capacity() << '\n';
    s.shrink_to_fit();
    std::cout << "после:             size=" << s.size()
              << ", capacity=" << s.capacity() << " (не гарантировано стандартом)\n";

    return 0;
}
