# Лекция 5. Классы: поля, методы, инкапсуляция

## План лекции

1. `struct` vs `class` — единственное отличие
2. Поля и методы: определение внутри и вне класса
3. Модификаторы доступа: `public`, `protected`, `private`
4. Указатель `this` и fluent interface
5. `const`-методы и `mutable`
6. Статические поля и методы (`inline static` в C++17)
7. Дружественные функции и классы (`friend`)
8. Вложенные классы и `enum class`
9. `sizeof`, выравнивание и padding
10. Принципы инкапсуляции и обзор идиомы PIMPL

---

## 1. `struct` vs `class`

В C++ `struct` и `class` — это одно и то же, за исключением **модификатора доступа по умолчанию**:

- `struct` — члены по умолчанию `public`
- `class` — члены по умолчанию `private`

```cpp
struct Point {
    double x; // public по умолчанию
    double y;
};

class PointClass {
    double x; // private по умолчанию
    double y;
public:
    double getX() const { return x; }
    double getY() const { return y; }
};
```

> **Конвенция:** `struct` используют для простых агрегатов данных (POD), `class` — когда есть инварианты и поведение.

Примеры на GitHub:
- [31_Struct](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/31_Struct)

---

## 2. Поля и методы: определение внутри и вне класса

Методы можно определять прямо в теле класса (неявно `inline`) или выносить реализацию в `.cpp`-файл.

```cpp
// point.h
class Point {
    double x_, y_;
public:
    // Определение внутри класса (inline)
    double getX() const { return x_; }
    double getY() const { return y_; }

    // Объявление — реализация в .cpp
    double distanceTo(const Point& other) const;
    void print() const;
};

// point.cpp
#include "point.h"
#include <cmath>
#include <iostream>

double Point::distanceTo(const Point& other) const {
    double dx = x_ - other.x_;
    double dy = y_ - other.y_;
    return std::sqrt(dx * dx + dy * dy);
}

void Point::print() const {
    std::cout << "(" << x_ << ", " << y_ << ")" << std::endl;
}
```

Примеры на GitHub:
- [22_YourFirstClass](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/22_YourFirstClass)
- [26_ClassAcrossMultipleFiles](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/26_ClassAcrossMultipleFiles)

---

## 3. Модификаторы доступа

| Модификатор | Доступ из класса | Доступ из наследника | Доступ извне |
|-------------|:---:|:---:|:---:|
| `public`    | да  | да  | да  |
| `protected` | да  | да  | нет |
| `private`   | да  | нет | нет |

```cpp
class Account {
public:
    std::string getOwner() const { return owner_; }
    double getBalance() const { return balance_; }
    void deposit(double amount) {
        if (amount > 0) balance_ += amount;
    }

protected:
    void setOwner(const std::string& name) { owner_ = name; }

private:
    std::string owner_;
    double balance_ = 0.0;
};
```

Примеры на GitHub:
- [25_SettersAndGetters](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/25_SettersAndGetters)

---

## 4. Указатель `this` и Fluent Interface

`this` — неявный указатель на текущий объект. Тип: `T* const` (для `const`-метода — `const T* const`).

```cpp
class QueryBuilder {
    std::string table_;
    std::string condition_;
    int limit_ = -1;

public:
    QueryBuilder& from(const std::string& table) {
        this->table_ = table;
        return *this; // возвращаем ссылку на себя
    }

    QueryBuilder& where(const std::string& cond) {
        condition_ = cond;
        return *this;
    }

    QueryBuilder& limit(int n) {
        limit_ = n;
        return *this;
    }

    std::string build() const {
        std::string sql = "SELECT * FROM " + table_;
        if (!condition_.empty()) sql += " WHERE " + condition_;
        if (limit_ > 0) sql += " LIMIT " + std::to_string(limit_);
        return sql;
    }
};

// Использование — цепочка вызовов (fluent interface):
// auto q = QueryBuilder().from("users").where("age > 18").limit(10).build();
```

Примеры на GitHub:
- [30_ThisPointer](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/30_ThisPointer)

---

## 5. `const`-методы и `mutable`

Метод, помеченный `const`, обещает не менять состояние объекта. Внутри него `this` имеет тип `const T*`.

Ключевое слово `mutable` позволяет изменять поле даже из `const`-метода — используется для кэшей, счётчиков, мьютексов.

