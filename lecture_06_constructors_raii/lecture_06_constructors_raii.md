# Лекция 6. Конструкторы, деструкторы, RAII

## План лекции

1. Виды конструкторов: по умолчанию, параметризованный, копирующий, конвертирующий
2. Member Initializer List (MIL) и порядок инициализации
3. Делегирующие конструкторы (C++11)
4. Ключевое слово `explicit`
5. `=default`, `=delete` и таблица автогенерации
6. Деструкторы и порядок уничтожения
7. Идиома RAII
8. Время жизни объектов: автоматическое, динамическое, статическое, временные объекты
9. Паттерн Singleton
10. Резюме и связь с правилом пяти

---

## 1. Виды конструкторов

### Конструктор по умолчанию

Вызывается без аргументов. Компилятор генерирует его автоматически, если не определён ни один конструктор.

```cpp
class Point {
public:
    Point() : x_(0.0), y_(0.0) {} // конструктор по умолчанию
    double x_, y_;
};

Point p;      // вызов конструктора по умолчанию
Point p2{};   // то же самое (uniform initialization)
```

### Параметризованный конструктор

```cpp
class Point {
public:
    Point(double x, double y) : x_(x), y_(y) {}

    // С параметрами по умолчанию — может служить и конструктором по умолчанию
    // Point(double x = 0.0, double y = 0.0) : x_(x), y_(y) {}

private:
    double x_, y_;
};

Point p(3.0, 4.0);
Point p2{1.0, 2.0}; // uniform initialization
```

### Копирующий конструктор

Создаёт объект как копию другого. Сигнатура: `T(const T&)`.

```cpp
class String {
    char* data_;
    size_t size_;

public:
    String(const char* s) {
        size_ = std::strlen(s);
        data_ = new char[size_ + 1];
        std::strcpy(data_, s);
    }

    // Копирующий конструктор — глубокая копия
    String(const String& other) : size_(other.size_) {
        data_ = new char[size_ + 1];
        std::strcpy(data_, other.data_);
    }

    ~String() { delete[] data_; }
};

String s1("Hello");
String s2 = s1;  // вызов копирующего конструктора
String s3(s1);   // то же самое
```

### Конвертирующий конструктор

Конструктор с одним параметром (или несколькими, если остальные имеют значения по умолчанию) может выполнять неявное преобразование типа.

```cpp
class Fraction {
    int num_, den_;
public:
    Fraction(int n, int d = 1) : num_(n), den_(d) {} // конвертирующий

    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        return os << f.num_ << "/" << f.den_;
    }
};

void print(const Fraction& f) {
    std::cout << f << std::endl;
}

print(42); // неявная конверсия int -> Fraction(42, 1)
```

Примеры на GitHub:
- [23_Constructors](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/23_Constructors)

---

## 2. Member Initializer List и порядок инициализации

MIL — единственный способ инициализировать `const`-поля, ссылки и объекты без конструктора по умолчанию.

```cpp
class Student {
    const int id_;            // const — только через MIL
    std::string& group_ref_;  // ссылка — только через MIL
    std::string name_;
    double gpa_;

public:
    Student(int id, std::string& group, const std::string& name, double gpa)
        : id_(id)             // 1-й по порядку объявления
        , group_ref_(group)   // 2-й
        , name_(name)         // 3-й
        , gpa_(gpa)           // 4-й
    {
        // Тело — для дополнительной логики
    }
};
```

> **Критически важно:** поля инициализируются в порядке **объявления в классе**, а НЕ в порядке MIL. Несовпадение порядка — частый источник UB.

```cpp
class Danger {
    int b_;
    int a_;
public:
    // Ловушка: a_ инициализируется ДО b_, потому что объявлен после b_
    // Нет! b_ объявлен первым, значит b_ инициализируется первым
    Danger(int x) : a_(x), b_(a_) {} // UB! b_ инициализируется первым,
                                      // а a_ ещё не инициализирован
};
```

---

## 3. Делегирующие конструкторы (C++11)

Позволяют одному конструктору вызывать другой, избегая дублирования кода.

```cpp
class Connection {
    std::string host_;
    int port_;
    bool ssl_;

public:
    Connection(const std::string& host, int port, bool ssl)
        : host_(host), port_(port), ssl_(ssl) {
        // общая логика инициализации
        std::cout << "Connecting to " << host_ << ":" << port_ << "\n";
    }

    // Делегирование к полному конструктору
    Connection(const std::string& host, int port)
        : Connection(host, port, false) {} // ssl = false

    Connection(const std::string& host)
        : Connection(host, 80, false) {} // порт 80, без ssl

    Connection()
        : Connection("localhost") {} // все значения по умолчанию
};
```

> При делегировании нельзя одновременно инициализировать поля в MIL — только вызов другого конструктора.

---

## 4. Ключевое слово `explicit`

Запрещает неявные преобразования и copy-initialization.

