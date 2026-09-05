// Пример 03: делегирующие конструкторы (C++11).
// Один конструктор вызывает другой того же класса, чтобы не дублировать код.
#include <iostream>
#include <string>

class Connection {
    std::string host_;
    int port_;
    bool ssl_;

public:
    // «Полный» конструктор: единственное место с реальной логикой.
    Connection(const std::string& host, int port, bool ssl)
        : host_(host), port_(port), ssl_(ssl) {
        std::cout << "Подключение к " << host_ << ":" << port_
                  << (ssl_ ? " (SSL)" : "") << '\n';
    }

    // Делегирование к полному конструктору: ssl = false.
    Connection(const std::string& host, int port)
        : Connection(host, port, false) {}

    // Делегирование: порт 80, без SSL.
    explicit Connection(const std::string& host)
        : Connection(host, 80, false) {}

    // Делегирование: все значения по умолчанию.
    Connection() : Connection("localhost") {}
};

int main() {
    std::cout << "--- разные уровни делегирования ---\n";
    Connection c1("example.com", 443, true);  // полный
    Connection c2("example.com", 8080);        // -> (host, 8080, false)
    Connection c3("api.local");                // -> (host, 80, false)
    Connection c4;                             // -> ("localhost", 80, false)

    (void)c1;
    (void)c2;
    (void)c3;
    (void)c4;
    return 0;
}
