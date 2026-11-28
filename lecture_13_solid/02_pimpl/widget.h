#pragma once

#include <memory>
#include <string>

// Идиома Pimpl (Pointer to Implementation).
//
// Заголовок НЕ включает «тяжёлые» заголовки (<vector>, <algorithm> и т.п.)
// и не раскрывает приватные поля. Все детали спрятаны в widget.cpp.
// Изменение реализации не требует перекомпиляции клиентов этого заголовка.
class Widget {
public:
    explicit Widget(std::string name);
    ~Widget();  // ВАЖНО: объявлен здесь, определён в .cpp (см. README)

    // Копирование запрещаем, перемещение разрешаем.
    Widget(const Widget&) = delete;
    Widget& operator=(const Widget&) = delete;
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    void set_name(const std::string& name);
    std::string get_name() const;

    // Добавляет значение во внутреннее (скрытое) хранилище.
    void add_value(int value);
    int sum() const;

private:
    struct Impl;                   // forward declaration
    std::unique_ptr<Impl> pimpl_;  // непрозрачный указатель
};
