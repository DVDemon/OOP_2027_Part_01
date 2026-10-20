# Лекция 8. Наследование: одиночное, множественное, виртуальное

## План лекции

1. Концепция наследования и синтаксис
2. Модификаторы наследования: public, protected, private (таблица доступа)
3. Порядок конструирования и разрушения объектов
4. Наследование конструкторов (`using Base::Base`)
5. Сокрытие имён (name hiding) и директива `using`
6. Срезка объектов (object slicing)
7. Приватное наследование vs композиция
8. Спецификатор `final` для классов
9. Принцип "предпочитайте композицию наследованию"
10. Практические рекомендации
11. Синтаксис множественного наследования
12. Проблема ромба (diamond problem)
13. Виртуальное наследование: синтаксис и семантика
14. Ответственность наиболее производного класса за вызов конструктора
15. Расположение объекта в памяти: с виртуальным наследованием и без
16. Корректировка указателей (pointer adjustment)
17. Примеси (mixins)
18. Альтернативы: композиция и чистые интерфейсы
19. Когда множественное наследование оправдано
20. Практические рекомендации по множественному наследованию

---

## 1. Концепция наследования

Наследование -- один из трёх столпов ООП. Оно позволяет создавать новый класс на основе существующего, перенимая его данные и поведение.

```cpp
class Animal {
protected:
    std::string name;
    int age;
public:
    Animal(const std::string& n, int a) : name(n), age(a) {}
    void info() const {
        std::cout << name << ", возраст: " << age << std::endl;
    }
};

class Dog : public Animal {
    std::string breed;
public:
    Dog(const std::string& n, int a, const std::string& b)
        : Animal(n, a), breed(b) {}
    void bark() const { std::cout << name << " говорит: Гав!" << std::endl; }
};
```

> Пример: [01_inheritance_basics](01_inheritance_basics)

---

## 2. Модификаторы наследования: таблица доступа

При наследовании уровень доступа к членам базового класса может измениться в зависимости от модификатора наследования.

### Таблица доступа

| Член в базовом классе | `public`-наследование | `protected`-наследование | `private`-наследование |
|---|---|---|---|
| `public`    | `public`    | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private`   | недоступен  | недоступен  | недоступен |

```cpp
class Base {
public:
    int pub;
protected:
    int prot;
private:
    int priv;  // никогда не доступен в наследнике
};

class PubDerived : public Base {
    // pub  -> public
    // prot -> protected
    // priv -> недоступен
};

class ProtDerived : protected Base {
    // pub  -> protected
    // prot -> protected
    // priv -> недоступен
};

class PrivDerived : private Base {
    // pub  -> private
    // prot -> private
    // priv -> недоступен
};
```

> Пример: [02_access_modifiers](02_access_modifiers)

**Ключевой момент:** `private`-члены базового класса *существуют* в объекте наследника, но к ним нельзя обратиться напрямую -- только через методы базового класса.

---

## 3. Порядок конструирования и разрушения

Порядок строго определён стандартом:

**Конструирование (сверху вниз):**
1. Конструктор базового класса
2. Инициализация членов производного класса (в порядке объявления)
3. Тело конструктора производного класса

**Разрушение (снизу вверх) -- обратный порядок:**
1. Тело деструктора производного класса
2. Деструкторы членов производного класса
3. Деструктор базового класса

```cpp
class Base {
public:
    Base()  { std::cout << "Base()" << std::endl; }
    ~Base() { std::cout << "~Base()" << std::endl; }
};

class Derived : public Base {
    std::string tag{"hello"};
public:
    Derived()  { std::cout << "Derived()" << std::endl; }
    ~Derived() { std::cout << "~Derived()" << std::endl; }
};

// Вывод при создании Derived d;
// Base()
// Derived()
// При разрушении:
// ~Derived()
// ~Base()
```

> Пример: [03_construction_order](03_construction_order)

---

## 4. Наследование конструкторов (`using Base::Base`)

C++11 позволяет "унаследовать" конструкторы базового класса:

```cpp
class Base {
public:
    Base(int x) { std::cout << "Base(int): " << x << std::endl; }
    Base(int x, double y) { std::cout << "Base(int,double)" << std::endl; }
};

class Derived : public Base {
public:
    using Base::Base;  // наследуем все конструкторы Base

    // Можно добавить свои конструкторы
    Derived() : Base(0) { std::cout << "Derived()" << std::endl; }
};

int main() {
    Derived d1(42);       // вызывает Base(int) через using
    Derived d2(1, 3.14);  // вызывает Base(int, double) через using
    Derived d3;           // вызывает Derived()
}
```

**Ограничения:**
- Унаследованные конструкторы не инициализируют члены производного класса (используйте default member initializers)
- Конструктор копирования и перемещения не наследуются через `using`

> Пример: [04_inheriting_constructors](04_inheriting_constructors)

---

## 5. Сокрытие имён (name hiding) и `using`

Если в производном классе объявлена функция с тем же именем, что и в базовом, **все** перегрузки базового класса скрываются:

```cpp
class Base {
public:
    void foo(int x)    { std::cout << "Base::foo(int)" << std::endl; }
    void foo(double x) { std::cout << "Base::foo(double)" << std::endl; }
};

