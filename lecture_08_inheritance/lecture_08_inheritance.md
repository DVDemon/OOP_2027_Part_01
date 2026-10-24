# Лекция 8. Наследование: одиночное, множественное, виртуальное

## План лекции

1. Концепция наследования и синтаксис
2. Модификаторы наследования: `public`, `protected`, `private`
3. Порядок конструирования и разрушения
4. Наследование конструкторов (`using Base::Base`)
5. Сокрытие имён и директива `using`
6. Срезка объектов (object slicing)
7. Приватное наследование vs композиция
8. `final` и «предпочитайте композицию наследованию»
9. Множественное наследование: синтаксис и неоднозначность
10. Проблема ромба (diamond problem)
11. Виртуальное наследование
12. Примеси, интерфейсы и альтернативы MI

Шаблоны в лекции не используются — их разбор в лекции 15.

---

## 1. Концепция наследования и синтаксис

Наследование — один из трёх столпов ООП. Оно позволяет создавать новый
класс на основе существующего, перенимая его данные и поведение.
Ключевое отношение — **is-a**: наследник обязан быть частным случаем базы.

```cpp
class Animal {
protected:
    std::string name_;
    int age_;

public:
    Animal(std::string name, int age)
        : name_(std::move(name)), age_(age) {}

    void info() const {
        std::cout << name_ << ", возраст: " << age_ << '\n';
    }
};

class Dog : public Animal {
    std::string breed_;

public:
    Dog(std::string name, int age, std::string breed)
        : Animal(std::move(name), age), breed_(std::move(breed)) {}

    void bark() const { std::cout << name_ << " говорит: Гав!\n"; }
};

int main() {
    Dog rex{"Рекс", 3, "овчарка"};
    rex.info();          // унаследовано
    rex.bark();          // собственное
    Animal& a = rex;     // Dog можно использовать как Animal
    a.info();
}
```

> Пример: [01_inheritance_basics](01_inheritance_basics)

**Важно:** `public`-наследование — это не «копирование кода», а заявление
о типе. Если наследник не может вести себя как база, наследование выбрано
неверно (см. раздел 8).

---

## 2. Модификаторы наследования

При наследовании уровень доступа к членам базового класса может измениться.

### Таблица доступа

| Член в базовом классе | `public`-наследование | `protected`-наследование | `private`-наследование |
|---|---|---|---|
| `public`    | `public`    | `protected` | `private` |
| `protected` | `protected` | `protected` | `private` |
| `private`   | недоступен  | недоступен  | недоступен |

```cpp
class Base {
public:
    int pub = 1;
protected:
    int prot = 2;
private:
    int priv = 3;  // недоступен наследнику ни при каком модификаторе
public:
    int get_priv() const { return priv; }
};

class PubDerived : public Base {
    // pub -> public, prot -> protected
};

class ProtDerived : protected Base {
    // pub -> protected, prot -> protected
};

class PrivDerived : private Base {
    // pub -> private, prot -> private
};
```

> Пример: [02_access_modifiers](02_access_modifiers)

**Ключевой момент:** `private`-члены базы *существуют* в объекте наследника,
но к ним нельзя обратиться напрямую — только через методы самой базы.

По умолчанию для `class` наследование приватное, для `struct` — публичное.
Для отношения is-a почти всегда нужен явный `public`.

---

## 3. Порядок конструирования и разрушения

Порядок строго определён стандартом.

**Конструирование (сверху вниз):**

1. Конструктор базового класса (и его членов).
2. Члены производного класса — в порядке объявления.
3. Тело конструктора производного класса.

**Разрушение — строго наоборот:**

1. Тело деструктора производного класса.
2. Члены производного класса.
3. Деструктор базового класса.

