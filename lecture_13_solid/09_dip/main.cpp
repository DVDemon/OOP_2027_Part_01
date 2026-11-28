// Пример 09: DIP — принцип инверсии зависимостей.
//
// «Модули верхнего уровня не должны зависеть от модулей нижнего уровня.
// Оба должны зависеть от абстракций».
//
// ПЛОХО — UserService жёстко содержит конкретную БД:
//
//     class UserService {
//         MySqlDatabase db_;   // жёсткая зависимость от реализации
//     public:
//         void create_user(const std::string& name) { db_.save(name); }
//     };
//
// Заменить БД нельзя, протестировать без реальной БД — тоже.
//
// ХОРОШО — UserService зависит от абстракции IUserRepository (см.
// user_service.h), а конкретная реализация подставляется в main
// (Composition Root).

#include <iostream>

#include "user_service.h"

namespace {

// Реализация-инфраструктура зависит от абстракции из слоя бизнес-логики.
class MySqlUserRepository : public dip::IUserRepository {
public:
    void save(const std::string& data) override {
        std::cout << "[MySQL] INSERT INTO users (name) VALUES ('"
                  << data << "')\n";
    }
};

class MongoUserRepository : public dip::IUserRepository {
public:
    void save(const std::string& data) override {
        std::cout << "[Mongo] db.users.insert({ name: '" << data << "' })\n";
    }
};

}  // namespace

int main() {
    // Composition Root: здесь и только здесь выбираем конкретную реализацию.
    dip::UserService mysql_service(std::make_unique<MySqlUserRepository>());
    mysql_service.create_user("Alice");

    dip::UserService mongo_service(std::make_unique<MongoUserRepository>());
    mongo_service.create_user("Bob");

    return 0;
}
