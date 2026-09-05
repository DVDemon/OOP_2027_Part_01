// Пример 03: иерархия std::exception и свои классы исключений.
//
// Показываем наследование от стандартных классов и перехват по базовому типу.
#include <iostream>
#include <stdexcept>
#include <string>

// Свой класс исключения: наследуемся от std::runtime_error,
// чтобы получить готовую реализацию what() и встроиться в иерархию.
class DatabaseError : public std::runtime_error {
    int error_code_;
public:
    DatabaseError(const std::string& msg, int code)
        : std::runtime_error(msg), error_code_(code) {}

    int code() const noexcept { return error_code_; }
};

// Более конкретная ошибка — наследник DatabaseError.
class ConnectionError : public DatabaseError {
public:
    explicit ConnectionError(const std::string& host)
        : DatabaseError("Не удалось подключиться к " + host, 1001) {}
};

int main() {
    // Бросаем конкретное исключение, ловим по базовому классу —
    // полиморфизм работает и для исключений.
    try {
        throw ConnectionError("db.example.com");
    }
    catch (const DatabaseError& e) {
        // Поймали ConnectionError по ссылке на базовый DatabaseError.
        std::cout << e.what() << " [код: " << e.code() << "]\n";
    }

    // Стандартные исключения тоже ловятся по общему std::exception.
    try {
        std::string s = "ab";
        s.at(10);  // бросит std::out_of_range (наследник std::logic_error)
    }
    catch (const std::exception& e) {
        std::cout << "std::exception: " << e.what() << '\n';
    }

    return 0;
}
