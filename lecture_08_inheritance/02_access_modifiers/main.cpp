// Пример 02: модификаторы наследования public / protected / private.
#include <iostream>

class Base {
public:
    int pub = 1;

protected:
    int prot = 2;

private:
    int priv = 3;  // НИКОГДА не доступен в наследнике напрямую

public:
    // Доступ к priv возможен только через методы самого Base.
    int get_priv() const { return priv; }
};

// public-наследование: pub -> public, prot -> protected.
class PubDerived : public Base {
public:
    void show() const {
        std::cout << "PubDerived: pub=" << pub      // ok: public
                  << " prot=" << prot               // ok: protected
                  << " priv(через метод)=" << get_priv() << '\n';
        // std::cout << priv;  // ОШИБКА: priv недоступен в наследнике
    }
};

// protected-наследование: pub -> protected, prot -> protected.
class ProtDerived : protected Base {
public:
    void show() const {
        // Внутри класса доступ к pub/prot есть, но "снаружи" pub уже скрыт.
        std::cout << "ProtDerived: pub=" << pub << " prot=" << prot << '\n';
    }
};

// private-наследование: pub -> private, prot -> private.
class PrivDerived : private Base {
public:
    void show() const {
        std::cout << "PrivDerived: pub=" << pub << " prot=" << prot << '\n';
    }
};

int main() {
    PubDerived pd;
    pd.show();
    std::cout << "снаружи PubDerived.pub = " << pd.pub << '\n';  // ok: public

    ProtDerived prd;
    prd.show();
    // prd.pub;   // ОШИБКА: при protected-наследовании pub стал protected

    PrivDerived prv;
    prv.show();
    // prv.pub;   // ОШИБКА: при private-наследовании pub стал private

    return 0;
}
