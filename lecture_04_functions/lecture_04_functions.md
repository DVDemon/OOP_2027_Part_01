# Лекция 4. Функции: перегрузка, параметры, inline, RVO

## План лекции

1. Объявление vs определение функции. Прототипы
2. Передача параметров: по значению, по ссылке, по указателю, по `const&`
3. Перегрузка функций
4. Алгоритм разрешения перегрузки
5. Аргументы по умолчанию
6. `inline`-функции и `constexpr`-функции
7. Возвращаемые значения. RVO и NRVO
8. Указатели на функции. `std::function`
9. Рекурсия: прямая, хвостовая, взаимная
10. ADL (Argument-Dependent Lookup)

---

## 1. Объявление vs определение функции

### Объявление (declaration)

Объявление сообщает компилятору о существовании функции — её имя, типы параметров и тип возврата. Объявление не содержит тела:

```cpp
// Объявление (прототип) — обычно в заголовочном файле .h
int add(int a, int b);
double calculate_area(double radius);
void print_message(const std::string& msg);
```

### Определение (definition)

Определение включает тело функции:

```cpp
// Определение — обычно в .cpp файле
int add(int a, int b) {
    return a + b;
}
```

### Связь с ODR

- Объявлений может быть сколько угодно (должны совпадать).
- Определение — ровно одно на всю программу (для обычных функций).
- `inline`-функции и шаблоны — исключение: могут определяться в нескольких единицах трансляции (при идентичности).

```cpp
// math_utils.h
#pragma once
int add(int a, int b);           // объявление

// math_utils.cpp
#include "math_utils.h"
int add(int a, int b) {          // определение
    return a + b;
}

// main.cpp
#include "math_utils.h"          // компилятор видит объявление
int main() {
    int result = add(2, 3);      // линковщик найдёт определение
    return 0;
}
```

---

## 2. Передача параметров

> **Пример:** [02_parameter_passing](02_parameter_passing)

### По значению (by value)

Функция получает **копию** аргумента:

```cpp
void increment(int x) {
    x += 1;   // изменяется только локальная копия
}

int a = 10;
increment(a);
std::cout << a;  // 10 — не изменился
```

### По ссылке (by reference)

Функция работает с **оригинальным** объектом:

```cpp
void increment(int& x) {
    x += 1;   // изменяется оригинал
}

int a = 10;
increment(a);
std::cout << a;  // 11
```

### По указателю (by pointer)

Аналог ссылки, но указатель может быть `nullptr`:

```cpp
void increment(int* x) {
    if (x != nullptr) {
        *x += 1;
    }
}

int a = 10;
increment(&a);
std::cout << a;  // 11

increment(nullptr);  // безопасно — ничего не произойдёт
```

### По константной ссылке (by const reference)

Для чтения без копирования. Идеальный способ передачи «тяжёлых» объектов:

```cpp
void print(const std::string& s) {
    std::cout << s << std::endl;
    // s += "!";  // Ошибка компиляции — const!
}

std::string name = "Hello";
print(name);       // без копирования
print("World");    // rvalue тоже можно — привязка к const&
```

### Когда что использовать

| Тип параметра | Когда использовать |
|---------------|-------------------|
| `T` (по значению) | Дешёвые типы (int, double, ptr), или когда нужна копия |
| `const T&` | Чтение без копирования (строки, вектора, объекты) |
| `T&` | Функция должна модифицировать аргумент |
| `T*` | Необязательный параметр (может быть nullptr) |

---

## 3. Перегрузка функций

В C++ несколько функций могут иметь одно имя, если различаются их **списки параметров**:

```cpp
#include <iostream>
#include <string>

// Перегрузка по количеству параметров
int max(int a, int b) {
    return (a > b) ? a : b;
}

int max(int a, int b, int c) {
    return max(max(a, b), c);
}

// Перегрузка по типу параметров
double max(double a, double b) {
    return (a > b) ? a : b;
}

// Перегрузка по квалификатору const
void process(std::string& s) {
    std::cout << "non-const version: " << s << std::endl;
}

void process(const std::string& s) {
    std::cout << "const version: " << s << std::endl;
}

int main() {
    std::cout << max(3, 7)          << std::endl;  // int max(int,int)
    std::cout << max(3, 7, 5)       << std::endl;  // int max(int,int,int)
    std::cout << max(3.14, 2.71)    << std::endl;  // double max(double,double)

    std::string s = "hello";
    process(s);                                       // non-const version
    process(std::string("temp"));                     // const version (rvalue → const&)

    return 0;
}
```

