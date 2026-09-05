// Пример 10: практические рекомендации (на материале практического задания).
//
// Иерархия персонажей: Character -> Warrior, Mage -> BattleMage.
// Показаны ОБА подхода и сделан вывод в духе рекомендаций лекции:
//   1) виртуальное наследование — корректное решение ромба, если MI необходимо;
//   2) композиция — предпочтительная альтернатива (BattleMageComposed).
//
// Рекомендации лекции:
//   * MI от интерфейсов — нормально; виртуальное наследование — только при нужде;
//   * most-derived класс отвечает за вызов конструктора виртуальной базы;
//   * не использовать reinterpret_cast в иерархиях MI;
//   * предпочитать композицию наследованию от классов с данными.

#include <iostream>
#include <string>

// --- Общий базовый класс с данными ----------------------------------------

class Character {
    std::string name_;
    int hp_;
    int level_;
public:
    Character(std::string name, int hp, int level)
        : name_(std::move(name)), hp_(hp), level_(level) {}
    const std::string& name() const { return name_; }
    int hp() const { return hp_; }
    int level() const { return level_; }
};

// --- Вариант А: виртуальное наследование (решение ромба) -------------------

// virtual -> в BattleMage будет ОДИН общий Character.
class Warrior : virtual public Character {
    int strength_;
public:
    Warrior(std::string name, int hp, int level, int strength)
        : Character(std::move(name), hp, level), strength_(strength) {}
    void attack() const {
        std::cout << name() << " бьёт с силой " << strength_ << '\n';
    }
};

class Mage : virtual public Character {
    int mana_;
public:
    Mage(std::string name, int hp, int level, int mana)
        : Character(std::move(name), hp, level), mana_(mana) {}
    void castSpell() const {
        std::cout << name() << " читает заклинание (мана " << mana_ << ")\n";
    }
};

class BattleMage : public Warrior, public Mage {
public:
    // most-derived: BattleMage сам вызывает конструктор виртуальной базы Character.
    BattleMage(std::string name, int hp, int level, int strength, int mana)
        : Character(name, hp, level),
          Warrior(name, hp, level, strength),
          Mage(std::move(name), hp, level, mana) {}
};

// --- Вариант Б: композиция (рекомендуемая альтернатива) --------------------

class BattleMageComposed {
    Character base_;
    int strength_;
    int mana_;
public:
    BattleMageComposed(std::string name, int hp, int level, int strength, int mana)
        : base_(std::move(name), hp, level), strength_(strength), mana_(mana) {}
    void attack() const {
        std::cout << base_.name() << " бьёт с силой " << strength_ << '\n';
    }
    void castSpell() const {
        std::cout << base_.name() << " читает заклинание (мана " << mana_ << ")\n";
    }
};

int main() {
    std::cout << "=== Вариант А: виртуальное наследование ===\n";
    BattleMage bm("Gandalf", 100, 10, 7, 50);
    bm.attack();
    bm.castSpell();
    // Единственная копия Character: hp() и level() однозначны.
    std::cout << bm.name() << ": hp=" << bm.hp()
              << ", level=" << bm.level() << '\n';

    std::cout << "\n=== Вариант Б: композиция (рекомендуется) ===\n";
    BattleMageComposed bmc("Merlin", 90, 9, 6, 60);
    bmc.attack();
    bmc.castSpell();

    return 0;
}
