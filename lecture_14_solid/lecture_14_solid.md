# Лекция 14. Принципы SOLID

## План лекции

1. Связанность (Coupling) и связность (Cohesion)
2. Идиома Pimpl — брандмауэр компиляции
3. Принцип Tell, Don't Ask (TDA)
4. Паттерн CQRS (Command-Query Responsibility Segregation)
5. SRP — принцип единственной ответственности (прогрессивный пример в 4 этапа)
6. OCP — принцип открытости/закрытости
7. LSP — принцип подстановки Лисков (контракты и инварианты)
8. ISP — принцип разделения интерфейсов
9. DIP — принцип инверсии зависимостей
10. Итоги и связь принципов между собой

---

## 1. Связанность (Coupling) и связность (Cohesion)

**Coupling** — степень зависимости одного модуля от другого. Чем выше связанность, тем сложнее менять код: изменение в одном месте каскадно ломает другие.

**Cohesion** — степень, в которой элементы внутри модуля относятся к одной задаче. Высокая связность — хорошо: класс делает одно дело и делает его хорошо.

Цель проектирования: **low coupling, high cohesion**.

### Пример высокой связанности (плохо)

```cpp
// Класс Report знает о деталях базы данных и форматирования
class Report {
    Database db_;           // прямая зависимость от конкретной БД
    PdfFormatter fmt_;      // прямая зависимость от конкретного формата
public:
    void generate() {
        auto data = db_.query("SELECT * FROM sales");
        fmt_.render(data);
    }
};
```

Здесь `Report` связан и с `Database`, и с `PdfFormatter`. Любое изменение в этих классах затронет `Report`.

### Пример слабой связанности (хорошо)

```cpp
class Report {
    IDataSource& source_;
    IFormatter& formatter_;
public:
    Report(IDataSource& src, IFormatter& fmt)
        : source_(src), formatter_(fmt) {}

    void generate() {
        auto data = source_.fetch();
        formatter_.render(data);
    }
};
```

Теперь `Report` зависит только от абстракций. Можно подставить любую базу данных или формат.

> **Пример:** [01_coupling_cohesion](01_coupling_cohesion)

---

## 2. Идиома Pimpl (Pointer to Implementation)

Pimpl — это техника сокрытия деталей реализации за непрозрачным указателем. Она решает две задачи:

1. **Брандмауэр компиляции** — изменение приватных полей не требует перекомпиляции зависимых модулей.
2. **Сокрытие зависимостей** — заголовочный файл не включает «тяжёлые» заголовки.

### Заголовочный файл (widget.h)

```cpp
#pragma once
#include <memory>
#include <string>

class Widget {
public:
    Widget();
    ~Widget();

    // Перемещение разрешено, копирование запрещено
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    void set_name(const std::string& name);
    std::string get_name() const;

private:
    struct Impl;                    // forward declaration
    std::unique_ptr<Impl> pimpl_;   // непрозрачный указатель
};
```

### Файл реализации (widget.cpp)

```cpp
#include "widget.h"
#include <vector>       // тяжёлый заголовок — только здесь
#include <algorithm>

struct Widget::Impl {
    std::string name;
    std::vector<int> internal_data;  // скрыто от пользователей
};

Widget::Widget() : pimpl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default;
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::set_name(const std::string& name) {
    pimpl_->name = name;
}

std::string Widget::get_name() const {
    return pimpl_->name;
}
```

**Важно:** деструктор нельзя оставлять `= default` в заголовке — компилятор не знает, как удалить `Impl`.

> **Пример:** [02_pimpl](02_pimpl)

---

## 3. Tell, Don't Ask (TDA)

Принцип: **не спрашивай объект о его состоянии, чтобы принять решение — скажи ему, что делать**.

### Нарушение TDA (плохо)

```cpp
void process_order(Order& order) {
    if (order.get_status() == Status::Paid) {        // Ask
        if (order.get_total() > 0) {                 // Ask
            order.set_status(Status::Shipped);        // Tell (но поздно)
            warehouse.ship(order.get_items());
        }
    }
}
```

Вся логика вытекла наружу. При изменении бизнес-правил придётся менять каждое место вызова.

