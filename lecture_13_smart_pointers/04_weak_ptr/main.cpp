// Пример 04: std::weak_ptr — борьба с циклическими ссылками.
#include <iostream>
#include <memory>

// ПЛОХО: оба указателя shared — образуется цикл, деструкторы НЕ вызываются.
namespace bad {
struct Node {
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;  // цикл!
    ~Node() { std::cout << "  ~bad::Node\n"; }
};

void leak() {
    auto a = std::make_shared<Node>();
    auto b = std::make_shared<Node>();
    a->next = b;
    b->prev = a;  // a -> b -> a: у обоих use_count == 2, при выходе станет 1
    std::cout << "  bad: use_count(a) = " << a.use_count() << '\n';
    // При выходе деструкторы НЕ вызовутся — утечка.
}
}  // namespace bad

// ХОРОШО: обратная связь — weak_ptr, цикл разорван.
namespace good {
struct Node {
    std::shared_ptr<Node> next;
    std::weak_ptr<Node> prev;   // не увеличивает счётчик
    ~Node() { std::cout << "  ~good::Node\n"; }
};

void noLeak() {
    auto a = std::make_shared<Node>();
    auto b = std::make_shared<Node>();
    a->next = b;
    b->prev = a;  // weak_ptr не участвует в подсчёте
    std::cout << "  good: use_count(a) = " << a.use_count() << '\n';
}  // деструкторы вызовутся корректно
}  // namespace good

int main() {
    std::cout << "--- bad::leak (цикл из shared_ptr) ---\n";
    bad::leak();
    std::cout << "  (деструкторы выше НЕ напечатались — память утекла)\n";

    std::cout << "--- good::noLeak (обратная связь через weak_ptr) ---\n";
    good::noLeak();

    // --- Использование weak_ptr ---
    std::cout << "--- работа с weak_ptr ---\n";
    auto shared = std::make_shared<int>(42);
    std::weak_ptr<int> weak = shared;
    std::cout << "expired? " << weak.expired() << '\n';  // 0 (false)

    if (auto locked = weak.lock()) {                     // lock даёт shared_ptr
        std::cout << "значение через lock = " << *locked << '\n';
    }

    shared.reset();                                      // удаляем объект
    std::cout << "после reset expired? " << weak.expired() << '\n';  // 1 (true)
    std::cout << "lock после удаления пуст? "
              << (weak.lock() ? "нет" : "да") << '\n';

    return 0;
}
