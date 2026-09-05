// Пример 05: четыре уровня гарантий безопасности исключений.
//
//   nothrow — функция никогда не бросает;
//   strong  — commit-or-rollback: при исключении состояние не меняется;
//   basic   — объект корректен, но в неопределённом состоянии;
//   none    — утечки/повреждение данных (так делать нельзя).
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

class IntStack {
    std::vector<int> data_;
public:
    // --- Nothrow: простые наблюдатели и swap не бросают ---
    std::size_t size() const noexcept { return data_.size(); }

    friend void swap(IntStack& a, IntStack& b) noexcept {
        std::swap(a.data_, b.data_);
    }

    // --- Basic-гарантия: при исключении на полпути часть данных добавлена ---
    // Объект остаётся корректным, но его содержимое предсказать нельзя.
    void add_basic(const std::vector<int>& items) {
        for (int item : items) {
            if (item < 0)
                throw std::invalid_argument("Отрицательное значение");
            data_.push_back(item);  // часть уже добавлена к моменту броска
        }
    }

    // --- Strong-гарантия: либо добавляем всё, либо ничего ---
    // Сначала готовим копию, и только потом фиксируем результат через swap.
    void add_strong(const std::vector<int>& items) {
        std::vector<int> copy = data_;   // работаем над копией
        for (int item : items) {
            if (item < 0)
                throw std::invalid_argument("Отрицательное значение");
            copy.push_back(item);
        }
        std::swap(data_, copy);          // фиксация: noexcept, не бросит
    }

    void print() const {
        std::cout << "  [";
        for (std::size_t i = 0; i < data_.size(); ++i)
            std::cout << (i ? ", " : "") << data_[i];
        std::cout << "]\n";
    }
};

int main() {
    // Basic: после исключения size() != 1 — часть «плохого» пакета осела.
    IntStack a;
    a.add_basic({10});
    try {
        a.add_basic({1, 2, -3, 4});  // бросит на -3, но 1 и 2 уже внутри
    } catch (const std::exception& e) {
        std::cout << "Basic: исключение «" << e.what() << "», состояние изменилось:\n";
        a.print();   // [10, 1, 2] — откат не произошёл
    }

    // Strong: после исключения состояние такое же, как до вызова.
    IntStack b;
    b.add_strong({10});
    try {
        b.add_strong({1, 2, -3, 4});  // бросит на -3, копия отброшена
    } catch (const std::exception& e) {
        std::cout << "Strong: исключение «" << e.what() << "», состояние НЕ изменилось:\n";
        b.print();   // [10] — откат произошёл автоматически
    }

    return 0;
}