### Соблюдение TDA (хорошо)

```cpp
void process_order(Order& order) {
    order.ship(warehouse);   // Tell — объект сам проверяет свои инварианты
}

// Внутри Order:
void Order::ship(Warehouse& wh) {
    if (status_ != Status::Paid || total_ <= 0)
        throw std::logic_error("Cannot ship unpaid order");
    status_ = Status::Shipped;
    wh.ship(items_);
}
```

> **Пример:** [03_tell_dont_ask](03_tell_dont_ask)

---

## 4. CQRS — Command-Query Responsibility Segregation

Принцип Бертрана Мейера: **каждый метод — либо команда (изменяет состояние, ничего не возвращает), либо запрос (возвращает данные, ничего не меняет)**. Не смешивайте.

### Нарушение CQRS

```cpp
class Stack {
public:
    // pop одновременно удаляет и возвращает — команда + запрос
    int pop() {
        int val = data_.back();
        data_.pop_back();
        return val;
    }
};
```

Проблема: если после `data_.pop_back()` произойдёт исключение при копировании, элемент потерян.

### Соблюдение CQRS

```cpp
class Stack {
public:
    int top() const { return data_.back(); }   // запрос — без побочных эффектов
    void pop() { data_.pop_back(); }            // команда — не возвращает значение
};
```

> **Пример:** [04_cqrs](04_cqrs)

---

## 5. SRP — принцип единственной ответственности

> «У класса должна быть только одна причина для изменения» — Роберт Мартин

### Этап 1: Антипаттерн — God Object

```cpp
class Employee {
    std::string name_;
    double salary_;
public:
    // Бизнес-логика
    double calculate_pay() const { return salary_ * 1.13; }

    // Персистентность
    void save_to_db(Database& db) {
        db.execute("INSERT INTO employees ...");
    }

    // Отчётность
    std::string generate_report() const {
        return "Employee: " + name_ + ", Pay: " + std::to_string(calculate_pay());
    }

    // UI
    void display(Screen& screen) {
        screen.draw(generate_report());
    }
};
```

Четыре причины для изменения — бизнес-логика, БД, отчёты, UI.

### Этап 2: Выделяем сохранение

```cpp
class Employee {
    std::string name_;
    double salary_;
public:
    double calculate_pay() const { return salary_ * 1.13; }
    const std::string& name() const { return name_; }
    double salary() const { return salary_; }
    std::string generate_report() const { /* ... */ }
    void display(Screen& screen) { /* ... */ }
};

class EmployeeRepository {
public:
    void save(const Employee& emp, Database& db) {
        db.execute("INSERT INTO employees ...");
    }
};
```

### Этап 3: Выделяем отчёты

```cpp
class EmployeeReportGenerator {
public:
    std::string generate(const Employee& emp) const {
        return "Employee: " + emp.name() +
               ", Pay: " + std::to_string(emp.calculate_pay());
    }
};
```

### Этап 4: Финальная декомпозиция

```cpp
// Чистая доменная модель
class Employee {
    std::string name_;
    double salary_;
public:
    Employee(std::string name, double salary)
        : name_(std::move(name)), salary_(salary) {}

    double calculate_pay() const { return salary_ * 1.13; }
    const std::string& name() const { return name_; }
    double salary() const { return salary_; }
};

// Инфраструктура: хранение
class EmployeeRepository { /* save / load */ };

// Представление: отчёты
class EmployeeReportGenerator { /* generate */ };

// Представление: экран
class EmployeeView {
public:
    void display(Screen& screen, const Employee& emp) { /* ... */ }
};
```

Теперь каждый класс имеет ровно одну причину для изменения.

> **Пример:** [05_srp](05_srp)

---

## 6. OCP — принцип открытости/закрытости

> «Программные сущности должны быть открыты для расширения, но закрыты для модификации» — Бертран Мейер

### Нарушение OCP

```cpp
class AreaCalculator {
public:
    double calculate(const Shape& shape) {
        if (shape.type == "circle")
            return 3.14159 * shape.radius * shape.radius;
        else if (shape.type == "rectangle")
            return shape.width * shape.height;
        // Добавление нового типа требует изменения этого кода!
    }
};
```

