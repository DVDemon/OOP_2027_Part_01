// Пример 02: проблема ромба (diamond problem).
//
// Демонстрирует:
//   * что при общем предке БЕЗ virtual в объекте оказываются ДВЕ копии базы;
//   * неоднозначность при обращении к унаследованным членам;
//   * способ обойти неоднозначность через явную квалификацию (это «костыль»).
//
//        Device          Device
//          |               |
//       Printer          Scanner
//          \              /
//             Copier        <- содержит ДВЕ копии Device

#include <iostream>
#include <string>

class Device {
public:
    int id;
    explicit Device(int id) : id(id) {
        std::cout << "Device(" << id << ")\n";
    }
};

class Printer : public Device {
public:
    explicit Printer(int id) : Device(id) {}
    void print() const { std::cout << "Printer id=" << id << '\n'; }
};

class Scanner : public Device {
public:
    explicit Scanner(int id) : Device(id) {}
    std::string scan() const {
        return "doc from scanner id=" + std::to_string(id);
    }
};

// Обычное (не виртуальное) наследование -> Copier содержит ДВЕ копии Device.
class Copier : public Printer, public Scanner {
public:
    explicit Copier(int id) : Printer(id), Scanner(id + 100) {}
};

int main() {
    Copier c(1);

    // c.id;          // ОШИБКА КОМПИЛЯЦИИ: неоднозначно — Printer::id или Scanner::id?

    // Приходится квалифицировать путь явно (это симптом проблемы, а не решение):
    std::cout << "c.Printer::id = " << c.Printer::id << '\n';  // 1
    std::cout << "c.Scanner::id = " << c.Scanner::id << '\n';  // 101

    // Две независимые копии Device лежат по разным адресам:
    std::cout << "&c.Printer::id = " << &c.Printer::id << '\n';
    std::cout << "&c.Scanner::id = " << &c.Scanner::id << '\n';

    c.print();
    std::cout << c.scan() << '\n';

    return 0;
}
