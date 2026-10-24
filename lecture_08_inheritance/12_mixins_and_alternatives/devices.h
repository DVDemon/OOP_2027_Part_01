#pragma once

#include <iostream>
#include <string>

// Три независимых устройства. Ни одно не является базой другого.
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

// Вариант A: множественное наследование от конкретных классов.
// Работает, но жёстко связывает MFP с реализациями устройств.
class MfpInherited : public Printer, public Scanner, public Fax {
public:
    void copy() const {
        const std::string doc = scan();
        print(doc);
    }
};

// Вариант B: композиция — MFP владеет устройствами и делегирует вызовы.
// Предпочтительный вариант, когда отношение — has-a.
class MfpComposed {
    Printer printer_;
    Scanner scanner_;
    Fax fax_;

public:
    void print(const std::string& doc) const { printer_.print(doc); }
    std::string scan() const { return scanner_.scan(); }
    void sendFax(const std::string& doc) const { fax_.send(doc); }

    void copy() const {
        const std::string doc = scan();
        print(doc);
    }
};
