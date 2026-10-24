// Пример 12: примеси без шаблонов, интерфейсы и альтернативы MI.
//
// Демонстрирует:
//   * MI от конкретных классов (MfpInherited) и его недостаток — жёсткая связь;
//   * композицию (MfpComposed) как предпочтительную альтернативу;
//   * примеси обычными классами (Button : Clickable, Hoverable);
//   * чистые интерфейсы (SmartDevice : IPrintable, IScannable).
//
// Когда MI оправдано:
//   * все базы, кроме одной, — интерфейсы без данных;
//   * базы — независимые примеси, не образующие ромб;
//   * класс должен реализовать несколько независимых контрактов.

#include <iostream>

#include "devices.h"
#include "interfaces.h"
#include "mixins.h"

int main() {
    std::cout << "=== A. Множественное наследование ===\n";
    MfpInherited mi;
    mi.print("документ A");
    std::cout << "scan: " << mi.scan() << '\n';
    mi.send("документ A");
    mi.copy();

    std::cout << "\n=== B. Композиция (предпочтительно) ===\n";
    MfpComposed mc;
    mc.print("документ B");
    std::cout << "scan: " << mc.scan() << '\n';
    mc.sendFax("документ B");
    mc.copy();

    std::cout << "\n=== C. Примеси без шаблонов ===\n";
    Button button{"Сохранить"};
    button.render();
    button.hover();
    std::cout << '\n';
    button.render();
    button.click();
    std::cout << '\n';

    std::cout << "\n=== D. Чистые интерфейсы ===\n";
    SmartDevice device;
    IPrintable* printable = &device;
    IScannable* scannable = &device;
    printable->print();
    std::cout << scannable->scan() << '\n';

    return 0;
}