```cpp
class Member {
    std::string tag_;
public:
    explicit Member(std::string tag) : tag_(std::move(tag)) {
        std::cout << "  Member(" << tag_ << ")\n";
    }
    ~Member() { std::cout << "  ~Member(" << tag_ << ")\n"; }
};

class Base {
    Member m_{"Base::m"};
public:
    Base()  { std::cout << "Base()\n"; }
    ~Base() { std::cout << "~Base()\n"; }
};

class Derived : public Base {
    Member m_{"Derived::m"};
public:
    Derived()  { std::cout << "Derived()\n"; }
    ~Derived() { std::cout << "~Derived()\n"; }
};
```

Вывод при создании и разрушении `Derived d;`:

```
  Member(Base::m)
Base()
  Member(Derived::m)
Derived()
~Derived()
  ~Member(Derived::m)
~Base()
  ~Member(Base::m)
```

> Пример: [03_construction_order](03_construction_order)

**Ловушка:** порядок определяется списком баз и объявлением полей, а не
порядком в списке инициализации. Несовпадение — источник багов, когда
инициализатор одного поля зависит от другого.

---

## 4. Наследование конструкторов

C++11 позволяет унаследовать конструкторы базы директивой `using`.

```cpp
class Base {
public:
    explicit Base(int x) { std::cout << "Base(int): " << x << '\n'; }
    Base(int x, double y) { std::cout << "Base(int,double)\n"; }
};

class Derived : public Base {
    std::string label_{"<нет>"};  // default member initializer

public:
    using Base::Base;  // наследуем оба конструктора Base

    Derived() : Base(0), label_{"по умолчанию"} {
        std::cout << "Derived()\n";
    }
};

int main() {
    Derived d1{42};       // Base(int)
    Derived d2{1, 3.14};  // Base(int,double)
    Derived d3;           // Derived()
}
```

**Ограничения:**

- унаследованный конструктор **не** инициализирует поля наследника —
  для них работают default member initializers;
- копирующий и перемещающий конструкторы через `using` не наследуются;
- если конструктор базы совпадает по сигнатуре с конструктором наследника,
  побеждает собственный.

> Пример: [04_inheriting_constructors](04_inheriting_constructors)

---

## 5. Сокрытие имён и директива `using`

Если в производном классе объявлена функция с тем же именем, что и в базе,
**все** перегрузки базы скрываются. Это не перегрузка и не переопределение,
а именно сокрытие имени.

```cpp
class Base {
public:
    void foo(int x)    { std::cout << "Base::foo(int): " << x << '\n'; }
    void foo(double x) { std::cout << "Base::foo(double): " << x << '\n'; }
};

class HidingDerived : public Base {
public:
    void foo(int x) { std::cout << "HidingDerived::foo(int)\n"; }
    // Base::foo(double) скрыта
};

class UsingDerived : public Base {
public:
    using Base::foo;  // возвращаем перегрузки базы в область видимости
    void foo(int x) { std::cout << "UsingDerived::foo(int)\n"; }
};
```

```cpp
HidingDerived h;
const double pi = 3.14;
h.foo(42);      // HidingDerived::foo(int)
h.foo(pi);      // HidingDerived::foo(int)! 3.14 -> 3 — ловушка
h.Base::foo(pi);  // обойти можно только явной квалификацией

UsingDerived u;
u.foo(pi);      // Base::foo(double) — перегрузка снова видна
```

> Пример: [05_name_hiding](05_name_hiding)

**Правило:** если наследник добавляет метод с уже существующим именем,
почти всегда нужен `using Base::имя;`.

---

## 6. Срезка объектов

При копировании объекта производного класса в переменную базового типа
производная часть отбрасывается: теряются и поля, и переопределения.

```cpp
class Base {
public:
    int x = 10;
    virtual ~Base() = default;
    virtual void print() const {
        std::cout << "Base: x=" << x << '\n';
    }
};

class Derived : public Base {
public:
    int y = 20;
    double weight = 1.5;
    void print() const override {
        std::cout << "Derived: x=" << x << ", y=" << y << '\n';
    }
};

void by_value(Base b) { b.print(); }        // СРЕЗКА
void by_ref(const Base& b) { b.print(); }   // без срезки
```

