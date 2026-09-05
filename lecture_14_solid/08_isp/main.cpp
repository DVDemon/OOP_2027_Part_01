// Пример 08: ISP — принцип разделения интерфейсов.
//
// «Клиенты не должны зависеть от интерфейсов, которые они не используют».
//
// ПЛОХО — «толстый» интерфейс IMachine с print/scan/fax. Простому принтеру
// приходится реализовывать scan() и fax() заглушками-исключениями:
//
//     class IMachine {
//     public:
//         virtual void print(const Document&) = 0;
//         virtual void scan(const Document&)  = 0;
//         virtual void fax(const Document&)   = 0;
//     };
//     class SimplePrinter : public IMachine {
//         void print(const Document&) override { /* OK */ }
//         void scan(const Document&)  override { throw ...; }  // плохо!
//         void fax(const Document&)   override { throw ...; }  // плохо!
//     };
//
// ХОРОШО — разделяем на роли IPrinter / IScanner / IFax. Класс реализует
// только то, что действительно умеет.

#include <iostream>
#include <string>

using Document = std::string;

// --- Узкие интерфейсы-роли ---
class IPrinter {
public:
    virtual ~IPrinter() = default;
    virtual void print(const Document& doc) = 0;
};

class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void scan(const Document& doc) = 0;
};

class IFax {
public:
    virtual ~IFax() = default;
    virtual void fax(const Document& doc) = 0;
};

// Простой принтер реализует ровно то, что умеет.
class SimplePrinter : public IPrinter {
public:
    void print(const Document& doc) override {
        std::cout << "Печать: " << doc << '\n';
    }
};

// МФУ реализует все три роли через множественное наследование интерфейсов.
class MultiFunctionDevice : public IPrinter, public IScanner, public IFax {
public:
    void print(const Document& doc) override {
        std::cout << "[МФУ] печать: " << doc << '\n';
    }
    void scan(const Document& doc) override {
        std::cout << "[МФУ] сканирование: " << doc << '\n';
    }
    void fax(const Document& doc) override {
        std::cout << "[МФУ] факс: " << doc << '\n';
    }
};

// Клиент зависит только от нужной ему роли.
void print_document(IPrinter& printer, const Document& doc) {
    printer.print(doc);
}

int main() {
    SimplePrinter simple;
    MultiFunctionDevice mfd;

    print_document(simple, "договор.txt");
    print_document(mfd, "отчёт.txt");
    mfd.scan("паспорт.txt");
    mfd.fax("заявка.txt");

    return 0;
}
