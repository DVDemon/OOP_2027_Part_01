#pragma once

#include <iostream>
#include <string>

// Чистые интерфейсы: классы без данных, только чисто виртуальные методы.
// Множественное наследование от них безопасно и распространено.
// Виртуальные функции подробно разбираются в лекции 9.

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

// Один основной класс + несколько «контрактов» (как в Java).
class SmartDevice : public IPrintable, public IScannable {
public:
    void print() const override { std::cout << "Печать...\n"; }
    std::string scan() const override { return "Документ отсканирован"; }
};
