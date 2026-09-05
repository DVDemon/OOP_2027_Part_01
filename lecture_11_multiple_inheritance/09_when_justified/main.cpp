// Пример 09: когда множественное наследование оправдано.
//
// Демонстрирует «золотой» сценарий: один основной (полиморфный) класс +
// несколько чистых интерфейсов (как в Java). Это безопасное и распространённое
// применение MI.

#include <iostream>
#include <string>
#include <vector>

// Основной класс с данными и поведением.
class GameObject {
    std::string name_;
public:
    explicit GameObject(std::string name) : name_(std::move(name)) {}
    virtual ~GameObject() = default;
    const std::string& name() const { return name_; }
    virtual void update() { std::cout << name_ << ": update()\n"; }
};

// Чистые интерфейсы — без данных, только чисто виртуальные методы.
class IDrawable {
public:
    virtual void draw() const = 0;
    virtual ~IDrawable() = default;
};

class ICollidable {
public:
    virtual bool collidesWith(const ICollidable& other) const = 0;
    virtual ~ICollidable() = default;
};

// Один основной класс (GameObject) + два интерфейса. Ромба нет:
// у интерфейсов нет общего предка с данными.
class Player : public GameObject, public IDrawable, public ICollidable {
public:
    explicit Player(std::string name) : GameObject(std::move(name)) {}

    void update() override { std::cout << name() << ": ход игрока\n"; }
    void draw() const override { std::cout << "Рисуем игрока " << name() << '\n'; }
    bool collidesWith(const ICollidable&) const override { return false; }
};

int main() {
    Player p("Hero");

    // С объектом можно работать через любой из «контрактов»:
    GameObject* go = &p;
    IDrawable* dr = &p;

    go->update();
    dr->draw();

    // Однородные коллекции по интерфейсу:
    std::vector<IDrawable*> scene{&p};
    for (const IDrawable* obj : scene) {
        obj->draw();
    }

    return 0;
}
