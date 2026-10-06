#pragma once

#include <memory>

// PIMPL (Pointer to IMPLementation): заголовок не раскрывает детали реализации.
// Клиенту виден только указатель на неполный тип Impl — менять Impl можно,
// не перекомпилируя клиентов и не нарушая ABI.
class Widget {
public:
    Widget();
    ~Widget();  // объявлен здесь, ОПРЕДЕЛЁН в .cpp, где Impl уже полон

    // Перемещение поддерживаем, копирование по умолчанию запрещено
    // (unique_ptr некопируем). Move-операции тоже определяем в .cpp.
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) = delete;

    void doWork();
    int getResult() const;

private:
    struct Impl;                   // forward declaration: тело скрыто в .cpp
    Impl* pImpl_;  // указатель на реализацию
};