```cpp
Derived d;
Base b = d;            // СРЕЗКА: копируется только Base-часть
b.print();             // Base: x=10
by_value(d);           // СРЕЗКА в параметре

Base arr[2] = {d, d};  // СРЕЗКА в массиве

by_ref(d);             // Derived: сохраняется динамический тип
Base& ref = d;
Base* ptr = &d;
```

> Пример: [06_object_slicing](06_object_slicing)

**Правило:** полиморфные объекты передаются по ссылке или указателю, но не
по значению. `sizeof(Base) < sizeof(Derived)` — наглядная иллюстрация того,
что в базовой переменной просто нет места для производных полей.

---

## 7. Приватное наследование vs композиция

Приватное наследование выражает отношение «реализован посредством»
(implemented-in-terms-of), а не is-a.

```cpp
// Вариант 1: приватное наследование
class StackPrivate : private std::vector<int> {
public:
    void push(int v)    { push_back(v); }
    void pop()          { pop_back(); }
    int  top() const    { return back(); }
    bool empty() const  { return std::vector<int>::empty(); }
};

// Вариант 2: композиция — предпочтительно
class StackComposed {
    std::vector<int> data_;
public:
    void push(int v)    { data_.push_back(v); }
    void pop()          { data_.pop_back(); }
    int  top() const    { return data_.back(); }
    bool empty() const  { return data_.empty(); }
};
```

В обоих вариантах наружу виден только интерфейс стека: `push_back`, `[]`,
`clear` недоступны.

> Пример: [07_private_vs_composition](07_private_vs_composition)

**Когда приватное наследование оправдано:**

- нужен доступ к `protected`-членам базы;
- нужно переопределить виртуальные функции базы;
- оптимизация пустого базового класса (EBO).

Во всех остальных случаях — композиция: она слабее связывает классы,
не влияет на публичный тип и проще в сопровождении.

---

## 8. `final` и «предпочитайте композицию наследованию»

### `final`

`final` запрещает наследование от класса или переопределение метода.

```cpp
class Framework final {
public:
    void run() const { std::cout << "Framework::run\n"; }
};
// class MyFramework : public Framework {};  // ОШИБКА

class Base {
public:
    virtual ~Base() = default;
    virtual void process() final { std::cout << "Base::process\n"; }
    virtual void handle() { std::cout << "Base::handle\n"; }
};

class Derived : public Base {
public:
    // void process() override {}  // ОШИБКА: process() помечен final
    void handle() override { std::cout << "Derived::handle\n"; }
};
```

`final` фиксирует точку, дальше которой иерархия не расширяется, и даёт
компилятору право девиртуализировать вызовы.

### Композиция вместо наследования

Наследование создаёт самую сильную связь между классами. Используйте его
только для отношения is-a, проходящего тест Лисков (LSP).

```cpp
// Плохо: машина НЕ является двигателем
class Car : public Engine { /* ... */ };

// Хорошо: машина СОДЕРЖИТ двигатель
class Car {
    Engine engine_;
    std::string model_;
public:
    std::string start() const { return model_ + ": " + engine_.ignite(); }
};
```

Классическая LSP-ловушка:

```cpp
class Ellipse {
public:
    virtual void setRadii(double rx, double ry);
};

// Круг «математически» эллипс, но контракт нарушен:
// setRadii(3, 5) превращает круг в не-круг.
class Circle : public Ellipse { /* ... */ };
```

> Пример: [08_final_and_composition](08_final_and_composition)

**Критерии выбора наследования:**

1. Отношение is-a проходит тест Лисков.
2. Нужен полиморфизм через виртуальные функции.
3. Базовый класс спроектирован для наследования.

---

## 9. Множественное наследование: синтаксис и неоднозначность

