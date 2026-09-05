// Пример 08: альтернативы множественному наследованию.
//
// Демонстрирует две безопасные альтернативы MI от «толстых» классов:
//   * композицию — MFP содержит Printer, Scanner, Fax как поля;
//   * чистые интерфейсы — наследование от классов без данных (только = 0).

#include <iostream>
#include <string>

// --- Альтернатива 1: композиция -------------------------------------------

class Printer {
public:
    void print(const std::string& doc) const {
        std::cout << "[PRINT] " << doc << '\n';
    }
};

class Scanner {
public:
    std::string scan() const { return "Отсканированный документ"; }
};

class Fax {
public:
    void send(const std::string& doc) const {
        std::cout << "[FAX] отправлено: " << doc << '\n';
    }
};

// MFP (Multi-Function Peripheral) не наследует Printer/Scanner/Fax,
// а ВЛАДЕЕТ ими. Делегирует вызовы своим полям.
class Mfp {
    Printer printer_;
    Scanner scanner_;
    Fax fax_;

public:
    void print(const std::string& doc) const { printer_.print(doc); }
    std::string scan() const { return scanner_.scan(); }
    void sendFax(const std::string& doc) const { fax_.send(doc); }
};

// --- Альтернатива 2: чистые интерфейсы ------------------------------------

class IPrintable {
public:
    virtual void print() const = 0;
    virtual ~IPrintable() = default;
};

class IScannable {
public:
    virtual std::string scan() const = 0;
    virtual ~IScannable() = default;
};

// Множественное наследование от интерфейсов (классов без данных) безопасно.
class SmartDevice : public IPrintable, public IScannable {
public:
    void print() const override { std::cout << "Печать...\n"; }
    std::string scan() const override { return "Документ отсканирован"; }
};

int main() {
    std::cout << "=== Композиция (Mfp владеет Printer/Scanner/Fax) ===\n";
    Mfp mfp;
    const std::string doc = mfp.scan();
    mfp.print(doc);
    mfp.sendFax(doc);

    std::cout << "\n=== Чистые интерфейсы ===\n";
    SmartDevice dev;
    IPrintable* p = &dev;  // работаем через интерфейс
    IScannable* s = &dev;
    p->print();
    std::cout << s->scan() << '\n';

    return 0;
}