```cpp
class Matrix {
    std::vector<std::vector<double>> data_;
    mutable double cached_det_ = 0.0;
    mutable bool det_valid_ = false;

public:
    double determinant() const {
        if (!det_valid_) {
            cached_det_ = computeDet(); // OK — mutable
            det_valid_ = true;
        }
        return cached_det_;
    }

    void set(int i, int j, double val) {
        data_[i][j] = val;
        det_valid_ = false; // инвалидируем кэш
    }

private:
    double computeDet() const { /* ... */ return 0.0; }
};
```

Примеры на GitHub:
- [33_ConstMember](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/33_ConstMember)

---

## 6. Статические поля и методы

Статические члены принадлежат классу, а не объекту.

```cpp
class Logger {
    // C++17: inline static — определение прямо в заголовке
    inline static int instance_count_ = 0;
    inline static std::string prefix_ = "[LOG]";

    std::string name_;

public:
    Logger(const std::string& name) : name_(name) {
        ++instance_count_;
    }

    ~Logger() { --instance_count_; }

    // Статический метод — нет this, нет доступа к нестатическим полям
    static int getInstanceCount() { return instance_count_; }

    static void setPrefix(const std::string& p) { prefix_ = p; }

    void log(const std::string& msg) const {
        std::cout << prefix_ << " [" << name_ << "] " << msg << "\n";
    }
};
```

До C++17 `static` поля (не `inline`) нужно было определять в `.cpp`:
```cpp
// logger.cpp
int Logger::instance_count_ = 0;
```

---

## 7. Дружественные функции и классы

`friend` даёт внешней функции или классу доступ к `private`/`protected` членам.

```cpp
class Vector2D {
    double x_, y_;

public:
    Vector2D(double x, double y) : x_(x), y_(y) {}

    // Дружественная функция
    friend double dot(const Vector2D& a, const Vector2D& b);

    // Дружественный оператор вывода
    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
        return os << "(" << v.x_ << ", " << v.y_ << ")";
    }

    // Дружественный класс
    friend class VectorSerializer;
};

double dot(const Vector2D& a, const Vector2D& b) {
    return a.x_ * b.x_ + a.y_ * b.y_; // доступ к private
}

class VectorSerializer {
public:
    static std::string toJson(const Vector2D& v) {
        return "{\"x\":" + std::to_string(v.x_) +
               ",\"y\":" + std::to_string(v.y_) + "}";
    }
};
```

> **Важно:** `friend` не транзитивен и не наследуется. Используйте экономно.

---

## 8. Вложенные классы и `enum class`

```cpp
class LinkedList {
public:
    enum class Error { Empty, OutOfRange, InvalidArg };

private:
    struct Node { // вложенный класс
        int data;
        Node* next;
        Node(int d, Node* n = nullptr) : data(d), next(n) {}
    };

    Node* head_ = nullptr;
    size_t size_ = 0;

public:
    void pushFront(int val) {
        head_ = new Node(val, head_);
        ++size_;
    }

    int front() const {
        if (!head_) throw Error::Empty;
        return head_->data;
    }

    size_t size() const { return size_; }
};
```

`enum class` (scoped enum) не допускает неявного приведения к `int` и не засоряет пространство имён.

---

## 9. `sizeof`, выравнивание и padding

Компилятор выравнивает поля для эффективного доступа к памяти. Порядок полей влияет на размер объекта.

```cpp
struct Bad {
    char a;    // 1 байт + 7 padding
    double b;  // 8 байт
    char c;    // 1 байт + 7 padding
};
// sizeof(Bad) == 24

struct Good {
    double b;  // 8 байт
    char a;    // 1 байт
    char c;    // 1 байт + 6 padding
};
// sizeof(Good) == 16

struct Empty {};
// sizeof(Empty) == 1  (гарантия уникальности адреса)
```

Можно управлять выравниванием:
```cpp
struct alignas(16) AlignedData {
    float values[4];
};

#pragma pack(push, 1)
struct Packed {
    char a;
    double b;
    char c;
};
// sizeof(Packed) == 10
#pragma pack(pop)
```

Примеры на GitHub:
- [32_SizeOfClassObjects](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/32_SizeOfClassObjects)

---

## 10. Принципы инкапсуляции и обзор PIMPL

**Инкапсуляция** — сокрытие внутренней реализации за стабильным интерфейсом.

