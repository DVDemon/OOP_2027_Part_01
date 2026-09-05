// Пример 01: синтаксис множественного наследования.
//
// Демонстрирует:
//   * наследование одного класса сразу от нескольких баз;
//   * объединение функциональности баз в производном классе;
//   * порядок конструирования (слева направо по списку наследования).

#include <iostream>
#include <string>

// Базовый класс: умеет печатать.
class Printer {
public:
    Printer() { std::cout << "Printer()\n"; }
    void print(const std::string& text) const {
        std::cout << "[PRINT] " << text << '\n';
    }
};

// Базовый класс: умеет сканировать.
class Scanner {
public:
    Scanner() { std::cout << "Scanner()\n"; }
    std::string scan() const { return "Отсканированный документ"; }
};

// Множественное наследование: Copier — это и Printer, и Scanner.
// Он получает интерфейсы обоих базовых классов.
class Copier : public Printer, public Scanner {
public:
    Copier() { std::cout << "Copier()\n"; }

    // Метод копирования использует возможности обеих баз.
    void copy() const {
        const std::string doc = scan();  // из Scanner
        print(doc);                      // из Printer
    }
};

int main() {
    // Порядок конструирования: Printer, затем Scanner, затем Copier —
    // строго по списку наследования (public Printer, public Scanner).
    Copier c;

    std::cout << "---\n";
    c.print("Привет");                       // унаследовано от Printer
    std::cout << "scan(): " << c.scan() << '\n';  // унаследовано от Scanner
    c.copy();                                // собственный метод Copier

    return 0;
}
