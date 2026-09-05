// Пример 03: модификаторы доступа public / protected / private.
#include <iostream>
#include <string>

// Account: поля private (инкапсуляция), публичный интерфейс контролирует инварианты.
class Account {
public:
    Account(std::string owner, double balance) : owner_(std::move(owner)), balance_(balance) {}

    // public: часть интерфейса, доступна всем.
    std::string getOwner() const { return owner_; }
    double getBalance() const { return balance_; }

    void deposit(double amount) {
        if (amount > 0) {            // инвариант: пополнять можно только положительной суммой
            balance_ += amount;
        }
    }

protected:
    // protected: доступно классу и наследникам, но не извне.
    void setOwner(const std::string& name) { owner_ = name; }

private:
    // private: только сам класс. Внешний код не может испортить состояние.
    std::string owner_;
    double balance_ = 0.0;
};

// Наследник имеет доступ к protected-методу setOwner, но НЕ к private-полям напрямую.
class AdminAccount : public Account {
public:
    AdminAccount(std::string owner, double balance) : Account(std::move(owner), balance) {}

    void rename(const std::string& name) {
        setOwner(name);     // OK: protected доступен наследнику
        // owner_ = name;   // ОШИБКА: 'owner_' is private в базовом классе
    }
};

int main() {
    Account acc{"Alice", 100.0};
    acc.deposit(50.0);
    acc.deposit(-1000.0);  // отклонено инвариантом: баланс не уйдёт в минус

    std::cout << "owner=" << acc.getOwner() << " balance=" << acc.getBalance() << '\n';
    // acc.balance_ = 1e9;     // ОШИБКА: private
    // acc.setOwner("Eve");    // ОШИБКА: protected, недоступно извне

    AdminAccount admin{"Bob", 500.0};
    admin.rename("Robert");
    std::cout << "admin owner=" << admin.getOwner()
              << " balance=" << admin.getBalance() << '\n';

    return 0;
}