```cpp
class BigInt {
    std::vector<int> digits_;

public:
    explicit BigInt(int value) {
        while (value > 0) {
            digits_.push_back(value % 10);
            value /= 10;
        }
    }

    explicit BigInt(const std::string& s) {
        for (char c : s) {
            digits_.push_back(c - '0');
        }
    }
};

void process(const BigInt& b) { /* ... */ }

// process(42);           // Ошибка компиляции! explicit запрещает
process(BigInt(42));      // OK — явное создание
// BigInt b = 42;         // Ошибка! copy-initialization запрещена
BigInt b(42);             // OK — direct initialization
BigInt b2{42};            // OK — direct-list-initialization
```

> **Правило:** делайте конструкторы с одним параметром `explicit`, если неявная конверсия семантически неочевидна.

---

## 5. `=default`, `=delete` и таблица автогенерации

```cpp
class Buffer {
public:
    Buffer() = default;                      // просим компилятор сгенерировать
    Buffer(const Buffer&) = delete;          // запрещаем копирование
    Buffer& operator=(const Buffer&) = delete;

    Buffer(Buffer&& other) noexcept = default;
    Buffer& operator=(Buffer&& other) noexcept = default;

    ~Buffer() = default;
};
```

### Таблица автогенерации специальных методов

| Объявлено пользователем | Default ctor | Copy ctor | Copy assign | Move ctor | Move assign | Dtor |
|-------------------------|:---:|:---:|:---:|:---:|:---:|:---:|
| Ничего                  | да  | да  | да  | да  | да  | да  |
| Любой конструктор       | нет | да  | да  | да  | да  | да  |
| Copy ctor               | нет | --  | да  | нет | нет | да  |
| Copy assign             | да  | да  | --  | нет | нет | да  |
| Move ctor               | нет | del | del | --  | нет | да  |
| Move assign             | да  | del | del | нет | --  | да  |
| Destructor              | да  | да* | да* | нет | нет | --  |

\* — генерируется, но deprecated (может быть убрано в будущем стандарте).

Примеры на GitHub:
- [24_DefaultedConstructors](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/24_DefaultedConstructors)

---

## 6. Деструкторы и порядок уничтожения

Деструктор вызывается автоматически при выходе объекта из области видимости, при `delete`, при раскрутке стека.

```cpp
class FileHandle {
    FILE* file_;
    std::string name_;

public:
    FileHandle(const std::string& name, const char* mode)
        : file_(std::fopen(name.c_str(), mode)), name_(name) {
        if (!file_) throw std::runtime_error("Cannot open " + name);
        std::cout << "Opened: " << name_ << "\n";
    }

    ~FileHandle() {
        if (file_) {
            std::fclose(file_);
            std::cout << "Closed: " << name_ << "\n";
        }
    }

    // Запрет копирования
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
};
```

**Порядок уничтожения:**
- Поля уничтожаются в **обратном** порядке их объявления.
- Локальные объекты уничтожаются в обратном порядке создания.

```cpp
class A {
public:
    A() { std::cout << "A ctor\n"; }
    ~A() { std::cout << "A dtor\n"; }
};

class B {
public:
    B() { std::cout << "B ctor\n"; }
    ~B() { std::cout << "B dtor\n"; }
};

class C {
    A a_;   // создаётся первым
    B b_;   // создаётся вторым
public:
    C() { std::cout << "C ctor\n"; }
    ~C() { std::cout << "C dtor\n"; }
};

// Создание C: A ctor -> B ctor -> C ctor
// Уничтожение C: C dtor -> B dtor -> A dtor
```

Примеры на GitHub:
- [28_Destructors](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/28_Destructors)
- [29_OrderOfConstructorDestructorCalls](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/29_OrderOfConstructorDestructorCalls)

---

## 7. Идиома RAII

**RAII (Resource Acquisition Is Initialization)** — ресурс захватывается в конструкторе и освобождается в деструкторе.

```cpp
class MutexGuard {
    std::mutex& mtx_;
public:
    explicit MutexGuard(std::mutex& m) : mtx_(m) {
        mtx_.lock();
    }
    ~MutexGuard() {
        mtx_.unlock();
    }
    MutexGuard(const MutexGuard&) = delete;
    MutexGuard& operator=(const MutexGuard&) = delete;
};

// Использование:
void safeIncrement(int& counter, std::mutex& m) {
    MutexGuard lock(m);     // захват
    ++counter;
    // При выходе — автоматический unlock, даже при исключении
}
```

Стандартные RAII-обёртки:
- `std::unique_ptr` / `std::shared_ptr` — управление памятью
- `std::lock_guard` / `std::unique_lock` — мьютексы
- `std::fstream` — файлы

```cpp
void readFile(const std::string& path) {
    std::ifstream file(path); // RAII: файл открыт
    if (!file.is_open()) throw std::runtime_error("Cannot open");

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }
    // Деструктор ifstream закрывает файл автоматически
}
```

Примеры на GitHub:
- [27_ManagingClassObjectsThroughPointers](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/27_ManagingClassObjectsThroughPointers)

---

## 8. Время жизни объектов

### Автоматическое (стек)

