// Пример 01: концепция наследования и синтаксис.
#include <iostream>
#include <string>

// Базовый класс. protected-члены доступны наследникам, но не "снаружи".
class Animal {
protected:
    std::string name;
    int age;

public:
    Animal(const std::string& n, int a) : name(n), age(a) {}

    void info() const {
        std::cout << name << ", возраст: " << age << '\n';
    }
};

// Производный класс: Dog "является" Animal (отношение is-a).
// public-наследование переносит интерфейс базового класса.
class Dog : public Animal {
    std::string breed;

public:
    // Базовый класс обязательно инициализируется в списке инициализации.
    Dog(const std::string& n, int a, const std::string& b)
        : Animal(n, a), breed(b) {}

    // Наследник может пользоваться protected-членами базы напрямую.
    void bark() const {
        std::cout << name << " говорит: Гав!" << '\n';
    }

    void describe() const {
        std::cout << name << " — порода " << breed << '\n';
    }
};

int main() {
    Dog rex{"Рекс", 3, "овчарка"};

    rex.info();      // унаследованный метод Animal
    rex.bark();      // собственный метод Dog
    rex.describe();

    return 0;
}
