#include "widget.h"

#include <numeric>   // «тяжёлые» заголовки — только здесь, не в widget.h
#include <vector>

// Полное определение Impl видно только в этой единице трансляции.
struct Widget::Impl {
    std::string name;
    std::vector<int> data;  // скрыто от пользователей заголовка
};

Widget::Widget(std::string name)
    : pimpl_(std::make_unique<Impl>()) {
    pimpl_->name = std::move(name);
}

// Деструктор определён здесь, где Impl — полный тип.
// Если оставить ~Widget() = default в заголовке, компилятор попытается
// инстанцировать удаление unique_ptr<Impl> с НЕПОЛНЫМ типом — ошибка.
Widget::~Widget() = default;

Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::set_name(const std::string& name) {
    pimpl_->name = name;
}

std::string Widget::get_name() const {
    return pimpl_->name;
}

void Widget::add_value(int value) {
    pimpl_->data.push_back(value);
}

int Widget::sum() const {
    return std::accumulate(pimpl_->data.begin(), pimpl_->data.end(), 0);
}
