// Пример 09: паттерн Singleton через Magic Statics (C++11).
// Гарантирует единственный экземпляр класса с доступом через статический метод.
#include <iostream>
#include <string>

class Database {
public:
    // Magic Static: ленивая, потокобезопасная инициализация (C++11).
    static Database& getInstance() {
        static Database instance;  // создаётся при первом вызове
        return instance;
    }

    void query(const std::string& sql) {
        std::cout << "Выполняю: " << sql << '\n';
        ++query_count_;
    }

    int queryCount() const { return query_count_; }

    // Запрет копирования и перемещения — экземпляр должен быть единственным.
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

private:
    Database() { std::cout << "Database инициализирована\n"; }
    ~Database() { std::cout << "Database остановлена\n"; }

    int query_count_ = 0;
};

int main() {
    std::cout << "--- первый доступ создаёт экземпляр ---\n";
    Database::getInstance().query("SELECT * FROM users");
    Database::getInstance().query("SELECT * FROM orders");

    std::cout << "--- это тот же объект ---\n";
    Database& a = Database::getInstance();
    Database& b = Database::getInstance();
    std::cout << "a и b — один объект: " << std::boolalpha << (&a == &b)
              << '\n';
    std::cout << "всего запросов: " << a.queryCount() << '\n';

    std::cout << "--- конец main (деструктор после) ---\n";
    return 0;
}
