// Пример 05: расположение объекта в памяти — с virtual и без.
//
// Демонстрирует:
//   * sizeof объекта при обычном и виртуальном наследовании;
//   * без virtual: две копии Device -> больше размер, разные адреса id;
//   * с virtual: один общий Device + vbptr в каждом промежуточном классе.
//
// Конкретные числа зависят от компилятора/ABI; важна КАЧЕСТВЕННАЯ картина.

#include <iostream>

namespace plain {  // обычное наследование (две копии базы)

struct Device { int id; };
struct Printer : Device { int p; };
struct Scanner : Device { int s; };
struct Copier : Printer, Scanner { int c; };

}  // namespace plain

namespace virt {  // виртуальное наследование (один общий экземпляр базы)

struct Device { int id; };
struct Printer : virtual Device { int p; };
struct Scanner : virtual Device { int s; };
struct Copier : Printer, Scanner { int c; };

}  // namespace virt

int main() {
    std::cout << "=== Обычное наследование (две копии Device) ===\n";
    std::cout << "sizeof(plain::Device)  = " << sizeof(plain::Device) << '\n';
    std::cout << "sizeof(plain::Printer) = " << sizeof(plain::Printer) << '\n';
    std::cout << "sizeof(plain::Copier)  = " << sizeof(plain::Copier) << '\n';

    plain::Copier pc{};
    // Две независимые копии Device::id -> разные адреса:
    std::cout << "&pc.Printer::id = " << &pc.Printer::id << '\n';
    std::cout << "&pc.Scanner::id = " << &pc.Scanner::id << '\n';

    std::cout << "\n=== Виртуальное наследование (один общий Device) ===\n";
    std::cout << "sizeof(virt::Device)  = " << sizeof(virt::Device) << '\n';
    std::cout << "sizeof(virt::Printer) = " << sizeof(virt::Printer)
              << "  (включает vbptr)\n";
    std::cout << "sizeof(virt::Copier)  = " << sizeof(virt::Copier) << '\n';

    virt::Copier vc{};
    // Единственная копия Device::id -> один адрес для обоих путей:
    std::cout << "&vc.Printer::id = " << &vc.Printer::id << '\n';
    std::cout << "&vc.Scanner::id = " << &vc.Scanner::id << '\n';
    std::cout << "оба пути ведут к одному id? " << std::boolalpha
              << (&vc.Printer::id == &vc.Scanner::id) << '\n';

    return 0;
}
