// Пример 07: приватное наследование vs композиция.
#include <iostream>

#include "stack.h"

int main() {
    stk::Stack s;
    s.push(10);
    s.push(20);
    s.push(30);

    std::cout << "размер: " << s.size() << '\n';
    std::cout << "вершина: " << s.top() << '\n';

    s.pop();
    std::cout << "после pop вершина: " << s.top() << '\n';

    while (!s.empty()) {
        std::cout << "снимаем " << s.top() << '\n';
        s.pop();
    }
    std::cout << "стек пуст: " << std::boolalpha << s.empty() << '\n';

    // Важно: интерфейс vector наружу НЕ протёк.
    // Нельзя сделать s.push_back(...), s[0], s.clear() и т.п. —
    // пользователь видит только операции стека.
    return 0;
}
