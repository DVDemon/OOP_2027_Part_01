# Лекция 11. Множественное и виртуальное наследование

## План лекции

1. Синтаксис множественного наследования
2. Проблема ромба (diamond problem)
3. Виртуальное наследование: синтаксис и семантика
4. Ответственность наиболее производного класса за вызов конструктора
5. Расположение объекта в памяти: с виртуальным наследованием и без
6. Корректировка указателей (pointer adjustment)
7. Примеси (mixins)
8. Альтернативы: композиция и чистые интерфейсы
9. Когда множественное наследование оправдано
10. Практические рекомендации

---

## 1. Синтаксис множественного наследования

C++ позволяет классу наследоваться от нескольких базовых классов:

```cpp
class Printer {
public:
    void print(const std::string& text) {
        std::cout << "[PRINT] " << text << std::endl;
    }
};

class Scanner {
public:
    std::string scan() {
        return "Отсканированный документ";
    }
};

class Copier : public Printer, public Scanner {
public:
    void copy() {
        std::string doc = scan();
        print(doc);
    }
};

int main() {
    Copier c;
    c.print("Hello");
    std::string s = c.scan();
    c.copy();
}
```

Порядок конструирования определяется порядком в списке наследования (слева направо), а не порядком в списке инициализации.

---

## 2. Проблема ромба (diamond problem)

Проблема возникает, когда два базовых класса имеют общего предка:

```cpp
class Device {
public:
    int id;
    Device(int id) : id(id) {
        std::cout << "Device(" << id << ")" << std::endl;
    }
};

class Printer : public Device {
public:
    Printer(int id) : Device(id) {}
    void print() { std::cout << "Printer id=" << id << std::endl; }
};

class Scanner : public Device {
public:
    Scanner(int id) : Device(id) {}
    std::string scan() { return "doc from scanner id=" + std::to_string(id); }
};

class Copier : public Printer, public Scanner {
public:
    Copier(int id) : Printer(id), Scanner(id + 100) {}
    // Copier содержит ДВЕ копии Device!
};

int main() {
    Copier c(1);
    // c.id;          // Ошибка: неоднозначность -- Printer::id или Scanner::id?
    c.Printer::id;    // OK: обращение через Printer
    c.Scanner::id;    // OK: обращение через Scanner
}
```

```
        Device          Device
          |               |
       Printer          Scanner
          \              /
            Copier
```

В объекте `Copier` существуют **две** независимые копии `Device`, что обычно нежелательно.

> Примеры: [17_Multi](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection04_05/17_Multi)

---

## 3. Виртуальное наследование

Виртуальное наследование решает проблему ромба -- обеспечивает единственный экземпляр базового класса:

```cpp
class Device {
public:
    int id;
    Device(int id) : id(id) {
        std::cout << "Device(" << id << ")" << std::endl;
    }
};

class Printer : virtual public Device {
public:
    Printer(int id) : Device(id) {
        std::cout << "Printer()" << std::endl;
    }
    void print() { std::cout << "Printer id=" << id << std::endl; }
};

class Scanner : virtual public Device {
public:
    Scanner(int id) : Device(id) {
        std::cout << "Scanner()" << std::endl;
    }
    std::string scan() { return "doc"; }
};

class Copier : public Printer, public Scanner {
public:
    // Copier ОБЯЗАН вызвать конструктор Device напрямую!
    Copier(int id) : Device(id), Printer(id), Scanner(id) {
        std::cout << "Copier()" << std::endl;
    }
};

int main() {
    Copier c(42);
    c.id;  // OK: единственная копия Device
    // Вывод:
    // Device(42)
    // Printer()
    // Scanner()
    // Copier()
}
```

```
        Device (один экземпляр)
        /    \
    Printer  Scanner   (virtual public Device)
        \    /
        Copier
```

---

## 4. Ответственность наиболее производного класса

При виртуальном наследовании конструктор виртуального базового класса вызывается **наиболее производным классом** (most-derived class), а не промежуточными:

