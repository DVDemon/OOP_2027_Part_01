#pragma once

#include <string>

namespace sql {

// QueryBuilder демонстрирует указатель this и fluent interface.
// Каждый «настраивающий» метод возвращает *this (ссылку на сам объект),
// что позволяет соединять вызовы в цепочку.
class QueryBuilder {
    std::string table_;
    std::string condition_;
    int limit_ = -1;

public:
    QueryBuilder& from(const std::string& table) {
        this->table_ = table;   // this — неявный указатель на текущий объект
        return *this;           // возвращаем ссылку на себя для цепочки
    }

    QueryBuilder& where(const std::string& cond) {
        condition_ = cond;
        return *this;
    }

    QueryBuilder& limit(int n) {
        limit_ = n;
        return *this;
    }

    // const-метод: ничего не меняет, this имеет тип const QueryBuilder*.
    std::string build() const {
        std::string sql = "SELECT * FROM " + table_;
        if (!condition_.empty()) {
            sql += " WHERE " + condition_;
        }
        if (limit_ > 0) {
            sql += " LIMIT " + std::to_string(limit_);
        }
        return sql;
    }
};

}  // namespace sql
