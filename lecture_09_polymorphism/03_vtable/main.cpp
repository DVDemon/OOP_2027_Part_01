// Пример 03: таблица виртуальных функций (vtable) — наблюдаем устройство.
//
// Стандарт C++ НЕ описывает vtable как таковую — это деталь реализации.
// Но на практике все распространённые компиляторы используют именно её:
// у класса с виртуальными функциями есть скрытый указатель vptr на массив
// адресов реализаций (vtable). Этот пример позволяет это "увидеть".
#include <iostream>

class Animal {
public:
    virtual void speak() const { std::cout << "(звук)\n"; }
    virtual void eat() const { std::cout << "(ест)\n"; }
    virtual ~Animal() = default;
};

class Cat : public Animal {
public:
    void speak() const override { std::cout << "Мяу!\n"; }
    // eat() не переопределяем — в vtable Cat останется адрес Animal::eat
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Гав!\n"; }
    void eat() const override { std::cout << "Грызёт кость\n"; }
};

int main() {
    Cat cat;
    Dog dog;

    // Размер объекта включает скрытый vptr (обычно размер указателя).
    std::cout << "sizeof(void*) = " << sizeof(void*) << " байт\n";
    std::cout << "sizeof(Cat)   = " << sizeof(Cat)
              << " байт (содержит скрытый vptr)\n\n";

    // На типичной реализации первые байты объекта — это vptr.
    // У объектов разных классов vptr указывает на РАЗНЫЕ vtable.
    const void* cat_vptr = *reinterpret_cast<void* const*>(&cat);
    const void* dog_vptr = *reinterpret_cast<void* const*>(&dog);
    std::cout << "vptr объекта Cat: " << cat_vptr << '\n';
    std::cout << "vptr объекта Dog: " << dog_vptr << '\n';
    std::cout << "vptr различаются: " << std::boolalpha
              << (cat_vptr != dog_vptr) << "\n\n";

    // Виртуальный вызов = разыменование vptr -> взять адрес из vtable -> вызов.
    Animal* zoo[] = {&cat, &dog};
    std::cout << "Виртуальные вызовы через указатель на Animal:\n";
    for (const Animal* a : zoo) {
        a->speak();  // a->vptr -> vtable[speak] -> вызов
        a->eat();    // для Cat это Animal::eat, для Dog — Dog::eat
    }

    return 0;
}