```cpp
void foo() {
    std::string s = "hello"; // создаётся на стеке
    // ...
} // s уничтожается при выходе из функции
```

### Динамическое (куча)

```cpp
void bar() {
    auto* p = new std::vector<int>(100);
    // ...
    delete p; // ручное освобождение (плохо!)

    // Лучше — RAII:
    auto sp = std::make_unique<std::vector<int>>(100);
    // автоматическое удаление при выходе
}
```

### Статическое

```cpp
class Config {
    static Config& instance() {
        static Config cfg; // создаётся при первом вызове, жив до конца программы
        return cfg;
    }
};
```

### Временные объекты

```cpp
std::string getName() { return "Alice"; }

// Временный объект живёт до конца полного выражения (full-expression)
std::cout << getName() << "\n"; // временный string жив на время вычисления <<

// Продление жизни через const&
const std::string& ref = getName(); // временный объект живёт, пока жива ref
```

---

## 9. Паттерн Singleton

Гарантирует существование единственного экземпляра класса.

```cpp
class Database {
public:
    static Database& getInstance() {
        static Database instance; // потокобезопасно с C++11 (Magic Statics)
        return instance;
    }

    void query(const std::string& sql) {
        std::cout << "Executing: " << sql << "\n";
        ++query_count_;
    }

    int getQueryCount() const { return query_count_; }

    // Запрет копирования и перемещения
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    Database(Database&&) = delete;
    Database& operator=(Database&&) = delete;

private:
    Database() { // private конструктор
        std::cout << "Database initialized\n";
    }
    ~Database() {
        std::cout << "Database shut down\n";
    }

    int query_count_ = 0;
};

// Использование:
// Database::getInstance().query("SELECT * FROM users");
```

> **Предостережение:** Singleton часто считается анти-паттерном — он создаёт глобальное состояние, затрудняет тестирование, скрывает зависимости. Используйте осознанно.

---

## 10. Резюме и связь с правилом пяти

Конструкторы и деструкторы — основа управления ресурсами в C++. Если класс управляет ресурсом (память, файл, сокет), необходимо корректно реализовать:

1. Деструктор
2. Копирующий конструктор
3. Копирующий оператор присваивания
4. Перемещающий конструктор
5. Перемещающий оператор присваивания

Это **Правило пяти** — тема следующей лекции.

---

## Ключевые концепции

| Концепция | Суть |
|-----------|------|
| MIL | Инициализация полей до входа в тело конструктора |
| Порядок инициализации | По порядку объявления полей в классе |
| Делегирование | Конструктор вызывает другой конструктор того же класса |
| `explicit` | Запрет неявных преобразований |
| `=default` / `=delete` | Явный контроль генерации специальных методов |
| RAII | Захват ресурса в конструкторе, освобождение в деструкторе |
| Singleton | Единственный экземпляр с доступом через статический метод |

---

## Типичные ошибки

1. **Порядок в MIL не совпадает с объявлением** — компилятор предупреждает (`-Wreorder`), но многие игнорируют.
2. **Забыть `explicit`** — `Vector(int n)` без `explicit` позволяет `Vector v = 5;`.
3. **Утечка ресурсов без RAII** — `new` без `delete`, особенно при исключениях.
4. **Деструктор не виртуальный** в базовом классе — UB при `delete` через указатель на базу (тема лекции о наследовании).
5. **Двойное освобождение** при поверхностном копировании (shallow copy).
6. **Инициализация в теле вместо MIL** — для `const`, ссылок и объектов без default ctor это ошибка компиляции.

---

## Вопросы для самопроверки

1. Когда компилятор перестаёт генерировать конструктор по умолчанию?
2. Почему MIL предпочтительнее присваивания в теле конструктора?
3. В каком порядке инициализируются поля? Почему порядок в MIL не определяет реальный порядок?
4. Что произойдёт, если конструктор с одним параметром не `explicit`, и мы передадим `int` в функцию, ожидающую `MyClass`?
5. Какие специальные методы перестаёт генерировать компилятор при объявлении деструктора?
6. Что гарантирует RAII при возникновении исключения?
7. Почему Singleton считается анти-паттерном? Когда его использование оправдано?
8. Что такое Magic Statics в C++11?

---

## Практическое задание

Реализуйте класс `DynamicArray`:

1. Конструктор по умолчанию (пустой массив).
2. Конструктор `explicit DynamicArray(size_t capacity)`.
3. Конструктор из `std::initializer_list<int>`.
4. Копирующий конструктор (глубокая копия).
5. Деструктор (освобождение `new[]`).
6. Делегирующий конструктор: `DynamicArray(size_t count, int value)` делегирует к `DynamicArray(count)`, затем заполняет.
7. Методы: `push_back`, `size`, `capacity`, `at` (с проверкой границ), `operator[]`.
8. Используйте MIL для всех конструкторов.
9. Убедитесь, что нет утечек памяти (проверьте valgrind/AddressSanitizer).
10. Добавьте `=delete` для перемещающих операций (пока — их реализация в следующей лекции).
