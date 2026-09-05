# Лекция 12. Обработка исключений. Гарантии безопасности

## План лекции

1. Коды ошибок vs исключения
2. Синтаксис: `throw`, `try`, `catch`
3. Иерархия `std::exception`
4. Раскрутка стека (stack unwinding)
5. Гарантии безопасности исключений (none / basic / strong / nothrow)
6. RAII и исключения
7. Copy-and-swap для строгой гарантии
8. `noexcept`: спецификатор и оператор
9. Повторный выброс: `throw;` vs `throw e;` (срезка!)
10. `exception_ptr`, `nested_exception`, исключения в конструкторах/деструкторах

---

## 1. Коды ошибок vs исключения

### Коды ошибок (C-стиль)

```cpp
int readFile(const char* path, std::string& content) {
    std::ifstream f(path);
    if (!f.is_open()) return -1;     // ошибка открытия
    // ...чтение...
    if (f.fail()) return -2;          // ошибка чтения
    return 0;                          // успех
}

int main() {
    std::string content;
    int err = readFile("data.txt", content);
    if (err == -1) { /* ... */ }      // легко забыть проверить
    else if (err == -2) { /* ... */ }
}
```

**Проблемы:** можно проигнорировать ошибку, загрязнение сигнатур, невозможность сообщить об ошибке из конструктора.

> Пример: [01_error_codes_vs_exceptions](01_error_codes_vs_exceptions)

### Исключения (C++ стиль)

```cpp
std::string readFile(const std::string& path) {
    std::ifstream f(path);
    if (!f.is_open())
        throw std::runtime_error("Не удалось открыть файл: " + path);
    std::string content((std::istreambuf_iterator<char>(f)),
                         std::istreambuf_iterator<char>());
    if (f.fail())
        throw std::runtime_error("Ошибка чтения файла: " + path);
    return content;
}
```

---

## 2. Синтаксис: `throw`, `try`, `catch`

```cpp
#include <stdexcept>
#include <iostream>

double divide(double a, double b) {
    if (b == 0.0)
        throw std::invalid_argument("Деление на ноль");
    return a / b;
}

int main() {
    try {
        double result = divide(10.0, 0.0);
        std::cout << result << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка аргумента: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Общая ошибка: " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Неизвестное исключение" << std::endl;
    }
}
```

**Порядок catch-блоков важен:** от наиболее конкретного к наиболее общему. Компилятор выбирает первый подходящий блок.

> Пример: [02_throw_try_catch](02_throw_try_catch)

---

## 3. Иерархия `std::exception`

```
std::exception
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::domain_error
│   ├── std::length_error
│   └── std::out_of_range
├── std::runtime_error
│   ├── std::range_error
│   ├── std::overflow_error
│   └── std::underflow_error
├── std::bad_alloc
├── std::bad_cast
├── std::bad_typeid
└── std::bad_exception
```

Создание собственных исключений:

```cpp
class DatabaseError : public std::runtime_error {
    int error_code;
public:
    DatabaseError(const std::string& msg, int code)
        : std::runtime_error(msg), error_code(code) {}
    int code() const { return error_code; }
};

class ConnectionError : public DatabaseError {
public:
    ConnectionError(const std::string& host)
        : DatabaseError("Не удалось подключиться к " + host, 1001) {}
};

// Использование
try {
    throw ConnectionError("db.example.com");
} catch (const DatabaseError& e) {
    std::cerr << e.what() << " [код: " << e.code() << "]" << std::endl;
}
```

---

## 4. Раскрутка стека (stack unwinding)

При выбросе исключения runtime разрушает все локальные объекты в обратном порядке, двигаясь по стеку вызовов до подходящего `catch`:

```cpp
class Logger {
    std::string name;
public:
    Logger(const std::string& n) : name(n) {
        std::cout << "  Создан: " << name << std::endl;
    }
    ~Logger() {
        std::cout << "  Разрушен: " << name << std::endl;
    }
};

void func3() {
    Logger l3("L3");
    throw std::runtime_error("Ошибка в func3");
}

void func2() {
    Logger l2("L2");
    func3();
}

void func1() {
    Logger l1("L1");
    func2();
}

int main() {
    try {
        func1();
    } catch (const std::exception& e) {
        std::cout << "Поймано: " << e.what() << std::endl;
    }
}
// Вывод:
//   Создан: L1
//   Создан: L2
//   Создан: L3
//   Разрушен: L3    <- stack unwinding
//   Разрушен: L2    <- stack unwinding
//   Разрушен: L1    <- stack unwinding
//   Поймано: Ошибка в func3
```

---

## 5. Гарантии безопасности исключений

| Гарантия | Описание | Пример |
|---|---|---|
| **Nothrow (no-throw)** | Функция никогда не бросает исключений | Деструкторы, `swap`, `std::vector::size()` |
| **Strong (строгая)** | При исключении состояние не меняется (commit-or-rollback) | `std::vector::push_back`, copy-and-swap assign |
| **Basic (базовая)** | При исключении объект в корректном, но неопределённом состоянии | Большинство стандартных операций |
| **None (никакой)** | Утечка ресурсов, повреждение данных | Плохой код |

