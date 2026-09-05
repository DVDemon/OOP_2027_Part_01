// Пример 04: CQRS — Command-Query Responsibility Segregation.
//
// Принцип Бертрана Мейера: каждый метод — либо КОМАНДА (меняет состояние,
// ничего не возвращает), либо ЗАПРОС (возвращает данные, ничего не меняет).
// Смешивать нельзя.
//
// ПЛОХО — pop() одновременно удаляет элемент и возвращает его:
//
//     int pop() {
//         int val = data_.back();
//         data_.pop_back();   // состояние уже изменено
//         return val;         // если копия val бросит исключение — элемент потерян
//     }
//
// ХОРОШО — разделяем на запрос top() и команду pop().

#include <iostream>
#include <stdexcept>
#include <vector>

class Stack {
    std::vector<int> data_;

public:
    // Команда: меняет состояние, ничего не возвращает.
    void push(int value) { data_.push_back(value); }

    // Команда: удаляет верхний элемент, ничего не возвращает.
    void pop() {
        if (data_.empty()) {
            throw std::out_of_range("pop из пустого стека");
        }
        data_.pop_back();
    }

    // Запрос: возвращает верхний элемент, ничего не меняет (const).
    int top() const {
        if (data_.empty()) {
            throw std::out_of_range("top из пустого стека");
        }
        return data_.back();
    }

    // Запрос: размер, без побочных эффектов.
    std::size_t size() const { return data_.size(); }

    bool empty() const { return data_.empty(); }
};

int main() {
    Stack s;
    s.push(10);
    s.push(20);
    s.push(30);

    // Сначала запрос (top), затем команда (pop) — два отдельных шага.
    while (!s.empty()) {
        std::cout << "Верхний: " << s.top() << " (размер " << s.size() << ")\n";
        s.pop();
    }

    return 0;
}