### Соблюдение OCP через полиморфизм

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Circle : public Shape {
    double radius_;
public:
    explicit Circle(double r) : radius_(r) {}
    double area() const override { return 3.14159 * radius_ * radius_; }
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
};

// Этот код НИКОГДА не меняется при добавлении новых фигур
double total_area(const std::vector<std::unique_ptr<Shape>>& shapes) {
    double sum = 0;
    for (const auto& s : shapes)
        sum += s->area();
    return sum;
}
```

Добавление `Triangle` не требует изменения `total_area` — только расширение иерархии.

> **Пример:** [06_ocp](06_ocp)

---

## 7. LSP — принцип подстановки Лисков

> «Если S является подтипом T, то объекты типа T могут быть заменены объектами типа S без нарушения корректности программы»

### Классический контрпример: Rectangle / Square

```cpp
class Rectangle {
protected:
    double width_, height_;
public:
    virtual void set_width(double w)  { width_ = w; }
    virtual void set_height(double h) { height_ = h; }
    double area() const { return width_ * height_; }
};

class Square : public Rectangle {
public:
    void set_width(double w) override {
        width_ = w;
        height_ = w;   // нарушение: клиент не ожидает, что set_width меняет height
    }
    void set_height(double h) override {
        width_ = h;
        height_ = h;
    }
};
```

Клиентский код, написанный для `Rectangle`, сломается:

```cpp
void test(Rectangle& r) {
    r.set_width(5);
    r.set_height(4);
    assert(r.area() == 20);  // FAIL для Square: area == 16
}
```

### Решение: неизменяемые объекты или разделение иерархии

```cpp
class Shape {
public:
    virtual ~Shape() = default;
    virtual double area() const = 0;
};

class Rectangle : public Shape {
    double w_, h_;
public:
    Rectangle(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
};

class Square : public Shape {
    double side_;
public:
    explicit Square(double s) : side_(s) {}
    double area() const override { return side_ * side_; }
};
```

**Контракты LSP:**
- **Предусловия** подтипа не могут быть сильнее, чем у базового типа.
- **Постусловия** подтипа не могут быть слабее.
- **Инварианты** базового типа должны сохраняться.

> **Пример:** [07_lsp](07_lsp)

---

## 8. ISP — принцип разделения интерфейсов

> «Клиенты не должны зависеть от интерфейсов, которые они не используют»

### Нарушение ISP — «толстый» интерфейс

```cpp
class IMachine {
public:
    virtual ~IMachine() = default;
    virtual void print(const Document& doc) = 0;
    virtual void scan(const Document& doc) = 0;
    virtual void fax(const Document& doc) = 0;
};

// Простой принтер вынужден реализовывать scan и fax
class SimplePrinter : public IMachine {
public:
    void print(const Document& doc) override { /* OK */ }
    void scan(const Document& doc) override {
        throw std::runtime_error("Not supported");  // плохо!
    }
    void fax(const Document& doc) override {
        throw std::runtime_error("Not supported");  // плохо!
    }
};
```

### Соблюдение ISP — разделение интерфейсов

```cpp
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

// Простой принтер реализует только то, что умеет
class SimplePrinter : public IPrinter {
public:
    void print(const Document& doc) override { /* ... */ }
};

// МФУ реализует всё
class MultiFunctionDevice : public IPrinter, public IScanner, public IFax {
public:
    void print(const Document& doc) override { /* ... */ }
    void scan(const Document& doc) override { /* ... */ }
    void fax(const Document& doc) override { /* ... */ }
};
```

> **Пример:** [08_isp](08_isp)

---

## 9. DIP — принцип инверсии зависимостей

> «Модули верхнего уровня не должны зависеть от модулей нижнего уровня. Оба должны зависеть от абстракций»

### Нарушение DIP

```cpp
class MySqlDatabase {
public:
    void save(const std::string& data) { /* SQL-запрос */ }
};

class UserService {
    MySqlDatabase db_;   // жёсткая зависимость от конкретной реализации
public:
    void create_user(const std::string& name) {
        db_.save(name);
    }
};
```

### Соблюдение DIP

```cpp
// Абстракция — определяется на уровне бизнес-логики
class IUserRepository {
public:
    virtual ~IUserRepository() = default;
    virtual void save(const std::string& data) = 0;
};

// Реализация — зависит от абстракции
class MySqlUserRepository : public IUserRepository {
public:
    void save(const std::string& data) override { /* SQL */ }
};

class MongoUserRepository : public IUserRepository {
public:
    void save(const std::string& data) override { /* MongoDB */ }
};

// Бизнес-логика — зависит от абстракции
class UserService {
    std::unique_ptr<IUserRepository> repo_;
public:
    explicit UserService(std::unique_ptr<IUserRepository> repo)
        : repo_(std::move(repo)) {}

