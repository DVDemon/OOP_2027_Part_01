// Пример 01: концепция наследования и синтаксис.
//
// Демонстрирует:
//   * базовый класс Animal и производный Dog : public Animal;
//   * отношение «является» (is-a): собака — это животное;
//   * доступ наследника к protected-членам базового класса;
//   * обязательную инициализацию базы в списке инициализации.

#include <iostream>
#include <string>
#include <utility>

// Базовый класс. protected-члены доступны наследникам, но не "снаружи".
class Animal {
protected:
    std::string name_;
    int age_;

public:
    Animal(std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    void info() const {
        std::cout << name_ << ", возраст: " << age_ << '\n';
    }

    const std::string& name() const noexcept { return name_; }
};

// Производный класс: Dog "является" Animal (отношение is-a).
// public-наследование переносит интерфейс базового класса.
class Dog : public Animal {
    std::string breed_;

public:
    // Базовый класс обязательно инициализируется в списке инициализации.
    Dog(std::string name, int age, std::string breed)
        : Animal(std::move(name), age), breed_(std::move(breed)) {}

    // Наследник может пользоваться protected-членами базы напрямую.
    void bark() const {
        std::cout << name_ << " говорит: Гав!" << '\n';
    }

    void describe() const {
        std::cout << name_ << " — порода " << breed_ << '\n';
    }
};

int main() {
    Dog rex{"Рекс", 3, "овчарка"};

    rex.info();      // унаследованный метод Animal
    rex.bark();      // собственный метод Dog
    rex.describe();

    // Dog можно использовать там, где ожидается Animal (is-a).
    Animal& animal = rex;
    animal.info();

    std::cout << "Size of dog:" << sizeof(rex) << " Size of animal:" << sizeof(animal) << std::endl;


    return 0;
}