> **Важно:** тип возвращаемого значения **не участвует** в разрешении перегрузки.

```cpp
int    foo(int x);
double foo(int x);  // Ошибка! Совпадает с первой по параметрам
```

---

## 4. Алгоритм разрешения перегрузки

Компилятор выбирает подходящую функцию в три этапа:

1. **Формирование множества кандидатов** — все функции с подходящим именем в текущей области видимости.
2. **Фильтрация** — удаляются функции, к которым аргументы не подходят (даже с преобразованиями).
3. **Ранжирование** — выбирается «наилучшее совпадение» по приоритету преобразований.

### Приоритет преобразований (от лучшего к худшему)

1. **Точное совпадение** (exact match) — включая trivial conversions (массив → указатель, функция → указатель).
2. **Promotion** — `short` → `int`, `float` → `double`, `bool` → `int`.
3. **Standard conversion** — `int` → `double`, `double` → `int`, указатели в иерархии.
4. **User-defined conversion** — конструкторы преобразования, операторы приведения.

```cpp
void foo(int x)    { std::cout << "int"    << std::endl; }
void foo(double x) { std::cout << "double" << std::endl; }
void foo(long x)   { std::cout << "long"   << std::endl; }

foo(42);       // exact match → foo(int)
foo(3.14);     // exact match → foo(double)
foo(42L);      // exact match → foo(long)
foo(3.14f);    // promotion float→double → foo(double)
// foo(42u);   // ambiguous! unsigned int → int и unsigned int → long одинаково хороши
```

Если несколько функций одинаково хороши — **ошибка компиляции** (ambiguity).

---

## 5. Аргументы по умолчанию

```cpp
#include <iostream>
#include <string>

// Аргументы по умолчанию задаются справа налево
void log(const std::string& message,
         const std::string& level = "INFO",
         bool timestamp = true)
{
    if (timestamp) {
        std::cout << "[TIME] ";
    }
    std::cout << "[" << level << "] " << message << std::endl;
}

int main() {
    log("Server started");                    // level="INFO", timestamp=true
    log("Connection lost", "WARNING");        // timestamp=true
    log("Debug data", "DEBUG", false);        // все аргументы явно

    return 0;
}
```

**Правила:**

1. Аргументы по умолчанию — только в **конце** списка параметров.
2. В объявлении и определении — указываются **один раз** (обычно в объявлении).
3. Не перегружайте и не задавайте умолчания одновременно — может быть неоднозначность:

```cpp
void foo(int a, int b = 10);
void foo(int a);              // неоднозначность при вызове foo(5)!
```

---

## 6. `inline` и `constexpr` функции

### `inline`-функции

Ключевое слово `inline` — **подсказка** компилятору встроить тело функции в место вызова (вместо генерации вызова через стек). В современном C++ основная роль `inline` — разрешить определение функции в заголовочном файле без нарушения ODR:

```cpp
// math.h
#pragma once

// inline позволяет определить функцию в заголовке
inline int square(int x) {
    return x * x;
}
```

Без `inline` определение в заголовке, включённом в несколько `.cpp`-файлов, привело бы к ошибке линковки (multiple definition).

> **Замечание:** современные компиляторы сами решают, инлайнить ли функцию, вне зависимости от `inline`. Ключевое слово влияет только на правила линковки.

### `constexpr`-функции

`constexpr`-функция **может** вычисляться на этапе компиляции, если все аргументы — константные выражения:

```cpp
constexpr int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

// Вычисление на этапе компиляции
constexpr int f5 = factorial(5);     // 120 — compile-time
static_assert(f5 == 120);

// Вычисление в рантайме (если аргумент не constexpr)
int n;
std::cin >> n;
int fn = factorial(n);               // runtime
```

С C++14 `constexpr`-функции могут содержать циклы, локальные переменные и условия:

```cpp
constexpr int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

static_assert(fibonacci(10) == 55);
```

---