Преимущества:
- Контроль инвариантов (валидация в setter)
- Свобода менять реализацию без изменения интерфейса
- Уменьшение связности между модулями

**PIMPL (Pointer to IMPLementation)** — идиома, выносящая реализацию в отдельный класс:

```cpp
// widget.h
#include <memory>

class Widget {
public:
    Widget();
    ~Widget();
    Widget(Widget&&) noexcept;
    Widget& operator=(Widget&&) noexcept;

    void doWork();
    int getResult() const;

private:
    struct Impl; // forward declaration
    std::unique_ptr<Impl> pImpl_;
};

// widget.cpp
#include "widget.h"

struct Widget::Impl {
    int counter = 0;
    std::string internal_data;
    // ... сложная реализация
    void compute() { ++counter; }
};

Widget::Widget() : pImpl_(std::make_unique<Impl>()) {}
Widget::~Widget() = default; // должен быть в .cpp, где Impl полон
Widget::Widget(Widget&&) noexcept = default;
Widget& Widget::operator=(Widget&&) noexcept = default;

void Widget::doWork() { pImpl_->compute(); }
int Widget::getResult() const { return pImpl_->counter; }
```

Преимущества PIMPL:
- Ускорение компиляции (изменение Impl не требует перекомпиляции клиентов)
- Стабильный ABI
- Полное сокрытие деталей из заголовочного файла

Примеры на GitHub:
- [03_Pimpl](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection10_11/03_Pimpl)
- [04_Pimpl](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection10_11/04_Pimpl)
- [05_Pimpl](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection10_11/05_Pimpl)

---

## Ключевые концепции

| Концепция | Суть |
|-----------|------|
| `struct` vs `class` | Отличие только в доступе по умолчанию |
| `this` | Неявный указатель на текущий объект |
| `const`-метод | Не модифицирует наблюдаемое состояние |
| `mutable` | Исключение из `const` для кэшей/счётчиков |
| `inline static` | Определение статического поля в заголовке (C++17) |
| `friend` | Выборочное предоставление доступа к `private` |
| Padding | Компилятор вставляет пустые байты для выравнивания |
| PIMPL | Сокрытие реализации через указатель на `Impl` |

---

## Типичные ошибки

1. **Забыть `const` у метода, не меняющего объект** — не сможете вызвать через `const`-ссылку.
2. **Сделать все поля `public`** — нарушение инкапсуляции, невозможно контролировать инварианты.
3. **Порядок полей без учёта padding** — лишний расход памяти при большом количестве объектов.
4. **Злоупотребление `friend`** — размывает границу интерфейса.
5. **Определение `static` поля в заголовке без `inline` (до C++17)** — ошибка множественного определения (ODR violation).
6. **PIMPL: деструктор в заголовке** — `unique_ptr<Impl>` требует полного определения `Impl` при уничтожении.

---

## Вопросы для самопроверки

1. Чем `struct` отличается от `class` в C++? Можно ли в `struct` иметь `private`-секцию?
2. Какой тип имеет `this` в обычном методе и в `const`-методе?
3. Для чего нужен `mutable`? Приведите пример, когда его использование оправдано.
4. Почему `sizeof(Empty) == 1` для пустого класса?
5. Что будет, если расположить поля `char, double, char` в структуре? Как оптимизировать?
6. Чем `inline static` (C++17) удобнее классического определения в `.cpp`?
7. Наследуется ли `friend`?
8. Какие преимущества даёт PIMPL? Какие накладные расходы?

---

## Практическое задание

Реализуйте класс `BankAccount`:

1. Приватные поля: `id_` (статический счётчик), `owner_`, `balance_`, `mutable log_count_` (счётчик обращений к `getBalance`).
2. Методы: `deposit`, `withdraw` (с проверкой), `getBalance` (`const`, увеличивает `log_count_`).
3. Fluent interface для `setOwner`, `deposit`, `withdraw` (цепочка вызовов).
4. Статический метод `getTotalAccounts()`.
5. Дружественная функция `transfer(BankAccount& from, BankAccount& to, double amount)`.
6. Вложенный `enum class Error { InsufficientFunds, NegativeAmount }`.
7. Разделите объявление (`.h`) и реализацию (`.cpp`).
8. Проверьте `sizeof` вашего класса и попробуйте оптимизировать расположение полей.
