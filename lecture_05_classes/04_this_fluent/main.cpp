// Пример 04: указатель this и fluent interface (цепочка вызовов).
#include <iostream>

#include "query_builder.h"

int main() {
    // Каждый метод возвращает *this, поэтому вызовы соединяются в цепочку.
    const std::string sql = sql::QueryBuilder{}
                                .from("users")
                                .where("age > 18")
                                .limit(10)
                                .build();

    // sql::QueryBuilder a = sql::QueryBuilder{};
    // a.from("users");
    // a.where("age > 18");
    // a.limit(10);
    std::cout << sql << '\n';
    // ожидаем: SELECT * FROM users WHERE age > 18 LIMIT 10

    // Без LIMIT и WHERE — те же методы, просто не вызываем.
    std::cout << sql::QueryBuilder{}.from("orders").build() << '\n';
    // ожидаем: SELECT * FROM orders

    return 0;
}