## 7. Возвращаемые значения. RVO и NRVO

### Возврат по значению

```cpp
std::string create_greeting(const std::string& name) {
    std::string result = "Hello, " + name + "!";
    return result;   // казалось бы — копирование?
}

std::string greeting = create_greeting("World");
```

### RVO (Return Value Optimization)

**RVO** — оптимизация, при которой компилятор создаёт возвращаемый объект непосредственно в памяти вызывающей стороны, **минуя копирование**:

```cpp
// Без RVO: создание result → копирование в возвращаемое значение → копирование в greeting
// С RVO:   компилятор создаёт объект сразу в greeting

std::string create() {
    return std::string("hello");  // RVO (возврат prvalue)
}
```

С C++17 **RVO для prvalue гарантирована** стандартом (mandatory copy elision).

### NRVO (Named Return Value Optimization)

**NRVO** — то же, но для именованной локальной переменной. Не гарантируется стандартом, но реализуется всеми основными компиляторами:

```cpp
std::string create_named() {
    std::string result = "hello";
    result += " world";
    return result;    // NRVO — обычно без копирования
}
```

**Условия для NRVO:**

1. Возвращается локальная переменная (не параметр, не глобальная).
2. Тип переменной совпадает с типом возврата.
3. Все пути возврата возвращают одну и ту же переменную.

```cpp
// NRVO может НЕ сработать:
std::string bad_nrvo(bool flag) {
    std::string a = "hello";
    std::string b = "world";
    if (flag) return a;   // два разных объекта —
    return b;             // компилятор не знает, какой оптимизировать
}
```

### Практические рекомендации

```cpp
// ХОРОШО: возвращайте по значению — компилятор оптимизирует
std::vector<int> generate_data(int n) {
    std::vector<int> result;
    result.reserve(n);
    for (int i = 0; i < n; ++i) {
        result.push_back(i * i);
    }
    return result;   // NRVO или move
}

// ПЛОХО: не используйте std::move при возврате локальной переменной
std::vector<int> bad_return() {
    std::vector<int> result = {1, 2, 3};
    return std::move(result);  // запрещает NRVO!
}
```

---

## 8. Указатели на функции. `std::function`

### Указатели на функции

> **Пример:** [08_function_pointers](08_function_pointers)

Функция в C++ имеет адрес, который можно сохранить в указателе:

```cpp
#include <iostream>

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }

// Тип: указатель на функцию (int, int) → int
using BinaryOp = int(*)(int, int);

int apply(BinaryOp op, int x, int y) {
    return op(x, y);
}

int main() {
    // Имя функции неявно преобразуется в указатель
    BinaryOp operation = add;
    std::cout << operation(3, 4) << std::endl;   // 7

    operation = mul;
    std::cout << operation(3, 4) << std::endl;   // 12

    // Передача функции как аргумента
    std::cout << apply(add, 10, 20) << std::endl; // 30
    std::cout << apply(mul, 10, 20) << std::endl; // 200

    // Массив указателей на функции
    BinaryOp ops[] = {add, mul};
    for (auto op : ops) {
        std::cout << op(5, 3) << " ";
    }
    std::cout << std::endl;  // "8 15"

    return 0;
}
```

### `std::function` (обзор)

`std::function` — обёртка из `<functional>`, способная хранить любой вызываемый объект: функцию, лямбду, функтор:

```cpp
#include <iostream>
#include <functional>

int add(int a, int b) { return a + b; }

int main() {
    // std::function может хранить обычную функцию
    std::function<int(int, int)> op = add;
    std::cout << op(3, 4) << std::endl;  // 7

    // ... или лямбду
    op = [](int a, int b) { return a - b; };
    std::cout << op(10, 3) << std::endl;  // 7

    // ... или лямбду с захватом
    int factor = 10;
    std::function<int(int)> scaled = [factor](int x) { return x * factor; };
    std::cout << scaled(5) << std::endl;  // 50

    // Проверка на пустоту
    std::function<void()> empty;
    if (!empty) {
        std::cout << "empty is null" << std::endl;
    }

    return 0;
}
```

**Когда что использовать:**

