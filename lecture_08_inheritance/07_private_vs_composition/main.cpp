// Пример 07: приватное наследование vs композиция.
//
// Демонстрирует:
//   * private-наследование от std::vector — отношение "реализован посредством";
//   * композицию как предпочтительную форму того же отношения;
//   * что интерфейс vector наружу не протекает ни в одном варианте.

#include <iostream>

#include "stack.h"

// Одинаковый сценарий для обоих вариантов. Шаблон здесь не нужен:
// классов всего два, поэтому пишем две обычные функции.
void demo_private(stk::StackPrivate& s) {
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
    std::cout << "стек пуст: " << std::boolalpha << s.empty() << "\n";
}

void demo_composed(stk::StackComposed& s) {
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
    std::cout << "стек пуст: " << std::boolalpha << s.empty() << "\n";
}

int main() {
    stk::StackPrivate sp;
    std::cout << "=== StackPrivate (private-наследование) ===\n";
    demo_private(sp);

    stk::StackComposed sc;
    std::cout << "\n=== StackComposed (композиция) ===\n";
    demo_composed(sc);

    // Ни в одном варианте интерфейс vector не виден снаружи:
    // sp.push_back(1);  // ОШИБКА: private-член
    // sc.push_back(1);  // ОШИБКА: нет такого метода

    // Композиция предпочтительна: связь слабее, vector не является
    // частью публичного типа, а заменить реализацию проще.
    return 0;
}
