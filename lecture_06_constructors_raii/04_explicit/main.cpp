// Пример 04: ключевое слово explicit.
// Запрещает неявные преобразования и copy-initialization для конструктора.
#include <iostream>
#include <string>
#include <vector>

// БЕЗ explicit: конструктор с одним параметром разрешает неявную конверсию.
class Money {
    long cents_;

public:
    Money(long cents) : cents_(cents) {}  // конвертирующий (опасно!)
    long cents() const { return cents_; }
};

// С explicit: создание только явное.
class SafeMoney {
    long cents_;

public:
    explicit SafeMoney(long cents) : cents_(cents) {}
    long cents() const { return cents_; }
};

void chargeUnsafe(const Money& m) {
    std::cout << "  списано " << m.cents() << " коп.\n";
}

void chargeSafe(const SafeMoney& m) {
    std::cout << "  списано " << m.cents() << " коп.\n";
}

int main() {
    std::cout << "--- без explicit: молчаливая конверсия ---\n";
    chargeUnsafe(500);        // 500 неявно стало Money(500) — легко ошибиться
    Money m = 1000;           // copy-initialization разрешена
    std::cout << "m = " << m.cents() << '\n';

    std::cout << "--- с explicit: только явное создание ---\n";
    // chargeSafe(500);       // ОШИБКА КОМПИЛЯЦИИ: explicit запрещает конверсию
    chargeSafe(SafeMoney(500));  // OK — явно
    // SafeMoney bad = 1000;  // ОШИБКА: copy-initialization запрещена
    SafeMoney good(1000);        // OK — direct initialization
    SafeMoney good2{2000};       // OK — direct-list-initialization
    std::cout << "good = " << good.cents() << ", good2 = " << good2.cents()
              << '\n';

    return 0;
}
