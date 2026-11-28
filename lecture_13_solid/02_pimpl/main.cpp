// Пример 02: идиома Pimpl — брандмауэр компиляции.
//
// ПЛОХО — приватные детали лежат прямо в заголовке:
//
//     // widget.h
//     #include <vector>      // тяжёлый заголовок утекает всем клиентам
//     class Widget {
//         std::string name_;
//         std::vector<int> data_;   // изменение этого поля => перекомпиляция
//     public:                       //                       ВСЕХ клиентов
//         // ...
//     };
//
// ХОРОШО — реализация спрятана за указателем (см. widget.h / widget.cpp):
// клиент видит только публичный интерфейс, а изменение приватных полей
// не требует его перекомпиляции.

#include <iostream>

#include "widget.h"

int main() {
    Widget w("counter");
    w.add_value(10);
    w.add_value(20);
    w.add_value(12);

    std::cout << "Имя: " << w.get_name() << '\n';
    std::cout << "Сумма: " << w.sum() << '\n';

    w.set_name("renamed");
    std::cout << "Новое имя: " << w.get_name() << '\n';

    // Перемещение разрешено (копирование — запрещено).
    Widget moved = std::move(w);
    std::cout << "После перемещения имя: " << moved.get_name() << '\n';

    return 0;
}
