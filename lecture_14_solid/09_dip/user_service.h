#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

// DIP — принцип инверсии зависимостей.
//
// Абстракция IUserRepository ПРИНАДЛЕЖИТ слою бизнес-логики (UserService),
// а не слою инфраструктуры. Конкретные реализации (MySQL, Mongo, фейк для
// тестов) зависят от этой абстракции, а не наоборот — направление зависимости
// инвертировано.

namespace dip {

// Абстракция, от которой зависит бизнес-логика.
class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual void save(const std::string& data) = 0;
};

// Бизнес-логика зависит только от абстракции.
class UserService {
    std::unique_ptr<IUserRepository> repo_;

public:
    explicit UserService(std::unique_ptr<IUserRepository> repo)
        : repo_(std::move(repo)) {}

    void create_user(const std::string& name) {
        repo_->save(name);
    }
};

}  // namespace dip