C++ позволяет классу наследоваться от нескольких баз.

```cpp
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

class Copier : public Printer, public Scanner {
public:
    Copier() { std::cout << "Copier()\n"; }
    void copy() const {
        const std::string doc = scan();
        print(doc);
    }
    void status() const {          // разрешаем неоднозначность
        Printer::status();
        Scanner::status();
    }
};
```

Порядок конструирования — `Printer()`, `Scanner()`, `Copier()` — задаётся
**списком наследования** (слева направо), а не списком инициализации.

### Неоднозначность

Если `status()` есть в обеих базах, вызов `c.status()` без собственного
метода в `Copier` — ошибка компиляции. Разрешение:

```cpp
c.Printer::status();   // явная квалификация
c.Scanner::status();
```

### Корректировка указателей

Каждая база — отдельный подобъект со своим смещением в объекте наследника.

```cpp
struct A { int a = 1; };
struct B { int b = 2; };
struct C : A, B { int c = 3; };

C obj;
C* pc = &obj;
A* pa = &obj;   // обычно совпадает с pc
B* pb = &obj;   // СМЕЩЁН на sizeof(A)

assert(static_cast<C*>(pb) == pc);  // обратная корректировка
```

`static_cast` и `dynamic_cast` корректируют адрес автоматически.
`reinterpret_cast` — **нет**, поэтому для навигации по иерархии он
непригоден: получите неверный адрес и UB.

> Пример: [09_multiple_inheritance](09_multiple_inheritance)

---

## 10. Проблема ромба

Проблема возникает, когда два промежуточных класса имеют общего предка.

```cpp
class Device {
public:
    int id;
    explicit Device(int id) : id(id) {
        std::cout << "Device(" << id << ")\n";
    }
};

class Printer : public Device {
public:
    explicit Printer(int id) : Device(id) {}
    void print() const { std::cout << "Printer id=" << id << '\n'; }
};

class Scanner : public Device {
public:
    explicit Scanner(int id) : Device(id) {}
    std::string scan() const {
        return "doc from scanner id=" + std::to_string(id);
    }
};

class Copier : public Printer, public Scanner {
public:
    // Printer::Device и Scanner::Device — ДВЕ независимые копии
    explicit Copier(int id) : Printer(id), Scanner(id + 100) {}
};
```

```
        Device          Device
          |               |
       Printer          Scanner
          \              /
             Copier
```

В объекте `Copier` две копии `Device`:

```cpp
// c.id;             // ОШИБКА: неоднозначно
c.Printer::id;       // 1
c.Scanner::id;       // 101 — независимое состояние
```

> Пример: [10_diamond_problem](10_diamond_problem)

**Почему это проблема:** дублируются данные, растёт размер объекта, а
изменение «одной» копии не меняет другую. Квалификация имён лишь маскирует
дублирование — это не решение.

---

## 11. Виртуальное наследование

`virtual` в списке наследования гарантирует **единственный** подобъект
общей базы.

```cpp
class Device {
public:
    int id;
    explicit Device(int id) : id(id) { std::cout << "Device(" << id << ")\n"; }
};

class Printer : virtual public Device {
public:
    explicit Printer(int id) : Device(id) { std::cout << "Printer()\n"; }
    void print() const { std::cout << "Printer id=" << id << '\n'; }
};

class Scanner : virtual public Device {
public:
    explicit Scanner(int id) : Device(id) { std::cout << "Scanner()\n"; }
    std::string scan() const { return "doc id=" + std::to_string(id); }
};

class Copier : public Printer, public Scanner {
public:
    // Copier — most-derived: именно он вызывает конструктор Device.
    explicit Copier(int id) : Device(id), Printer(id), Scanner(id) {
        std::cout << "Copier()\n";
    }
};
```

```
        Device  (один экземпляр)
        /    \
    Printer  Scanner   (virtual public Device)
        \    /
        Copier
```