```cpp
class A {
public:
    A(int x) { std::cout << "A(" << x << ")" << std::endl; }
};

class B : virtual public A {
public:
    B() : A(1) { std::cout << "B()" << std::endl; }
    // A(1) здесь будет проигнорирован, если B не most-derived
};

class C : virtual public A {
public:
    C() : A(2) { std::cout << "C()" << std::endl; }
    // A(2) тоже будет проигнорирован
};

class D : public B, public C {
public:
    // D -- most-derived, именно он вызывает конструктор A
    D() : A(999), B(), C() {
        std::cout << "D()" << std::endl;
    }
};

int main() {
    D d;
    // Вывод:
    // A(999)   <-- вызван из D, не из B или C
    // B()
    // C()
    // D()

    B b;
    // Вывод:
    // A(1)     <-- B сам является most-derived
    // B()
}
```

**Важно:** если most-derived класс не вызовет конструктор виртуального базового явно, будет вызван конструктор по умолчанию. Если его нет -- ошибка компиляции.

---

## 5. Расположение в памяти

### Без виртуального наследования

```
Copier (два экземпляра Device):
+-------------------------+
| Device (от Printer)     |  <- Printer::id
|   int id                |
+-------------------------+
| Printer members         |
+-------------------------+
| Device (от Scanner)     |  <- Scanner::id
|   int id                |
+-------------------------+
| Scanner members         |
+-------------------------+
| Copier members          |
+-------------------------+
```

### С виртуальным наследованием

```
Copier (один экземпляр Device):
+-------------------------+
| Printer subobject       |
|   vbptr -----+          |  <- указатель на виртуальный базовый
+-------------------------+
| Scanner subobject       |
|   vbptr -----+          |
+-------------------------+
| Copier members          |
+-------------------------+
| Device (shared)     <---+  <- единственный экземпляр
|   int id                |
+-------------------------+
```

Виртуальный базовый класс обычно размещается в конце объекта. Промежуточные классы содержат `vbptr` (virtual base pointer) для доступа к нему.

---

## 6. Корректировка указателей (pointer adjustment)

При множественном наследовании приведение указателя к разным базовым классам может менять его числовое значение:

```cpp
class A { int a; public: virtual ~A() = default; };
class B { int b; public: virtual ~B() = default; };
class C : public A, public B { int c; };

int main() {
    C obj;
    C* pc = &obj;
    A* pa = &obj;  // указывает на начало A-подобъекта (== &obj)
    B* pb = &obj;  // указывает на начало B-подобъекта (СМЕЩЁН!)

    std::cout << "C*: " << pc << std::endl;
    std::cout << "A*: " << pa << std::endl;
    std::cout << "B*: " << pb << std::endl;
    // pa == pc, но pb != pc (pb смещён на sizeof(A))

    // Компилятор автоматически корректирует указатели при приведении
    C* back = static_cast<C*>(pb);  // корректировка обратно
    assert(back == pc);
}
```

**Следствие:** нельзя использовать `reinterpret_cast` для навигации по иерархии наследования -- он не выполняет корректировку.

---

## 7. Примеси (mixins)

Примесь (mixin) -- класс, добавляющий функциональность без создания отношения is-a:

```cpp
template<typename T>
class Printable {
public:
    void print(std::ostream& os = std::cout) const {
        os << static_cast<const T&>(*this).toString() << std::endl;
    }
};

template<typename T>
class Comparable {
public:
    bool operator!=(const T& other) const {
        return !(static_cast<const T&>(*this) == other);
    }
    bool operator>(const T& other) const {
        return other < static_cast<const T&>(*this);
    }
    bool operator<=(const T& other) const {
        return !(static_cast<const T&>(*this) > other);
    }
    bool operator>=(const T& other) const {
        return !(static_cast<const T&>(*this) < other);
    }
};

class Student : public Printable<Student>, public Comparable<Student> {
    std::string name;
    double gpa;
public:
    Student(const std::string& n, double g) : name(n), gpa(g) {}
    std::string toString() const { return name + " (GPA: " + std::to_string(gpa) + ")"; }
    bool operator==(const Student& other) const { return gpa == other.gpa; }
    bool operator<(const Student& other) const { return gpa < other.gpa; }
};

int main() {
    Student a("Alice", 4.5), b("Bob", 3.8);
    a.print();           // "Alice (GPA: 4.500000)"
    std::cout << std::boolalpha;
    std::cout << (a > b) << std::endl;   // true
    std::cout << (a != b) << std::endl;  // true
}
```

