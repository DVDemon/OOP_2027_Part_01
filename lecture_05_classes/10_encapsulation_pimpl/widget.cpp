#include "widget.h"

#include <string>

// Полное определение Impl видно только здесь. Любые изменения этой структуры
// не требуют перекомпиляции кода, который включает только widget.h.
struct Widget::Impl {
    int counter = 0;
    std::string internal_data = "secret";  // деталь реализации, скрытая от клиента

    void compute() { ++counter; }
};

Widget::Widget() : pImpl_(std::make_unique<Impl>()) {}

// Деструктор обязан быть здесь: для уничтожения unique_ptr<Impl> нужен полный
// тип Impl. В заголовке (где Impl неполон) =default привёл бы к ошибке.
Widget::~Widget() = default;

Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::doWork() {
    pImpl_->compute();
}

int Widget::getResult() const {
    return pImpl_->counter;
}
