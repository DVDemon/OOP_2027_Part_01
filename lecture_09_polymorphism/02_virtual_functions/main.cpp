// Пример 02: виртуальные функции — синтаксис и семантика (позднее связывание).
#include <iostream>

class Animal {
public:
    // virtual включает механизм позднего связывания (late binding):
    // фактическая реализация выбирается в runtime по типу объекта.
    virtual void speak() const {
        std::cout << "(неопределённый звук)\n";
    }
    // Невиртуальная функция — раннее связывание (по статическому типу).
    void describe() const {
        std::cout << "Это животное: ";
        speak();  // внутри виртуального вызова сработает версия потомка
    }
    virtual ~Animal() = default;
};

class Cat : public Animal {
public:
    void speak() const override {
        std::cout << "Мяу!\n";
    }
};

class Dog : public Animal {
public:
    void speak() const override {
        std::cout << "Гав!\n";
    }
};

// Принимаем по ссылке на базовый класс — полиморфизм работает.
void makeNoise(const Animal& animal) {
    animal.speak();  // какой именно speak() — решается в runtime
}

int main() {
    Cat c;
    Dog d;

    std::cout << "Через ссылку на базовый класс (виртуальный вызов):\n";
    makeNoise(c);  // "Мяу!"
    makeNoise(d);  // "Гав!"

    std::cout << "\nЧерез указатель на базовый класс:\n";
    Animal* zoo[] = {&c, &d};
    for (const Animal* a : zoo) {
        a->speak();
    }

    std::cout << "\nНевиртуальный метод вызывает виртуальный:\n";
    c.describe();  // "Это животное: Мяу!"

    return 0;
}