Это паттерн CRTP (Curiously Recurring Template Pattern) -- безопасная и эффективная альтернатива множественному наследованию.

---

## 8. Альтернативы множественному наследованию

### Композиция

```cpp
// Вместо множественного наследования:
class MFP {  // Multi-Function Peripheral
    Printer printer;
    Scanner scanner;
    Fax fax;
public:
    void print(const std::string& doc) { printer.print(doc); }
    std::string scan() { return scanner.scan(); }
    void sendFax(const std::string& doc) { fax.send(doc); }
};
```

### Чистые интерфейсы

```cpp
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

class SmartDevice : public IPrintable, public IScannable {
public:
    void print() const override { std::cout << "Printing..." << std::endl; }
    std::string scan() const override { return "Scanned document"; }
};
```

Множественное наследование от интерфейсов (классов без данных) безопасно и широко распространено.

---

## 9. Когда множественное наследование оправдано

| Ситуация | Рекомендация |
|---|---|
| Наследование от нескольких интерфейсов | Безопасно, используйте |
| Один основной класс + интерфейсы | Нормально (как в Java) |
| Mixin-классы (CRTP) | Хорошая практика |
| Два "больших" класса с данными | Избегайте -- используйте композицию |
| Diamond без virtual | Почти всегда ошибка |

**Правило:** если вы задумались о множественном наследовании от классов с данными -- скорее всего, нужна композиция.

---

## 10. Практические рекомендации

- Множественное наследование от интерфейсов -- нормальная практика
- Виртуальное наследование добавляет накладные расходы -- используйте только при необходимости
- Помните, что most-derived класс отвечает за вызов конструктора виртуального базового
- Не используйте `reinterpret_cast` в иерархиях множественного наследования
- Тестируйте преобразования указателей, если используете MI с данными
- Предпочитайте CRTP mixins классическому MI

---

## Типичные ошибки

1. **Забыли `virtual` при наследовании** -- получили две копии базового класса
2. **Не вызвали конструктор виртуального базового из most-derived класса** -- вызван default ctor или ошибка компиляции
3. **Использовали `reinterpret_cast` вместо `static_cast`/`dynamic_cast`** -- некорректная корректировка указателей
4. **Неоднозначность имён** -- при обращении к одноимённым членам разных базовых классов без квалификации
5. **Глубокие ромбовидные иерархии** -- сложность растёт экспоненциально, рефакторьте

---

## Вопросы для самопроверки

1. Что такое проблема ромба? Нарисуйте диаграмму.
2. Как виртуальное наследование решает проблему ромба?
3. Кто отвечает за вызов конструктора виртуального базового класса?
4. Почему указатель при приведении к другому базовому классу может измениться?
5. Чем отличается расположение объекта в памяти с виртуальным наследованием и без?
6. Что такое mixin и как он реализуется через CRTP?
7. Когда множественное наследование от классов с данными безопасно?
8. Назовите три альтернативы множественному наследованию.

---

## Практическое задание

1. Реализуйте ромбовидную иерархию: `Character` -> `Warrior`, `Mage` -> `BattleMage`
   - `Character` содержит `name`, `hp`, `level`
   - `Warrior` добавляет `strength`, метод `attack()`
   - `Mage` добавляет `mana`, метод `castSpell()`
   - `BattleMage` комбинирует оба
2. Сначала реализуйте без виртуального наследования -- покажите проблему (два `Character`)
3. Затем добавьте `virtual` -- покажите решение
4. Выведите адреса подобъектов, чтобы показать корректировку указателей
5. Реализуйте альтернативный вариант через композицию и сравните подходы