```cpp
// Nothrow гарантия
void swap(MyClass& a, MyClass& b) noexcept {
    std::swap(a.data, b.data);
}

// Strong гарантия через copy-and-swap
MyClass& MyClass::operator=(const MyClass& other) {
    MyClass tmp(other);  // Копирование может бросить исключение
    swap(*this, tmp);    // noexcept -- если дошли сюда, всё ОК
    return *this;        // tmp разрушается в деструкторе
}

// Basic гарантия
void MyClass::addItems(const std::vector<int>& items) {
    for (int item : items) {
        data.push_back(item);  // Может бросить на полпути
    }
    // При исключении: часть items добавлена, часть нет
    // Но объект корректен
}
```

---

## 6. RAII и исключения

RAII (Resource Acquisition Is Initialization) -- ключевой принцип для безопасного управления ресурсами при наличии исключений:

```cpp
// ПЛОХО: утечка при исключении
void bad() {
    int* p = new int[100];
    riskyOperation();  // Если бросит -- утечка!
    delete[] p;
}

// ХОРОШО: RAII через unique_ptr
void good() {
    auto p = std::make_unique<int[]>(100);
    riskyOperation();  // Если бросит -- p автоматически освободится
}

// RAII для файлов
void processFile(const std::string& path) {
    std::ifstream file(path);  // RAII: файл закроется в деструкторе
    if (!file) throw std::runtime_error("Cannot open: " + path);

    std::string line;
    while (std::getline(file, line)) {
        process(line);  // Даже если бросит -- файл закроется
    }
}

// RAII для lock-ов
void threadSafe() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII
    riskyOperation();  // Если бросит -- мьютекс разблокируется
}
```

---

## 7. Copy-and-swap для строгой гарантии

```cpp
class Buffer {
    size_t size;
    int* data;
public:
    Buffer(size_t n = 0) : size(n), data(n ? new int[n]{} : nullptr) {}

    Buffer(const Buffer& other) : size(other.size),
                                   data(other.size ? new int[other.size] : nullptr) {
        std::copy(other.data, other.data + size, data);
    }

    Buffer(Buffer&& other) noexcept : size(other.size), data(other.data) {
        other.size = 0;
        other.data = nullptr;
    }

    ~Buffer() { delete[] data; }

    // Copy-and-swap idiom -- строгая гарантия
    friend void swap(Buffer& a, Buffer& b) noexcept {
        std::swap(a.size, b.size);
        std::swap(a.data, b.data);
    }

    Buffer& operator=(Buffer other) {  // копия по значению!
        swap(*this, other);
        return *this;
    }
    // Если копирование (в параметре) бросит -- *this не изменён
    // Если дошли до swap -- это noexcept, всё безопасно
};
```

---

## 8. `noexcept`: спецификатор и оператор

### Спецификатор

```cpp
void safe() noexcept {
    // Обещаем не бросать исключений
    // Если всё же бросим -- std::terminate()!
}

void maybe(bool flag) noexcept(true)  { /* ... */ }  // эквивалент noexcept
void maybe2(bool flag) noexcept(false) { /* ... */ } // может бросать

// Условный noexcept
template<typename T>
void process(T& t) noexcept(noexcept(t.doWork())) {
    t.doWork();
}
```

### Оператор

```cpp
struct A {
    void foo() noexcept {}
    void bar() {}
};

static_assert(noexcept(A().foo()) == true);
static_assert(noexcept(A().bar()) == false);
```

**Зачем `noexcept`:**
- Компилятор генерирует более оптимальный код
- `std::vector::push_back` использует move только если move-конструктор `noexcept`
- Деструкторы неявно `noexcept`

> Пример: [08_noexcept](08_noexcept)

---

## 9. Повторный выброс: `throw;` vs `throw e;`

```cpp
class BaseException : public std::exception {
public:
    const char* what() const noexcept override { return "BaseException"; }
};

class DerivedException : public BaseException {
public:
    const char* what() const noexcept override { return "DerivedException"; }
};

void rethrowCorrect() {
    try {
        throw DerivedException();
    } catch (const BaseException& e) {
        std::cout << "Ловим: " << e.what() << std::endl;
        throw;  // ПРАВИЛЬНО: пробрасывает оригинальный DerivedException
    }
}

void rethrowWrong() {
    try {
        throw DerivedException();
    } catch (const BaseException& e) {
        std::cout << "Ловим: " << e.what() << std::endl;
        throw e;  // ОШИБКА: срезка! Бросает копию BaseException
    }
}

int main() {
    try {
        rethrowCorrect();
    } catch (const DerivedException& e) {
        std::cout << "OK: " << e.what() << std::endl;  // "DerivedException"
    }

    try {
        rethrowWrong();
    } catch (const DerivedException& e) {
        std::cout << "Не попадём сюда" << std::endl;
    } catch (const BaseException& e) {
        std::cout << "Срезка: " << e.what() << std::endl;  // "BaseException"
    }
}
```