class Derived : public Base {
public:
    void foo(int x) { std::cout << "Derived::foo(int)" << std::endl; }
    // Base::foo(double) теперь скрыта!
};

int main() {
    Derived d;
    d.foo(42);    // OK: Derived::foo(int)
    d.foo(3.14);  // Вызовет Derived::foo(int) -- неявное преобразование!
    // d.Base::foo(3.14); // Можно вызвать явно
}
```

Чтобы вернуть перегрузки базового класса в область видимости:

```cpp
class Derived : public Base {
public:
    using Base::foo;  // "воскрешаем" все foo из Base
    void foo(int x) { std::cout << "Derived::foo(int)" << std::endl; }
};

int main() {
    Derived d;
    d.foo(42);    // Derived::foo(int)
    d.foo(3.14);  // Base::foo(double) -- теперь доступна!
}
```

> Пример: [05_name_hiding](05_name_hiding)

---

## 6. Срезка объектов (object slicing)

При копировании объекта производного класса в переменную базового типа "срезается" производная часть:

```cpp
class Base {
public:
    int x = 10;
    virtual void print() const { std::cout << "Base: " << x << std::endl; }
};

class Derived : public Base {
public:
    int y = 20;
    void print() const override { std::cout << "Derived: " << x << ", " << y << std::endl; }
};

int main() {
    Derived d;
    d.print();  // "Derived: 10, 20"

    Base b = d;  // СРЕЗКА! Копируется только Base-часть
    b.print();   // "Base: 10" -- полиморфизм потерян, y потерян

    // Правильно -- через указатель или ссылку:
    Base& ref = d;
    ref.print();  // "Derived: 10, 20" -- полиморфизм работает

    Base* ptr = &d;
    ptr->print(); // "Derived: 10, 20"
}
```

**Правило:** передавайте полиморфные объекты по указателю или ссылке, никогда по значению.

---

## 7. Приватное наследование vs композиция

Приватное наследование реализует отношение "реализовано посредством" (implemented-in-terms-of):

```cpp
// Приватное наследование
class Stack : private std::vector<int> {
public:
    void push(int val) { push_back(val); }
    void pop()         { std::vector<int>::pop_back(); }
    int top() const    { return back(); }
    bool empty() const { return std::vector<int>::empty(); }
    // Остальные методы vector недоступны пользователю Stack
};

// Эквивалент через композицию (предпочтительный вариант)
class Stack2 {
    std::vector<int> data;  // композиция
public:
    void push(int val) { data.push_back(val); }
    void pop()         { data.pop_back(); }
    int top() const    { return data.back(); }
    bool empty() const { return data.empty(); }
};
```

**Когда приватное наследование оправдано:**
- Нужен доступ к `protected`-членам базового класса
- Нужно переопределить виртуальные функции
- Оптимизация пустого базового класса (EBO)

Во всех остальных случаях -- предпочитайте композицию.

---

## 8. Спецификатор `final`

`final` запрещает наследование от класса или переопределение метода:

```cpp
class Framework final {
    // От этого класса нельзя наследоваться
public:
    void run() { std::cout << "Framework::run" << std::endl; }
};

// class MyFramework : public Framework {}; // Ошибка компиляции!

class Base {
public:
    virtual void process() final {
        // Этот метод нельзя переопределить
        std::cout << "Base::process" << std::endl;
    }
    virtual void handle() { std::cout << "Base::handle" << std::endl; }
};

class Derived : public Base {
public:
    // void process() override {} // Ошибка: process() is final
    void handle() override { std::cout << "Derived::handle" << std::endl; }
};
```

> Пример: [08_final_specifier](08_final_specifier)

---

## 9. Принцип "предпочитайте композицию наследованию"

Наследование создаёт сильнейшую связь между классами. Используйте его только когда оно действительно отражает отношение "является" (is-a):

```cpp
// Плохо: круг НЕ "является" эллипсом в плане подстановки
class Ellipse {
public:
    virtual void setRadii(double rx, double ry) { /* ... */ }
};
class Circle : public Ellipse {
    // Нарушение LSP: setRadii(3, 5) делает круг не-кругом
};

