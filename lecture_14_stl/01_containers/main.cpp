// Пример 01: контейнер — объект, который владеет памятью.
//
// ПЛОХО (ручное владение сырым массивом) — размер и вместимость живут в
// отдельных переменных, освобождение памяти легко забыть, а при копировании
// получается либо утечка, либо двойное освобождение:
//
//     int* data = new int[3];
//     std::size_t size = 0, capacity = 3;
//     // ... push_back руками ...
//     delete[] data;   // а если забыли или вышли по исключению?
//
// ХОРОШО — контейнер STL владеет буфером по RAII: деструктор освобождает
// память сам, копирование делает глубокую копию, перемещение передаёт владение.
// Память контейнер запрашивает у аллокатора — сменной политики выделения.
// Здесь же показываем, что метаданные контейнера лежат на стеке, а буфер — в
// куче, и что у всех контейнеров один интерфейс обхода begin()/end().

#include <array>
#include <cstddef>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Счётчики обращений к аллокатору: показывают, кто и сколько раз просит память.
// ---------------------------------------------------------------------------
namespace mem {
inline std::size_t allocations = 0;
inline std::size_t deallocations = 0;
inline std::size_t bytes = 0;

inline void reset() {
    allocations = deallocations = bytes = 0;
}

inline void report(const std::string& what) {
    std::cout << what << ": allocate=" << allocations
              << ", deallocate=" << deallocations
              << ", bytes=" << bytes << '\n';
}
}  // namespace mem

// ---------------------------------------------------------------------------
// Свой аллокатор — тонкая обёртка над std::allocator с логированием.
// Контейнеры STL параметризованы типом аллокатора: vector<T, Alloc>.
// ---------------------------------------------------------------------------
template <class T>
struct CountingAllocator {
    using value_type = T;

    CountingAllocator() = default;

    // Аллокатор должен уметь перевоплощаться в аллокатор другого типа:
    // списку нужны не int, а узлы списка.
    template <class U>
    CountingAllocator(const CountingAllocator<U>&) noexcept {}

    T* allocate(std::size_t n) {
        ++mem::allocations;
        mem::bytes += n * sizeof(T);
        return std::allocator<T>{}.allocate(n);
    }

    void deallocate(T* p, std::size_t n) noexcept {
        ++mem::deallocations;
        std::allocator<T>{}.deallocate(p, n);
    }

    template <class U>
    bool operator==(const CountingAllocator<U>&) const noexcept {
        return true;
    }
    template <class U>
    bool operator!=(const CountingAllocator<U>&) const noexcept {
        return false;
    }
};

using IntVector = std::vector<int, CountingAllocator<int>>;
using IntList = std::list<int, CountingAllocator<int>>;

// Один шаблон обходит любой контейнер: у всех есть begin() и end().
template <class Container>
void dump(const Container& c, const std::string& name) {
    std::cout << name << " [" << c.size() << "]: ";
    for (const auto& x : c)
        std::cout << x << ' ';
    std::cout << '\n';
}

int main() {
    std::cout << "=== 1. RAII: контейнер сам освобождает память ===\n";
    mem::reset();
    {
        IntVector v{1, 2, 3};
        v.push_back(4);  // capacity 3 -> 6: перераспределение
        dump(v, "vector");
        mem::report("внутри блока");
    }
    mem::report("после выхода из блока");  // allocate == deallocate

    std::cout << "\n=== 2. Метаданные — на стеке, данные — в куче ===\n";
    IntVector v{10, 20, 30};
    int local = 0;
    std::array<int, 3> a{1, 2, 3};

    std::cout << "sizeof(vector) = " << sizeof(v)
              << " (обычно три указателя: begin, end, capacity)\n";
    std::cout << "адрес объекта v        (стек): " << static_cast<const void*>(&v) << '\n';
    std::cout << "адрес локальной int    (стек): " << static_cast<const void*>(&local) << '\n';
    std::cout << "адрес буфера v.data()  (куча): "
              << static_cast<const void*>(v.data()) << '\n';
    std::cout << "sizeof(array<int,3>) = " << sizeof(a)
              << ", адрес его элементов (стек): "
              << static_cast<const void*>(a.data()) << '\n';

    std::cout << "\n=== 3. Аллокатор: сколько запросов делает контейнер ===\n";
    mem::reset();
    {
        IntVector big;
        for (int i = 0; i < 100; ++i)
            big.push_back(i);
        mem::report("vector: 100 push_back");
    }

    mem::reset();
    {
        IntList big;
        for (int i = 0; i < 100; ++i)
            big.push_back(i);
        mem::report("list:   100 push_back");
    }
    // vector просит память редкими крупными кусками (рост capacity),
    // list — отдельным запросом на каждый узел.

    std::cout << "\n=== 4. Глубокая копия и передача владения ===\n";
    mem::reset();
    {
        IntVector original{1, 2, 3};
        mem::report("исходный вектор");

        IntVector copy = original;  // копия: новый буфер, поэлементное копирование
        mem::report("после копирования");
        dump(copy, "copy");

        IntVector moved = std::move(copy);  // буфер перешёл по указателю
        mem::report("после перемещения");
        dump(moved, "moved");
        std::cout << "copy после перемещения, size = " << copy.size() << '\n';
    }

    std::cout << "\n=== 5. Один интерфейс — разные контейнеры ===\n";
    IntVector v2{1, 2, 3};
    IntList l2{4, 5, 6};
    std::array<int, 3> a2{7, 8, 9};
    dump(v2, "vector");
    dump(l2, "list  ");
    dump(a2, "array ");

    return 0;
}
