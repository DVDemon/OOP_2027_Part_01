// Пример 09: синтаксис множественного наследования, неоднозначность,
// корректировка указателей.
//
// Демонстрирует:
//   * наследование одного класса сразу от нескольких баз;
//   * порядок конструирования по списку наследования (слева направо);
//   * неоднозначность одноимённых членов разных баз и её разрешение;
//   * что указатель на разные подобъекты может иметь разные адреса.

#include <cstddef>
#include <iostream>
#include <string>

// ------------------------- часть 1: синтаксис -----------------------------

class Printer {
public:
    Printer() { std::cout << "Printer()\n"; }

    void print(const std::string& text) const {
        std::cout << "[PRINT] " << text << '\n';
    }

    void status() const { std::cout << "Printer: готов\n"; }
};

class Scanner {
public:
    Scanner() { std::cout << "Scanner()\n"; }

    std::string scan() const { return "Отсканированный документ"; }

    void status() const { std::cout << "Scanner: готов\n"; }
};

// Copier — и Printer, и Scanner. Порядок баз задаёт порядок конструкторов.
class Copier : public Printer, public Scanner {
public:
    Copier() { std::cout << "Copier()\n"; }

    void copy() const {
        const std::string doc = scan();  // из Scanner
        print(doc);                      // из Printer
    }

    // status() есть в обеих базах -> при вызове нужна квалификация.
    void status() const {
        Printer::status();
        Scanner::status();
    }
};

// ------------------- часть 2: корректировка указателей --------------------

struct A { int a = 1; };
struct B { int b = 2; };
struct C : A, B { int c = 3; };

int main() {
    std::cout << "=== синтаксис и порядок конструирования ===\n";
    Copier copier;  // Printer() -> Scanner() -> Copier()

    copier.print("Привет");
    std::cout << "scan(): " << copier.scan() << '\n';
    copier.copy();

    std::cout << "\n=== неоднозначность имён ===\n";
    // Если бы Copier не объявлял свой status(), вызов copier.status()
    // был бы ошибкой компиляции: Printer::status или Scanner::status?
    copier.Printer::status();  // явная квалификация базы
    copier.Scanner::status();
    copier.status();           // наш метод вызывает обе версии

    std::cout << "\n=== корректировка указателей ===\n";
    C obj;
    C* pc = &obj;
    A* pa = &obj;   // A-подобъект обычно лежит в начале
    B* pb = &obj;   // B-подобъект смещён на sizeof(A)

    std::cout << "C*: " << static_cast<const void*>(pc) << '\n';
    std::cout << "A*: " << static_cast<const void*>(pa) << '\n';
    std::cout << "B*: " << static_cast<const void*>(pb) << '\n';
    std::cout << "pa == pc? " << std::boolalpha
              << (static_cast<const void*>(pa) == static_cast<const void*>(pc))
              << '\n';
    std::cout << "pb == pc? "
              << (static_cast<const void*>(pb) == static_cast<const void*>(pc))
              << "  <- указатель скорректирован\n";

    // Обратное приведение тоже корректирует адрес.
    C* back = static_cast<C*>(pb);
    std::cout << "static_cast<C*>(pb) == pc? " << (back == pc) << '\n';

    // reinterpret_cast корректировку НЕ выполняет -> неверный адрес и UB.
    // Используйте static_cast / dynamic_cast.

    return 0;
}