| Тип | Достоинства | Недостатки |
|-----|------------|------------|
| Указатель на функцию | Нулевой overhead | Не хранит лямбды с захватом |
| `std::function` | Универсальный | Heap allocation, виртуальный вызов |
| Шаблонный параметр | Нулевой overhead, универсальный | Шаблонный код |

---

## 9. Рекурсия

### Прямая рекурсия

```cpp
// Классический факториал
int factorial(int n) {
    if (n <= 1) return 1;      // базовый случай
    return n * factorial(n - 1); // рекурсивный случай
}

// Числа Фибоначчи (наивная — экспоненциальная сложность!)
int fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}
```

### Хвостовая рекурсия (tail recursion)

Рекурсивный вызов — последняя операция в функции. Компилятор может оптимизировать в цикл:

```cpp
int factorial_tail(int n, int acc = 1) {
    if (n <= 1) return acc;
    return factorial_tail(n - 1, n * acc);  // хвостовой вызов
}

// Компилятор (с -O2) может превратить в:
// int factorial_tail(int n, int acc) {
//     while (n > 1) { acc *= n; --n; }
//     return acc;
// }
```

### Взаимная рекурсия

Две функции вызывают друг друга:

```cpp
// Объявления (необходимы для взаимной рекурсии)
bool is_even(unsigned n);
bool is_odd(unsigned n);

bool is_even(unsigned n) {
    if (n == 0) return true;
    return is_odd(n - 1);
}

bool is_odd(unsigned n) {
    if (n == 0) return false;
    return is_even(n - 1);
}

// is_even(4) → is_odd(3) → is_even(2) → is_odd(1) → is_even(0) → true
```

> **Внимание:** глубокая рекурсия может привести к переполнению стека (stack overflow). Для больших значений используйте итеративные алгоритмы.

---

## 10. ADL (Argument-Dependent Lookup)

ADL (также известный как Koenig Lookup) — механизм, при котором компилятор ищет функцию не только в текущей области видимости, но и в **пространствах имён аргументов**:

```cpp
#include <iostream>
#include <string>

namespace geometry {
    struct Point {
        double x, y;
    };

    // Функция в том же namespace, что и Point
    void print(const Point& p) {
        std::cout << "(" << p.x << ", " << p.y << ")" << std::endl;
    }

    // Оператор << для Point
    std::ostream& operator<<(std::ostream& os, const Point& p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
}

int main() {
    geometry::Point p{3.0, 4.0};

    // ADL находит geometry::print, хотя мы не написали geometry::
    print(p);   // OK благодаря ADL

    // ADL — причина, по которой работает operator<< без квалификации
    std::cout << p << std::endl;  // ADL находит geometry::operator<<

    // Без ADL пришлось бы писать:
    // geometry::operator<<(std::cout, p);  // неудобно!

    return 0;
}
```

### Где ADL критически важен

1. **Операторы** — `std::cout << x` работает благодаря ADL (ищет `operator<<` в namespace типа `x`).
2. **Идиома `swap`:**

```cpp
namespace my {
    struct Widget { /* ... */ };
    void swap(Widget& a, Widget& b) { /* оптимизированная версия */ }
}

template <typename T>
void generic_sort(T& a, T& b) {
    using std::swap;     // делаем std::swap видимым
    if (a > b) swap(a, b);  // ADL найдёт my::swap для my::Widget
}
```

3. **`begin`/`end` для range-based for** — ADL позволяет найти пользовательские `begin`/`end`.

---

## Ключевые концепции

1. **Объявление** сообщает о существовании функции; **определение** содержит тело. ODR требует ровно одно определение.
2. **`const T&`** — основной способ передачи «тяжёлых» объектов (без копирования, без модификации).
3. **Перегрузка** различает функции по типам и количеству параметров, но **не** по типу возврата.
4. **RVO/NRVO** устраняют копирование при возврате по значению. С C++17 RVO для prvalue гарантирована.
5. **`constexpr`-функции** вычисляются на этапе компиляции, если аргументы — константные выражения.
6. **Указатели на функции** позволяют передавать поведение как данные.
7. **ADL** ищет функции в пространствах имён аргументов — критически важен для операторов и идиомы swap.

---

## Распространённые ошибки

### 1. `std::move` при возврате локальной переменной