```cpp
Copier c(42);
c.id;   // OK: неоднозначности нет
&c.Printer::id == &c.Scanner::id;  // true
```

### Ответственность наиболее производного класса

При виртуальном наследовании конструктор виртуальной базы вызывает только
**most-derived** класс. Вызовы `Device(...)` внутри `Printer` и `Scanner`
игнорируются, когда они не являются most-derived. Если объект `Printer`
создаётся напрямую, он сам most-derived — и его `Device(id)` работает.

```cpp
// D most-derived -> вызывает A(999), а не A(1)/A(2) из B/C
class D : public B, public C {
public:
    D() : A(999), B(), C() {}
};
```

### Расположение в памяти

- **Обычное наследование:** подобъекты баз лежат подряд, каждая копия
  общего предка занимает своё место.
- **Виртуальное наследование:** промежуточные классы хранят `vbptr`
  (указатель на виртуальную базу), а общая база размещается отдельно,
  обычно в конце объекта.

```cpp
struct Device { int id; };
struct Printer : virtual Device { int p; };
struct Copier : Printer, Scanner { int c; };
// sizeof(Printer) > sizeof(Device) + sizeof(int) — из-за vbptr
```

Конкретные числа зависят от ABI; важно качественное различие: объект
становится больше, а доступ к виртуальной базе — косвенным.

> Пример: [11_virtual_inheritance](11_virtual_inheritance)

**Цена:** `vbptr`, косвенный доступ, более сложные конструкторы. Применяйте
`virtual` только там, где общий подобъект действительно нужен.

---

## 12. Примеси, интерфейсы и альтернативы

### Примеси

Примесь (mixin) — небольшой класс, добавляющий поведение, но не создающий
отношение is-a. Здесь примеси обычные, без шаблонов.

```cpp
class Clickable {
public:
    void click() const { std::cout << "клик\n"; }
};

class Hoverable {
public:
    void hover() const { std::cout << "наведение\n"; }
};

class Button : public Clickable, public Hoverable {
    std::string label_;
public:
    explicit Button(std::string label) : label_(std::move(label)) {}
    void render() const { std::cout << "[" << label_ << "] "; }
};
```

Ромба не возникает: у примесей нет общей базы и общего состояния.

### Чистые интерфейсы

