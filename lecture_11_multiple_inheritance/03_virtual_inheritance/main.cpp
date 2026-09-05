// Пример 03: виртуальное наследование — синтаксис и семантика.
//
// Демонстрирует:
//   * ключевое слово virtual в списке наследования;
//   * единственный общий экземпляр базового класса Device;
//   * отсутствие неоднозначности при обращении к c.id;
//   * порядок конструирования: виртуальная база — самой первой.
//
//        Device  (один экземпляр)
//        /    \
//    Printer  Scanner   (virtual public Device)
//        \    /
//        Copier

#include <iostream>
#include <string>

class Device {
public:
    int id;
    explicit Device(int id) : id(id) {
        std::cout << "Device(" << id << ")\n";
    }
};

// virtual public Device -> общий подобъект Device будет ОДИН.
class Printer : virtual public Device {
public:
    explicit Printer(int id) : Device(id) {
        std::cout << "Printer()\n";
    }
    void print() const { std::cout << "Printer id=" << id << '\n'; }
};

class Scanner : virtual public Device {
public:
    explicit Scanner(int id) : Device(id) {
        std::cout << "Scanner()\n";
    }
    std::string scan() const { return "doc id=" + std::to_string(id); }
};

class Copier : public Printer, public Scanner {
public:
    // Copier — most-derived класс — ОБЯЗАН вызвать конструктор Device напрямую.
    // Вызовы Device(...) внутри Printer и Scanner здесь игнорируются.
    explicit Copier(int id) : Device(id), Printer(id), Scanner(id) {
        std::cout << "Copier()\n";
    }
};

int main() {
    Copier c(42);

    std::cout << "---\n";
    // Единственная копия Device — неоднозначности нет:
    std::cout << "c.id = " << c.id << '\n';
    std::cout << "&c.Printer::Device::id == &c.Scanner::Device::id? "
              << std::boolalpha
              << (&c.Printer::id == &c.Scanner::id) << '\n';  // true

    c.print();
    std::cout << c.scan() << '\n';

    return 0;
}