```cpp
std::vector<int> bad() {
    std::vector<int> v = {1, 2, 3};
    return std::move(v);  // ПЛОХО: запрещает NRVO!
}
// Правильно: return v;
```

### 2. Неоднозначность при перегрузке с default-аргументами

```cpp
void foo(int a, int b = 0);
void foo(int a);
// foo(5);  // Ошибка: ambiguous — подходят обе перегрузки
```

### 3. Передача больших объектов по значению

```cpp
void process(std::vector<int> data) {    // копирование всего вектора!
    // ...
}
// Правильно:
void process(const std::vector<int>& data) {  // только ссылка
    // ...
}
```

### 4. Бесконечная рекурсия (забытый базовый случай)

```cpp
int factorial(int n) {
    return n * factorial(n - 1);  // нет if (n <= 1) — stack overflow!
}
```

### 5. Dangling reference при возврате ссылки на локальную переменную

```cpp
int& bad_ref() {
    int x = 42;
    return x;   // ОШИБКА: x уничтожается — dangling reference!
}
```

---

## Вопросы для самопроверки

1. В чём разница между объявлением и определением функции?
2. Когда передавать параметр по значению, а когда по `const&`?
3. Участвует ли тип возвращаемого значения в разрешении перегрузки?
4. В каком порядке компилятор предпочитает преобразования при перегрузке?
5. Что такое RVO и NRVO? Когда NRVO может не сработать?
6. Почему `return std::move(local)` хуже, чем `return local`?
7. Чем `std::function` отличается от указателя на функцию?
8. Что такое хвостовая рекурсия? Почему она эффективнее обычной?
9. Как ADL помогает при использовании `std::cout << custom_object`?
10. Можно ли иметь `constexpr`-функцию с циклом?

---

## Практическое задание

### Задание: «Библиотека числовых операций»

Создайте библиотеку функций с перегрузками и протестируйте её:

**Часть 1. Перегрузка функций**

Реализуйте семейство перегруженных функций `clamp`, ограничивающих значение диапазоном:

```cpp
int clamp(int value, int low, int high);
double clamp(double value, double low, double high);
// Шаблонная версия (бонус):
// template <typename T>
// T clamp(T value, T low, T high);
```

**Часть 2. Функции высшего порядка**

Реализуйте функцию `transform_array`, принимающую массив и функцию-преобразователь:

```cpp
using TransformFn = int(*)(int);

void transform_array(int* arr, size_t size, TransformFn fn);

// Примеры функций-преобразователей:
int square(int x);
int negate(int x);
int abs_val(int x);
```

**Часть 3. `constexpr`-вычисления**

Реализуйте `constexpr`-функции и проверьте через `static_assert`:

```cpp
constexpr int power(int base, int exp);       // base^exp
constexpr bool is_prime(int n);                // проверка простоты
constexpr int gcd(int a, int b);              // НОД (алгоритм Евклида)
```

**Часть 4. Тесты**

Напишите Google Test тесты для всех функций. Убедитесь в корректности перегрузки — тесты должны вызывать каждую перегруженную версию.

**Требования:**
- Разделите код на `functions.h` (объявления), `functions.cpp` (определения), `tests.cpp`
- Компиляция с `-std=c++20 -Wall -Wextra`
- Все `constexpr`-функции проверены через `static_assert`

---

## Ссылки на примеры

| Тема | Пример |
|------|--------|
| Объявление vs определение функции. Прототипы | [01_declaration_definition](01_declaration_definition) |
| Передача параметров | [02_parameter_passing](02_parameter_passing) |
| Перегрузка функций | [03_overloading](03_overloading) |
| Алгоритм разрешения перегрузки | [04_overload_resolution](04_overload_resolution) |
| Аргументы по умолчанию | [05_default_arguments](05_default_arguments) |
| `inline`-функции и `constexpr`-функции | [06_inline_constexpr](06_inline_constexpr) |
| Возвращаемые значения. RVO и NRVO | [07_rvo_nrvo](07_rvo_nrvo) |
| Указатели на функции. `std::function` | [08_function_pointers](08_function_pointers) |
| Рекурсия: прямая, хвостовая, взаимная | [09_recursion](09_recursion) |
| ADL (Argument-Dependent Lookup) | [10_adl](10_adl) |