Классы без данных, только чисто виртуальные методы. Множественное
наследование от них безопасно и широко распространено.

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
    void print() const override { std::cout << "Печать...\n"; }
    std::string scan() const override { return "Документ отсканирован"; }
};
```

Виртуальные функции подробно разбираются в лекции 9; здесь важен состав
типа: один основной класс и несколько независимых контрактов.

### Композиция

Если класс **владеет** подсистемами, это has-a, и нужна композиция:

```cpp
class MfpComposed {
    Printer printer_;
    Scanner scanner_;
    Fax fax_;
public:
    void print(const std::string& doc) const { printer_.print(doc); }
    std::string scan() const { return scanner_.scan(); }
    void sendFax(const std::string& doc) const { fax_.send(doc); }
};
```

### Когда MI оправдано

| Ситуация | Рекомендация |
|---|---|
| Один основной класс + интерфейсы без данных | безопасно, используйте |
| Независимые примеси поведения | безопасно, если нет общей базы |
| Два «больших» класса с данными | избегайте — нужна композиция |
| Ромб без `virtual` | почти всегда ошибка |

> Пример: [12_mixins_and_alternatives](12_mixins_and_alternatives)

**Правило:** если вы задумались о множественном наследовании от классов
с данными — скорее всего, нужна композиция.

---

## Ключевые идеи лекции

1. **`public`-наследование = is-a.** Наследник обязан проходить тест Лисков.
2. **Модификатор наследования** меняет доступ к членам базы; `private`-члены
   недоступны наследнику напрямую.
3. **Порядок:** база → члены → тело; разрушение — наоборот.
4. **`using Base::Base`** наследует конструкторы, но не инициализирует поля
   наследника; копирование так не наследуется.
5. **Name hiding:** объявление имени в наследнике скрывает все перегрузки
   базы; лечится `using`.
6. **Срезка:** копирование в базовый тип теряет производную часть.
7. **Композиция предпочтительнее** приватного наследования; приватное
   наследование — только для `protected`, виртуальных функций и EBO.
8. **`final`** запрещает расширение иерархии и помогает оптимизациям.
9. **MI:** порядок баз слева направо; неоднозначные имена квалифицируются;
   указатели на разные базы имеют разные адреса.
10. **Ромб** порождает две копии общего предка.
11. **`virtual`-база** существует в одном экземпляре; её конструктор
    вызывает most-derived класс; цена — `vbptr` и косвенный доступ.
12. **Примеси и интерфейсы** — безопасные формы MI; с данными — композиция.

---

## Типичные ошибки

1. **Забыли вызвать конструктор базы** — вызывается конструктор по
   умолчанию, если он есть; иначе ошибка компиляции.
2. **Срезка в параметрах и контейнерах** — передача полиморфного объекта
   по значению.
3. **Name hiding** — добавили метод с тем же именем и потеряли перегрузки
   базы.
4. **Публичное наследование при отношении has-a** — нарушение семантики.
5. **Ромб без `virtual`** — две копии базы и рассинхронизация состояния.
6. **Most-derived класс не вызвал конструктор виртуальной базы** — будет
   вызван default-конструктор или возникнет ошибка.
7. **`reinterpret_cast` в иерархии MI** — указатель не корректируется.
8. **Глубокие иерархии** (больше 3 уровней) — сложность растёт быстрее
   пользы.

---

## Вопросы для самопроверки

1. Чем отличаются `public`, `protected` и `private`-наследование?
   Составьте таблицу доступа.
2. В каком порядке вызываются конструкторы и деструкторы при наследовании?
3. Что наследуется, а что нет при `using Base::Base`?
4. Что такое name hiding и как его снять?
5. Что произойдёт при `Base b = derived;`? Как избежать потери данных?
6. Когда приватное наследование оправдано, а когда нужна композиция?
7. Зачем нужен `final` и на что он влияет?
8. Почему `Circle : Ellipse` считается нарушением LSP?
9. Как разрешается неоднозначность имён при множественном наследовании?
10. Почему указатели на разные базы одного объекта могут различаться?
11. Что такое проблема ромба и как её решает `virtual`?
12. Кто отвечает за вызов конструктора виртуальной базы и почему?
13. Чем примеси и интерфейсы отличаются от MI от классов с данными?

---

## Практическое задание

Реализуйте иерархию геометрических фигур.

1. Базовый класс `Shape` с координатами центра и методами `area()`,
   `perimeter()`, `info()`.
2. `Rectangle : public Shape` с шириной и высотой.
3. `Square : public Rectangle` — обоснуйте корректность с точки зрения LSP.
4. Продемонстрируйте:
   - порядок конструирования и разрушения (вывод в консоль);
   - срезку объекта и способ её избежать;
   - `using Base::Base` для наследования конструкторов;
   - `final` на одном из классов.

Дополнительно (часть II): реализуйте ромб `Character → Warrior, Mage →
BattleMage`:

1. Сначала без `virtual` — покажите две копии `Character`.
2. Затем с `virtual` — покажите единственный подобъект.
3. Выведите адреса подобъектов, чтобы показать корректировку указателей.
4. Реализуйте альтернативу через композицию и сравните подходы.
5. Не используйте шаблоны.

---

## Связь с практикой

Лабораторная работа 4 «Полиморфизм и наследование (мини-миры)» использует
материал этой лекции: абстрактный базовый класс, полиморфную коллекцию,
виртуальный деструктор и приведение типов. Шаблоны понадобятся позже —
в лекции 15 и лабораторной работе 5.