    void create_user(const std::string& name) {
        repo_->save(name);
    }
};

// Сборка в main (Composition Root)
int main() {
    auto repo = std::make_unique<MySqlUserRepository>();
    UserService service(std::move(repo));
    service.create_user("Alice");
}
```

**Ключевая идея:** абстракция `IUserRepository` принадлежит слою бизнес-логики, а не слою инфраструктуры. Направление зависимости инвертировано.

> **Пример:** [09_dip](09_dip)

---

## 10. Связь принципов между собой

| Принцип | Что решает | Связь |
|---------|-----------|-------|
| SRP | Одна причина для изменения | Основа для ISP и DIP |
| OCP | Расширение без модификации | Реализуется через полиморфизм (LSP) |
| LSP | Корректность подстановки | Гарантирует работу OCP |
| ISP | Минимальные интерфейсы | Следствие SRP для интерфейсов |
| DIP | Зависимость от абстракций | Использует ISP для определения интерфейсов |

SOLID — не догма, а набор эвристик. Применяйте с умом: слишком формальное следование может привести к избыточной абстракции.

---

## Ключевые концепции

- **Coupling/Cohesion** — стремитесь к слабой связанности и высокой связности.
- **Pimpl** — скрывает реализацию за указателем, ускоряет компиляцию.
- **TDA** — объект сам управляет своим состоянием.
- **CQRS** — методы-команды и методы-запросы не смешиваются.
- **SRP** — один класс = одна ответственность = одна причина для изменения.
- **OCP** — новое поведение добавляется без изменения существующего кода.
- **LSP** — подтип должен быть полностью совместим с базовым типом.
- **ISP** — много маленьких интерфейсов лучше одного большого.
- **DIP** — зависьте от абстракций, а не от конкретных реализаций.

---

## Типичные ошибки

1. **Pimpl без определения деструктора в .cpp** — `unique_ptr` не может удалить неполный тип.
2. **Square наследует Rectangle с мутабельными полями** — нарушение LSP.
3. **God Object** — класс с 10+ обязанностями, который «всё знает».
4. **Пустые реализации в «толстом» интерфейсе** — верный признак нарушения ISP.
5. **`new ConcreteClass` внутри бизнес-логики** — нарушение DIP, невозможность тестирования.

---

## Вопросы для самопроверки

1. Чем отличается coupling от cohesion? Какое сочетание желательно?
2. Почему деструктор Pimpl-класса нельзя оставить `= default` в заголовке?
3. Приведите пример нарушения TDA из реального кода.
4. Как CQRS связан с безопасностью исключений?
5. Объясните, почему `Square : public Rectangle` нарушает LSP.
6. Как OCP реализуется через виртуальные функции?
7. В чём разница между DIP и обычным dependency injection?

---

## Практическое задание

Спроектируйте систему уведомлений (Notification System):

1. Создайте интерфейс `INotificationSender` с методом `send(message)`.
2. Реализуйте `EmailSender`, `SmsSender`, `TelegramSender`.
3. Создайте `NotificationService`, который принимает вектор `INotificationSender` (DIP).
4. Добавьте `IMessageFormatter` с реализациями `PlainText` и `HtmlFormatter` (ISP/SRP).
5. Покажите, что добавление нового канала (например, `SlackSender`) не требует изменения `NotificationService` (OCP).
6. Используйте Pimpl для сокрытия деталей реализации `EmailSender`.

Убедитесь, что каждый принцип SOLID соблюдён, и прокомментируйте, где именно.