// Лучше: композиция
class Car {
    Engine engine;           // Машина СОДЕРЖИТ двигатель
    std::vector<Wheel> wheels;
public:
    void start() { engine.ignite(); }
};
```

**Критерии выбора наследования:**
1. Отношение is-a проходит тест Лисков (LSP)
2. Нужен полиморфизм через виртуальные функции
3. Базовый класс спроектирован для наследования

---

## 10. Практические рекомендации

- Используйте `public`-наследование для отношения is-a
- Избегайте глубоких иерархий наследования (больше 3 уровней -- тревожный знак)
- Всегда инициализируйте базовый класс в списке инициализации
- Помните о порядке конструирования/разрушения
- Применяйте `override` при переопределении виртуальных функций
- Не забывайте о срезке при передаче по значению

---

## Типичные ошибки

1. **Забыли вызвать конструктор базового класса** -- будет вызван конструктор по умолчанию (если есть)
2. **Срезка в контейнерах:** `std::vector<Base>` вместо `std::vector<std::unique_ptr<Base>>`
3. **Name hiding:** добавили метод с тем же именем и потеряли перегрузки базового класса
4. **Публичное наследование при отношении has-a** -- нарушение семантики

---

## Вопросы для самопроверки

1. Чем отличается `public`-, `protected`- и `private`-наследование? Составьте таблицу доступа.
2. В каком порядке вызываются конструкторы и деструкторы при наследовании?
3. Что такое name hiding и как с ним бороться?
4. Что произойдёт при присваивании `Base b = derived;`? Как этого избежать?
5. Когда приватное наследование предпочтительнее композиции?
6. Зачем нужен спецификатор `final`?
7. Что наследуется, а что нет при `using Base::Base`?
8. Почему глубокие иерархии наследования -- это проблема?

---

## Практическое задание

Реализуйте иерархию геометрических фигур:

1. Базовый класс `Shape` с полями `x`, `y` (координаты центра) и методами `area()`, `perimeter()`, `info()`
2. Класс `Rectangle : public Shape` с полями `width`, `height`
3. Класс `Square : public Rectangle` -- обоснуйте, корректно ли это с точки зрения LSP
4. Продемонстрируйте:
   - Порядок конструирования/разрушения (вывод в консоль)
   - Срезку объекта и способ её избежать
   - Использование `using Base::Base` для наследования конструкторов
   - Применение `final` к одному из классов

---

# Часть II. Множественное и виртуальное наследование

## 11. Синтаксис множественного наследования

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

## 12. Проблема ромба (diamond problem)

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

> Пример: [02_diamond_problem](02_diamond_problem)

---

## 13. Виртуальное наследование

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

## 14. Ответственность наиболее производного класса

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

## 15. Расположение в памяти

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

## 16. Корректировка указателей (pointer adjustment)

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

## 17. Примеси (mixins)

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

## 18. Альтернативы множественному наследованию

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

## 19. Когда множественное наследование оправдано

| Ситуация | Рекомендация |
|---|---|
| Наследование от нескольких интерфейсов | Безопасно, используйте |
| Один основной класс + интерфейсы | Нормально (как в Java) |
| Mixin-классы (CRTP) | Хорошая практика |
| Два "больших" класса с данными | Избегайте -- используйте композицию |
| Diamond без virtual | Почти всегда ошибка |

**Правило:** если вы задумались о множественном наследовании от классов с данными -- скорее всего, нужна композиция.

---

## 20. Практические рекомендации

- Множественное наследование от интерфейсов -- нормальная практика
- Виртуальное наследование добавляет накладные расходы -- используйте только при необходимости
- Помните, что most-derived класс отвечает за вызов конструктора виртуального базового
- Не используйте `reinterpret_cast` в иерархиях множественного наследования
- Тестируйте преобразования указателей, если используете MI с данными
- Предпочитайте CRTP mixins классическому MI

---

## Типичные ошибки: множественное наследование

1. **Забыли `virtual` при наследовании** -- получили две копии базового класса
2. **Не вызвали конструктор виртуального базового из most-derived класса** -- вызван default ctor или ошибка компиляции
3. **Использовали `reinterpret_cast` вместо `static_cast`/`dynamic_cast`** -- некорректная корректировка указателей
4. **Неоднозначность имён** -- при обращении к одноимённым членам разных базовых классов без квалификации
5. **Глубокие ромбовидные иерархии** -- сложность растёт экспоненциально, рефакторьте

---

## Вопросы для самопроверки: множественное наследование

1. Что такое проблема ромба? Нарисуйте диаграмму.
2. Как виртуальное наследование решает проблему ромба?
3. Кто отвечает за вызов конструктора виртуального базового класса?
4. Почему указатель при приведении к другому базовому классу может измениться?
5. Чем отличается расположение объекта в памяти с виртуальным наследованием и без?
6. Что такое mixin и как он реализуется через CRTP?
7. Когда множественное наследование от классов с данными безопасно?
8. Назовите три альтернативы множественному наследованию.

---

## Практическое задание: множественное наследование

1. Реализуйте ромбовидную иерархию: `Character` -> `Warrior`, `Mage` -> `BattleMage`
   - `Character` содержит `name`, `hp`, `level`
   - `Warrior` добавляет `strength`, метод `attack()`
   - `Mage` добавляет `mana`, метод `castSpell()`
   - `BattleMage` комбинирует оба
2. Сначала реализуйте без виртуального наследования -- покажите проблему (два `Character`)
3. Затем добавьте `virtual` -- покажите решение
4. Выведите адреса подобъектов, чтобы показать корректировку указателей
5. Реализуйте альтернативный вариант через композицию и сравните подходы
