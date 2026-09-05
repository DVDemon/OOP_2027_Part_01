// Пример 10: инкапсуляция и идиома PIMPL.
#include <iostream>
#include <utility>

#include "widget.h"

int main() {
    Widget w;
    w.doWork();
    w.doWork();
    w.doWork();

    std::cout << "result = " << w.getResult() << '\n';  // 3

    // Перемещение: внутренний указатель переходит к w2, w остаётся пустым.
    Widget w2 = std::move(w);
    w2.doWork();
    std::cout << "result после move = " << w2.getResult() << '\n';  // 4

    // Клиент не знает и не может узнать про Impl::internal_data —
    // реализация полностью скрыта за заголовком widget.h.
    return 0;
}