> Пример: [09_rethrow](09_rethrow)

**Правило:** для повторного выброса всегда используйте `throw;`, а не `throw e;`.

---

## 10. Дополнительные темы

### `std::exception_ptr` -- перенос исключений между потоками

```cpp
#include <exception>
#include <thread>

std::exception_ptr globalException = nullptr;

void threadFunc() {
    try {
        throw std::runtime_error("Ошибка в потоке");
    } catch (...) {
        globalException = std::current_exception();
    }
}

int main() {
    std::thread t(threadFunc);
    t.join();

    if (globalException) {
        try {
            std::rethrow_exception(globalException);
        } catch (const std::exception& e) {
            std::cout << "Из потока: " << e.what() << std::endl;
        }
    }
}
```

### `std::nested_exception` -- вложенные исключения

```cpp
void lowLevel() {
    throw std::runtime_error("Ошибка диска");
}

void midLevel() {
    try {
        lowLevel();
    } catch (...) {
        std::throw_with_nested(
            std::runtime_error("Ошибка базы данных"));
    }
}

void printNested(const std::exception& e, int level = 0) {
    std::cerr << std::string(level * 2, ' ') << e.what() << std::endl;
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& nested) {
        printNested(nested, level + 1);
    }
}

int main() {
    try {
        midLevel();
    } catch (const std::exception& e) {
        printNested(e);
    }
    // Вывод:
    // Ошибка базы данных
    //   Ошибка диска
}
```

### Исключения в конструкторах и деструкторах

```cpp
// Исключения в конструкторах -- НОРМАЛЬНО и даже рекомендуется
class Connection {
    int* resource;
public:
    Connection(const std::string& host) {
        resource = new int(42);
        if (host.empty()) {
            delete resource;  // Освободить то, что успели выделить!
            throw std::invalid_argument("Пустой хост");
        }
    }
    ~Connection() { delete resource; }
};

// Исключения в деструкторах -- ОПАСНО
class Bad {
    ~Bad() {
        // throw std::runtime_error("ой"); // НЕЛЬЗЯ!
        // Если деструктор вызван при раскрутке стека -- std::terminate()
    }
};
```

**Правило:** деструкторы не должны бросать исключений. Они неявно `noexcept`.

### Производительность

```cpp
// Исключения "стоят" только при выбросе (zero-cost exceptions на happy path)
// Но сам выброс дорогой: раскрутка стека, поиск catch, RTTI

// Не используйте исключения для управления потоком выполнения:
// ПЛОХО:
try {
    while (true) {
        auto item = queue.popOrThrow();  // исключение как break
        process(item);
    }
} catch (const QueueEmpty&) {}

// ХОРОШО:
while (!queue.empty()) {
    auto item = queue.pop();
    process(item);
}
```

> Пример: [10_advanced](10_advanced)

---

## Типичные ошибки

1. **`throw e;` вместо `throw;`** -- срезка исключения при повторном выбросе
2. **Исключение в деструкторе** -- потенциальный `std::terminate()`
3. **Ловить по значению** (`catch(std::exception e)`) -- срезка
4. **Забыли RAII** -- утечка ресурсов при раскрутке стека
5. **`catch(...)` без `throw;`** -- "проглатывание" исключения, сокрытие ошибки
6. **Использование исключений для нормального потока** -- плохая производительность

---

## Вопросы для самопроверки

1. Когда предпочтительнее коды ошибок, а когда исключения?
2. Что происходит при раскрутке стека? Какие объекты разрушаются?
3. Перечислите четыре уровня гарантий безопасности исключений. Приведите пример каждого.
4. Как idiom copy-and-swap обеспечивает строгую гарантию?
5. Чем `throw;` отличается от `throw e;`? Покажите проблему на примере.
6. Зачем нужен `noexcept`? Как он влияет на `std::vector`?
7. Почему нельзя бросать исключения из деструктора?
8. Как перенести исключение между потоками?

---

## Практическое задание

Реализуйте класс `Matrix` с гарантиями безопасности:

1. Конструктор, выбрасывающий `std::invalid_argument` при некорректных размерах
2. `operator=` с использованием copy-and-swap (строгая гарантия)
3. `at(i, j)` с проверкой границ и выбросом `std::out_of_range`
4. Собственная иерархия исключений: `MatrixError -> DimensionMismatch, SingularMatrix`
5. Функция `multiply(const Matrix&, const Matrix&)`, бросающая `DimensionMismatch`
6. `noexcept`-функции: `rows()`, `cols()`, `swap()`, деструктор
7. Продемонстрируйте все четыре уровня гарантий на примерах
