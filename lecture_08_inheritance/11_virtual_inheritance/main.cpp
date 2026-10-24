// Пример 11: виртуальное наследование.
//
// Демонстрирует:
//   * virtual в списке наследования -> единственный подобъект общей базы;
//   * отсутствие неоднозначности при обращении к общим членам;
//   * ответственность most-derived класса за конструктор виртуальной базы;
//   * расположение объекта в памяти и накладные расходы (vbptr).
//
//        Device  (один экземпляр)
//        /    \
//    Printer  Scanner   (virtual public Device)
//        \    /
//        Copier

#include <iostream>
#include <string>

// ------------------------- часть 1: семантика -----------------------------

class Device {
public:
    int id;

    explicit Device(int id) : id(id) {
        std::cout << "Device(" << id << ")\n";
    }
    // Конструктора по умолчанию нет специально: most-derived класс обязан
    // вызвать Device(int) явно.
};

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
    // Copier — most-derived: именно он вызывает конструктор Device.
    // Вызовы Device(...) внутри Printer и Scanner здесь игнорируются.
    explicit Copier(int id) : Device(id), Printer(id), Scanner(id) {
        std::cout << "Copier()\n";
    }
};

// ------------------------- часть 2: память --------------------------------

namespace plain {  // обычное наследование: две копии Device
struct Device { int id; };
struct Printer : Device { int p; };
struct Scanner : Device { int s; };
struct Copier : Printer, Scanner { int c; };
}  // namespace plain

namespace virt {   // виртуальное наследование: один общий Device
struct Device { int id; };
struct Printer : virtual Device { int p; };
struct Scanner : virtual Device { int s; };
struct Copier : Printer, Scanner { int c; };
}  // namespace virt

int main() {
    std::cout << "=== most-derived класс вызывает виртуальную базу ===\n";
    Copier c(42);
    // Ожидаемо: Device(42) -> Printer() -> Scanner() -> Copier()

    std::cout << "\n=== единственный подобъект Device ===\n";
    std::cout << "c.id = " << c.id << '\n';  // неоднозначности нет
    std::cout << "оба пути ведут к одному id? " << std::boolalpha
              << (&c.Printer::id == &c.Scanner::id) << '\n';

    c.print();
    std::cout << c.scan() << '\n';

    std::cout << "\n=== Printer как most-derived ===\n";
    Printer p(7);
    // Здесь Printer сам наиболее производный, поэтому его Device(7) работает:
    // Device(7) -> Printer()

    std::cout << "\n=== расположение в памяти ===\n";
    std::cout << "обычное наследование:\n";
    std::cout << "  sizeof(plain::Device)  = " << sizeof(plain::Device) << '\n';
    std::cout << "  sizeof(plain::Printer) = " << sizeof(plain::Printer) << '\n';
    std::cout << "  sizeof(plain::Copier)  = " << sizeof(plain::Copier) << '\n';

    plain::Copier pc{};
    std::cout << "  &pc.Printer::id = " << &pc.Printer::id << '\n';
    std::cout << "  &pc.Scanner::id = " << &pc.Scanner::id
              << "  <- разные адреса\n";

    std::cout << "виртуальное наследование:\n";
    std::cout << "  sizeof(virt::Device)  = " << sizeof(virt::Device) << '\n';
    std::cout << "  sizeof(virt::Printer) = " << sizeof(virt::Printer)
              << "  (включает vbptr)\n";
    std::cout << "  sizeof(virt::Copier)  = " << sizeof(virt::Copier) << '\n';

    virt::Copier vc{};
    std::cout << "  &vc.Printer::id = " << &vc.Printer::id << '\n';
    std::cout << "  &vc.Scanner::id = " << &vc.Scanner::id
              << "  <- один и тот же адрес\n";

    return 0;
}
