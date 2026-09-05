// Пример 03: std::shared_ptr — разделяемое владение через подсчёт ссылок.
#include <iostream>
#include <memory>
#include <string>

struct Widget {
    std::string name;
    explicit Widget(std::string n) : name(std::move(n)) {
        std::cout << "Widget(" << name << ") создан\n";
    }
    ~Widget() {
        std::cout << "Widget(" << name << ") разрушен\n";
    }
};

int main() {
    // make_shared: одна аллокация под объект и control block сразу.
    auto sp1 = std::make_shared<Widget>("A");
    std::cout << "use_count = " << sp1.use_count() << '\n';  // 1

    {
        auto sp2 = sp1;  // копирование — счётчик увеличивается
        std::cout << "после копии use_count = " << sp1.use_count() << '\n';  // 2
        std::cout << "sp2->name = " << sp2->name << '\n';
    }  // sp2 разрушен — счётчик уменьшается, объект ещё жив

    std::cout << "после блока use_count = " << sp1.use_count() << '\n';  // 1

    // reset уменьшает счётчик; когда он достигнет 0 — объект удаляется.
    sp1.reset();
    std::cout << "после reset sp1 пуст? " << (sp1 ? "нет" : "да") << '\n';

    // make_shared vs конструктор от сырого указателя.
    auto good = std::make_shared<Widget>("good");   // эффективно: 1 аллокация
    std::shared_ptr<Widget> ok(new Widget("ok"));   // 2 аллокации (объект + блок)
    std::cout << "good и ok созданы независимо\n";

    return 0;
}  // здесь разрушатся good и ok
