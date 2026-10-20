// Пример 10: практические рекомендации и типичные ошибки.
// Все опасные конструкции закомментированы — показываем правильные варианты.
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// Рекомендация: shared_ptr, который не нужно копировать, передавать по const&.
void inspect(const std::shared_ptr<int>& p) {
    std::cout << "inspect: *p = " << *p
              << ", use_count = " << p.use_count() << '\n';  // счётчик не растёт
}

// Рекомендация: передача владения unique_ptr — по значению.
void consume(std::unique_ptr<int> p) {
    std::cout << "consume: владею значением " << *p << '\n';
}  // здесь ресурс освобождается

int main() {
    // --- Рекомендация: всегда make_unique / make_shared ---
    auto u = std::make_unique<int>(10);
    auto s = std::make_shared<int>(20);

    // --- Передача shared_ptr по const& (счётчик не увеличивается) ---
    inspect(s);  // use_count остаётся 1

    // --- Передача владения unique_ptr через move ---
    consume(std::move(u));
    std::cout << "после consume u пуст? " << (u ? "нет" : "да") << '\n';

    // --- unique_ptr в контейнере: только через std::move ---
    std::vector<std::unique_ptr<int>> v;
    auto p = std::make_unique<int>(42);
    // v.push_back(p);            // ОШИБКА: копирование unique_ptr запрещено
    v.push_back(std::move(p));    // OK
    std::cout << "в векторе: " << *v.front() << '\n';

    // --- ТИПИЧНАЯ ОШИБКА 1: два shared_ptr из одного сырого указателя ---
    // int* raw = new int(1);
    // std::shared_ptr<int> sp1(raw);
    // std::shared_ptr<int> sp2(raw);   // два независимых control block ->
    //                                  // двойное освобождение, крах.
    // Правильно — один владелец, остальные через копирование shared_ptr:
    auto sp1 = std::make_shared<int>(1);
    auto sp2 = sp1;  // общий control block
    std::cout << "sp1/sp2 use_count = " << sp1.use_count() << '\n';  // 2

    // --- ТИПИЧНАЯ ОШИБКА 2: shared_ptr(this) в стеке ---
    // class Bad { std::shared_ptr<Bad> self; Bad() : self(this) {} };
    // Bad b;  // shared_ptr попытается удалить объект, лежащий на стеке -> UB.
    // Правильно — enable_shared_from_this (см. пример 05).

    std::cout << "Все рекомендации продемонстрированы.\n";
    return 0;
}
