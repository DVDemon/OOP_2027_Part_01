# Лекция 2. Операторы, ввод-вывод, управляющие конструкции

## План лекции

1. Арифметические операторы
2. Побитовые операторы
3. Логические операторы и операторы сравнения
4. Приоритет и ассоциативность операторов. Префиксный/постфиксный инкремент
5. Составные операторы присваивания. `std::numeric_limits`
6. Математические функции `<cmath>`
7. Потоковый ввод-вывод: `std::cin`, `std::cout`, манипуляторы
8. Условные конструкции: `if` (включая C++17 init-if), `switch`
9. Циклы: `for`, range-based `for`, `while`, `do-while`
10. `break`, `continue`, оператор «запятая»
11. Неявные и явные преобразования типов: promotion, usual arithmetic conversions, `static_cast`

---

## 1. Арифметические операторы

Стандартные арифметические операторы работают с числовыми типами:

```cpp
#include <iostream>

int main() {
    int a = 17, b = 5;

    std::cout << "a + b  = " << (a + b)  << std::endl;  // 22
    std::cout << "a - b  = " << (a - b)  << std::endl;  // 12
    std::cout << "a * b  = " << (a * b)  << std::endl;  // 85
    std::cout << "a / b  = " << (a / b)  << std::endl;  // 3 (целочисленное деление!)
    std::cout << "a % b  = " << (a % b)  << std::endl;  // 2 (остаток от деления)

    // Деление с плавающей точкой
    double c = 17.0, d = 5.0;
    std::cout << "c / d  = " << (c / d)  << std::endl;  // 3.4

    // Внимание: целочисленное переполнение — UB для знаковых типов
    // int overflow = INT_MAX + 1;  // undefined behavior!

    return 0;
}
```

> **Важно:** деление целых чисел даёт целый результат с усечением к нулю. `17 / 5 == 3`, а не `3.4`.

> **Основные тезисы:**
> - Целочисленное деление усекает дробную часть в сторону нуля: `17 / 5 == 3`; `%` определён только для целых типов.
> - Деление или остаток на ноль — UB; переполнение знаковых типов — тоже UB (для `unsigned` определён wrap-around по модулю 2^n).
> - Перед операцией применяются целочисленные продвижения и usual arithmetic conversions: `5 + 2.0` даёт `double`.
> - **Стандарт:** `[expr.mul]`, `[expr.arith.conv]`, `[expr.pre]`.
> - **Core Guidelines:** [ES.103](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es103) — не переполняйте; [ES.105](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es105) — не делите на ноль; [ES.100](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es100) — не смешивайте signed/unsigned.

---

## 2. Побитовые операторы

Побитовые операторы работают с отдельными битами целых чисел:

```cpp
#include <iostream>
#include <bitset>

int main() {
    unsigned int a = 0b1100;  // 12
    unsigned int b = 0b1010;  // 10

    std::cout << "a & b  = " << std::bitset<4>(a & b)  << std::endl;  // 1000 (AND)
    std::cout << "a | b  = " << std::bitset<4>(a | b)  << std::endl;  // 1110 (OR)
    std::cout << "a ^ b  = " << std::bitset<4>(a ^ b)  << std::endl;  // 0110 (XOR)
    std::cout << "~a     = " << std::bitset<8>(~a)      << std::endl;  // 11110011 (NOT)
    std::cout << "a << 1 = " << std::bitset<8>(a << 1)  << std::endl;  // 00011000 (сдвиг влево)
    std::cout << "a >> 1 = " << std::bitset<4>(a >> 1)  << std::endl;  // 0110 (сдвиг вправо)

    // Практическое применение: битовые флаги
    constexpr unsigned int READ    = 0b001;
    constexpr unsigned int WRITE   = 0b010;
    constexpr unsigned int EXECUTE = 0b100;

    unsigned int permissions = READ | WRITE;           // установить флаги
    bool can_read = (permissions & READ) != 0;         // проверить флаг
    permissions &= ~WRITE;                             // сбросить флаг

    return 0;
}
```

> **Основные тезисы:**
> - Побитовые операции применяются к целым типам; для работы с битами предпочитайте `unsigned`.
> - Сдвиг влево знакового значения с переполнением — UB; сдвиг вправо отрицательного числа — implementation-defined.
> - Типовой приём — битовые флаги: установить `|=`, проверить `&`, сбросить `&= ~`.
> - `std::bitset` удобен для печати и отладки битового представления.
> - **Стандарт:** `[expr.shift]`, `[expr.bit.and]`, `[expr.bit.or]`, `[expr.bit.xor]`, `[expr.unary.op]`.
> - **Core Guidelines:** [ES.101](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es101) — для битовых операций используйте `unsigned`; [ES.102](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es102) — для арифметики используйте `signed`; [ES.100](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es100) — не смешивайте signed/unsigned.

---

## 3. Логические операторы и операторы сравнения

> **Пример:** [03_logical_comparison](03_logical_comparison)

```cpp
#include <iostream>

int main() {
    bool a = true, b = false;

    // Логические операторы
    std::cout << "a && b = " << (a && b) << std::endl;  // 0 (AND)
    std::cout << "a || b = " << (a || b) << std::endl;  // 1 (OR)
    std::cout << "!a     = " << (!a)     << std::endl;  // 0 (NOT)

    // Операторы сравнения
    int x = 10, y = 20;
    std::cout << (x == y) << std::endl;  // 0
    std::cout << (x != y) << std::endl;  // 1
    std::cout << (x < y)  << std::endl;  // 1
    std::cout << (x >= y) << std::endl;  // 0

    // Короткая схема вычислений (short-circuit evaluation)
    // Если левый операнд && равен false — правый не вычисляется
    int* ptr = nullptr;
    if (ptr != nullptr && *ptr > 0) {
        // безопасно: *ptr не вычисляется при nullptr
    }

    return 0;
}
```

**Short-circuit evaluation** — важнейшее свойство `&&` и `||`: правый операнд вычисляется только если нужен для определения результата.

> **Основные тезисы:**
> - `&&` и `||` вычисляются по короткой схеме (short-circuit): правый операнд — только если нужен для результата.
> - Логические операции возвращают `bool`; сравнения применимы к арифметическим типам и тоже возвращают `bool`.
> - Числа с плавающей точкой нельзя сравнивать через `==` — сравнивайте с погрешностью (epsilon).
> - **Стандарт:** `[expr.log.and]`, `[expr.log.or]`, `[expr.log.not]`, `[expr.eq]`, `[expr.rel]`.
> - **Core Guidelines:** [ES.87](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es87) — не добавляйте избыточные `==`/`!=` в условия.

---

## 4. Приоритет и ассоциативность. Инкремент/декремент

### Таблица приоритетов (основные)

| Приоритет | Оператор | Ассоциативность |
|-----------|----------|-----------------|
| 1 (высш.) | `::` | Левая |
| 2 | `a++`, `a--` (постфиксные) | Левая |
| 3 | `++a`, `--a`, `!`, `~`, `*`, `&` (унарные) | Правая |
| 5 | `*`, `/`, `%` | Левая |
| 6 | `+`, `-` | Левая |
| 7 | `<<`, `>>` | Левая |
| 9 | `<`, `<=`, `>`, `>=` | Левая |
| 10 | `==`, `!=` | Левая |
| 14 | `&&` | Левая |
| 15 | `||` | Левая |
| 16 | `=`, `+=`, `-=` ... | Правая |

> **Правило:** при сомнениях — ставьте скобки. Читаемость важнее краткости.

### Префиксный и постфиксный инкремент

> **Пример:** [04_precedence_increment](04_precedence_increment)

```cpp
#include <iostream>

int main() {
    int a = 5;

    int b = ++a;  // Сначала увеличить a (a=6), затем вернуть. b=6, a=6
    int c = a++;  // Сначала вернуть a (c=6), затем увеличить. c=6, a=7

    std::cout << "a = " << a << std::endl;  // 7
    std::cout << "b = " << b << std::endl;  // 6
    std::cout << "c = " << c << std::endl;  // 6

    // Для встроенных типов разницы в производительности нет
    // Для итераторов и сложных типов — предпочитайте ++i (префиксный)

    return 0;
}
```

> **Основные тезисы:**
> - Приоритет и ассоциативность фиксированы стандартом; при сомнении ставьте скобки — читаемость важнее краткости.
> - Постфиксный `++` по приоритету выше префиксного; префиксные унарные операторы — правоассоциативны.
> - Для встроенных типов `++i` и `i++` в простых выражениях эквивалентны; для итераторов и сложных типов префиксный вариант не создаёт временных копий.
> - **Стандарт:** `[expr.post.incr]`, `[expr.pre.incr]`, `[expr.ass]`; полная таблица приоритетов — в грамматике выражений (`[expr.compound]`).
> - **Core Guidelines:** [ES.41](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es41) — при сомнении в приоритете берите выражение в скобки.

---

## 5. Составные операторы присваивания. `numeric_limits`

### Составные операторы

> **Пример:** [05_compound_numeric_limits](05_compound_numeric_limits)

```cpp
int x = 10;
x += 5;   // x = x + 5;    → 15
x -= 3;   // x = x - 3;    → 12
x *= 2;   // x = x * 2;    → 24
x /= 4;   // x = x / 4;    → 6
x %= 4;   // x = x % 4;    → 2
x <<= 2;  // x = x << 2;   → 8
x >>= 1;  // x = x >> 1;   → 4
x &= 0b11; // x = x & 0b11; → 0
```

### `std::numeric_limits`

> **Пример:** [05_compound_numeric_limits](05_compound_numeric_limits)

```cpp
#include <iostream>
#include <limits>

int main() {
    std::cout << "int min:    " << std::numeric_limits<int>::min() << std::endl;
    std::cout << "int max:    " << std::numeric_limits<int>::max() << std::endl;
    std::cout << "int digits: " << std::numeric_limits<int>::digits << std::endl;

    std::cout << "double min:     " << std::numeric_limits<double>::min() << std::endl;
    std::cout << "double max:     " << std::numeric_limits<double>::max() << std::endl;
    std::cout << "double epsilon: " << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "double inf:     " << std::numeric_limits<double>::infinity() << std::endl;

    // Проверка на NaN
    double nan_val = std::numeric_limits<double>::quiet_NaN();
    std::cout << "NaN == NaN: " << (nan_val == nan_val) << std::endl;  // 0 (false!)

    return 0;
}
```

> **Основные тезисы:**
> - `x op= y` эквивалентно `x = x op y`, но левый операнд вычисляется один раз; составных форм для `&&`, `||` и оператора запятой не существует.
> - `std::numeric_limits<T>` — типобезопасные константы и свойства типов вместо макросов `INT_MAX`/`DBL_MAX`.
> - `min()` для типов с плавающей точкой — наименьшее положительное нормализованное значение; наименьшее конечное — `lowest()`.
> - `NaN` не равен самому себе: проверяйте через `std::isnan`, а не через `==`.
> - **Стандарт:** `[expr.ass]`, `[limits]`.
> - **Core Guidelines:** [ES.31](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es31) — не используйте макросы для констант (вместо них `constexpr`/`numeric_limits`); [ES.45](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es45) — избегайте «магических чисел».

---

## 6. Математические функции `<cmath>`

> **Пример:** [06_cmath](06_cmath)

```cpp
#include <iostream>
#include <cmath>

int main() {
    // Основные функции
    std::cout << "abs(-5)     = " << std::abs(-5)      << std::endl;
    std::cout << "sqrt(144)   = " << std::sqrt(144.0)   << std::endl;
    std::cout << "pow(2, 10)  = " << std::pow(2.0, 10)  << std::endl;
    std::cout << "ceil(3.2)   = " << std::ceil(3.2)     << std::endl;  // 4
    std::cout << "floor(3.8)  = " << std::floor(3.8)    << std::endl;  // 3
    std::cout << "round(3.5)  = " << std::round(3.5)    << std::endl;  // 4

    // Тригонометрия (аргумент в радианах)
    std::cout << "sin(pi/2)   = " << std::sin(M_PI / 2) << std::endl;
    std::cout << "cos(0)      = " << std::cos(0.0)      << std::endl;

    // Логарифмы
    std::cout << "log(e)      = " << std::log(M_E)      << std::endl;  // 1.0
    std::cout << "log10(1000) = " << std::log10(1000.0)  << std::endl; // 3.0
    std::cout << "log2(1024)  = " << std::log2(1024.0)   << std::endl; // 10.0

    return 0;
}
```

> **Основные тезисы:**
> - Функции `<cmath>` работают с `double` (есть перегрузки для `float`, `long double` и целых); углы — в радианах.
> - `M_PI`, `M_E` — нестандартные расширения GNU; в C++20 есть `std::numbers::pi` из `<numbers>`.
> - Не путайте C-заголовок `<math.h>` и C++-обёртку `<cmath>`; `std::abs` перегружен и для целых, и для `double`.
> - `std::pow(x, 2)` для целых может быть заметно медленнее `x * x`.
> - **Стандарт:** `[cmath.syn]`, `[c.math]`, `[numbers]` (C++20).

---

## 7. Потоковый ввод-вывод

### `std::cout` и форматирование

> **Пример:** [07_stream_io](07_stream_io)

```cpp
#include <iostream>
#include <iomanip>

int main() {
    // Базовый вывод
    std::cout << "Hello" << ' ' << "World" << std::endl;

    // Манипуляторы форматирования
    double pi = 3.141592653589793;

    std::cout << std::fixed << std::setprecision(2) << pi << std::endl;  // 3.14
    std::cout << std::scientific << pi << std::endl;  // 3.14e+00

    // Ширина поля и выравнивание
    std::cout << std::setw(10) << std::right << 42 << std::endl;    // "        42"
    std::cout << std::setw(10) << std::left  << 42 << std::endl;    // "42        "
    std::cout << std::setfill('0') << std::setw(5) << 42 << std::endl; // "00042"

    // Системы счисления
    int num = 255;
    std::cout << std::dec << num << std::endl;   // 255
    std::cout << std::hex << num << std::endl;   // ff
    std::cout << std::oct << num << std::endl;   // 377
    std::cout << std::showbase << std::hex << num << std::endl; // 0xff

    // Логические значения
    std::cout << std::boolalpha << true << std::endl;  // "true"

    return 0;
}
```

### `std::cin`

```cpp
#include <iostream>
#include <string>

int main() {
    // Чтение числа
    int age;
    std::cout << "Введите возраст: ";
    std::cin >> age;

    // Чтение строки (до пробела)
    std::string name;
    std::cout << "Введите имя: ";
    std::cin >> name;

    // Чтение строки целиком (до конца строки)
    std::cin.ignore();  // пропустить '\n' после предыдущего >>
    std::string full_name;
    std::cout << "Введите полное имя: ";
    std::getline(std::cin, full_name);

    // Проверка корректности ввода
    int value;
    std::cout << "Введите число: ";
    if (std::cin >> value) {
        std::cout << "Прочитано: " << value << std::endl;
    } else {
        std::cout << "Ошибка ввода!" << std::endl;
        std::cin.clear();             // сбросить флаг ошибки
        std::cin.ignore(1000, '\n');  // очистить буфер
    }

    return 0;
}
```

> **Основные тезисы:**
> - `>>` читает до разделителя-пробела, `std::getline` — до конца строки; после `>>` в буфере остаётся `'\n'`.
> - Результат ввода проверяется как условие: `if (std::cin >> x)`; при неудаче поток переходит в состояние ошибки (восстановление — `clear()` + `ignore()`).
> - Манипуляторы `setw`/`setprecision`/`fixed`/`hex`/`boolalpha` меняют формат вывода; `setw` действует только на следующее значение.
> - Для производительности предпочитайте `'\n'` вместо `std::endl` — `endl` дополнительно сбрасывает буфер.
> - **Стандарт:** `[iostream.objects]`, `[istream]`, `[ostream]`; в C++20 появился `std::format`.

---

## 8. Условные конструкции

### `if` и `else`

> **Пример:** [08_conditionals](08_conditionals)

```cpp
int score = 85;

if (score >= 90) {
    std::cout << "Отлично" << std::endl;
} else if (score >= 75) {
    std::cout << "Хорошо" << std::endl;
} else if (score >= 60) {
    std::cout << "Удовлетворительно" << std::endl;
} else {
    std::cout << "Неудовлетворительно" << std::endl;
}
```

### `if` с инициализатором (C++17)

```cpp
#include <map>
#include <string>

std::map<std::string, int> grades = {{"Иванов", 85}, {"Петров", 92}};

// C++17: переменная it ограничена областью видимости if
if (auto it = grades.find("Иванов"); it != grades.end()) {
    std::cout << "Оценка: " << it->second << std::endl;
} else {
    std::cout << "Студент не найден" << std::endl;
}
// it здесь уже недоступна
```

### `switch`

> **Пример:** [08_conditionals](08_conditionals)

```cpp
enum class Color { Red, Green, Blue };

Color c = Color::Green;

switch (c) {
    case Color::Red:
        std::cout << "Красный" << std::endl;
        break;
    case Color::Green:
        std::cout << "Зелёный" << std::endl;
        break;
    case Color::Blue:
        std::cout << "Синий" << std::endl;
        break;
    // Нет default — компилятор предупредит, если не все значения обработаны
}

// C++17: switch с инициализатором
switch (int val = compute(); val) {
    case 0:  /* ... */ break;
    case 1:  /* ... */ break;
    default: /* ... */ break;
}
```

> **Внимание:** забытый `break` приводит к «проваливанию» (fall-through). В C++17 используйте атрибут `[[fallthrough]]` для явного указания намеренного проваливания.

> **Основные тезисы:**
> - В цепочке `if`-`else if` выполняется первое истинное условие — порядок проверок важен.
> - Init-`if` (C++17) ограничивает область видимости вспомогательной переменной рамками `if`/`else` — «время жизни» имени сокращается.
> - `switch` работает с целыми типами и `enum`; забытый `break` даёт проваливание — намеренное помечайте `[[fallthrough]]` (C++17).
> - **Стандарт:** `[stmt.if]`, `[stmt.switch]`, `[dcl.attr.fallthrough]`.
> - **Core Guidelines:** [ES.5](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es5) — держите области видимости малыми; [ES.78](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es78) — в конце непустого `case` делайте явное действие и не полагайтесь на проваливание.

---

## 9. Циклы

### Цикл `for`

> **Пример:** [09_loops](09_loops)

```cpp
// Классический for
for (int i = 0; i < 10; ++i) {
    std::cout << i << " ";
}
std::cout << std::endl;

// Несколько переменных
for (int i = 0, j = 10; i < j; ++i, --j) {
    std::cout << i << ":" << j << " ";
}
```

### Range-based `for` (C++11)

> **Пример:** [09_loops](09_loops)

```cpp
#include <vector>
#include <string>

std::vector<std::string> names = {"Алиса", "Борис", "Вера"};

// По значению (копия)
for (auto name : names) {
    std::cout << name << std::endl;
}

// По константной ссылке (без копирования, без изменения)
for (const auto& name : names) {
    std::cout << name << std::endl;
}

// По ссылке (можно изменять)
for (auto& name : names) {
    name += "!";
}

// Инициализатор-список
for (int x : {1, 2, 3, 4, 5}) {
    std::cout << x << " ";
}
```

### Циклы `while` и `do-while`

> **Пример:** [09_loops](09_loops)

```cpp
// while — проверка перед итерацией
int n = 1;
while (n <= 1024) {
    std::cout << n << " ";
    n *= 2;
}

// do-while — тело выполняется хотя бы один раз
int input;
do {
    std::cout << "Введите положительное число: ";
    std::cin >> input;
} while (input <= 0);
```

### Вложенные циклы

> **Пример:** [09_loops](09_loops)

```cpp
// Таблица умножения
for (int i = 1; i <= 9; ++i) {
    for (int j = 1; j <= 9; ++j) {
        std::cout << std::setw(4) << i * j;
    }
    std::cout << std::endl;
}
```

> **Основные тезисы:**
> - `for` — инициализация/условие/шаг; переменная цикла живёт в области цикла.
> - Range-based `for` перебирает контейнеры без индексов: `const auto&` — без копирования, `auto&` — с изменением элементов.
> - `while` проверяет условие до тела; `do-while` гарантирует хотя бы одну итерацию (пример — повтор ввода).
> - **Стандарт:** `[stmt.for]`, `[stmt.ranged]`, `[stmt.while]`, `[stmt.do]`.
> - **Core Guidelines:** [ES.71](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es71) — предпочитайте range-for; [ES.74](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es74) — объявляйте переменную цикла в инициализаторе `for`; [ES.75](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es75) — избегайте `do`-циклов, но для ввода с повторением они оправданы.

---

## 10. `break`, `continue`, оператор «запятая»

### `break` и `continue`

> **Пример:** [10_break_continue_comma](10_break_continue_comma)

```cpp
// break — выход из цикла
for (int i = 0; i < 100; ++i) {
    if (i * i > 50) {
        std::cout << "Первое i, где i*i > 50: " << i << std::endl;
        break;  // выход
    }
}

// continue — переход к следующей итерации
for (int i = 0; i < 20; ++i) {
    if (i % 3 == 0) continue;  // пропустить кратные 3
    std::cout << i << " ";
}
```

### Оператор «запятая»

Оператор запятая вычисляет оба операнда слева направо и возвращает результат правого:

```cpp
int a = (1, 2, 3);  // a = 3 (результат последнего выражения)

// Чаще всего встречается в for:
for (int i = 0, j = 10; i < j; ++i, --j) {
    // i увеличивается, j уменьшивается одновременно
}
```

> **Основные тезисы:**
> - `break` завершает ближайший цикл или `switch`; `continue` переходит к следующей итерации.
> - Глубокие вложенные циклы с `break`/`continue` тяжело читать — выносите внутреннюю логику в функцию.
> - Оператор «запятая» вычисляет операнды слева направо и возвращает результат правого; вне инициализатора `for` он ухудшает читаемость.
> - **Стандарт:** `[stmt.break]`, `[stmt.cont]`, `[expr.comma]`.
> - **Core Guidelines:** [ES.77](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es77) — минимизируйте `break`/`continue` в циклах; [ES.40](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es40) — избегайте сложных выражений.

---

## 11. Неявные и явные преобразования типов

> **Пример:** [11_type_conversions](11_type_conversions)

### Неявные преобразования

```cpp
int i = 42;
double d = i;           // int → double (безопасно, расширение)
int j = d;              // double → int (потеря дробной части! warning)

// Integer promotion
short s = 10;
auto result = s + 1;    // short → int перед сложением, result — int

// Usual arithmetic conversions
int a = 5;
double b = 2.5;
auto c = a + b;          // int → double, c — double
```

### Явные преобразования (C++ casts)

```cpp
double pi = 3.14159;
int truncated = static_cast<int>(pi);    // 3 — явное преобразование

// Избегайте C-style cast: (int)pi — он слишком мощный и непредсказуемый
```

> **Основные тезисы:**
> - Неявные преобразования «выравнивают» типы перед операцией: продвижения (`short`→`int`) и usual arithmetic conversions (`int`→`double`).
> - Расширяющие преобразования безопасны; сужающие (narrowing) теряют данные — `{}`-инициализация их запрещает.
> - Явные преобразования: `static_cast`/`const_cast`/`reinterpret_cast`/`dynamic_cast`; C-style cast `(T)x` сочетает их сразу и опасен.
> - **Стандарт:** `[conv]`, `[conv.prom]`, `[expr.arith.conv]`, `[expr.static.cast]`, `[expr.cast]`, `[dcl.init.list]`.
> - **Core Guidelines:** [ES.23](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es23) — предпочитайте `{}`-инициализацию; [ES.46](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es46) — избегайте сужающих преобразований; [ES.48](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es48) — избегайте приведений; [ES.49](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es49) — если приводите, используйте именованные `*_cast`.

---

## Ключевые концепции

1. **Целочисленное деление** отбрасывает дробную часть: `7 / 2 == 3`.
2. **Short-circuit evaluation** в `&&` и `||` — правый операнд может не вычисляться.
3. **Префиксный инкремент** (`++i`) для итераторов и сложных типов предпочтительнее постфиксного (`i++`).
4. **`std::numeric_limits`** — типобезопасная замена макросов `INT_MAX`, `DBL_MIN` и т.д.
5. **Range-based `for`** с `const auto&` — идиоматический способ перебора контейнеров без копирования.
6. **`if` с инициализатором** (C++17) ограничивает область видимости вспомогательных переменных.
7. **Неявные преобразования** могут терять данные — используйте `{}` для защиты.

---

## Распространённые ошибки

### 1. Сравнение `float`/`double` через `==`

```cpp
double a = 0.1 + 0.2;
if (a == 0.3) { /* может не выполниться! */ }

// Правильно:
constexpr double eps = 1e-9;
if (std::abs(a - 0.3) < eps) { /* ... */ }
```

### 2. Забытый `break` в `switch`

```cpp
switch (x) {
    case 1:
        std::cout << "one";
        // забыли break — проваливаемся в case 2!
    case 2:
        std::cout << "two";
        break;
}
```

### 3. Использование `=` вместо `==` в условии

```cpp
int x = 5;
if (x = 0) {          // присваивание! x стал 0, условие false
    // никогда не выполнится
}
// Правильно: if (x == 0)
// Совет: компилятор с -Wall предупредит
```

### 4. Переполнение знаковых целых

```cpp
int x = std::numeric_limits<int>::max();
x += 1;  // undefined behavior! (для signed)
// Для unsigned — определено (wraparound)
```

### 5. Путаница `std::cin >> str` и `std::getline`

```cpp
int n;
std::cin >> n;          // читает число, '\n' остаётся в буфере
std::string line;
std::getline(std::cin, line);  // прочитает пустую строку!
// Решение: std::cin.ignore() между >> и getline
```

---

## Вопросы для самопроверки

1. Чему равно выражение `7 / 2` и `7.0 / 2`? Почему результаты различаются?
2. Что такое short-circuit evaluation? Приведите пример, где это критически важно.
3. В чём разница между `++i` и `i++`? Когда разница существенна?
4. Как C++17 init-if (`if (auto x = ...; condition)`) улучшает код?
5. Почему нельзя сравнивать `double` через `==`?
6. Что произойдёт, если забыть `break` в `switch`?
7. Какой тип имеет выражение `5 + 2.0`?
8. Зачем нужен `std::cin.ignore()` перед `std::getline`?
9. Как вывести число в шестнадцатеричном формате с префиксом `0x`?
10. Чем `while` отличается от `do-while` с точки зрения минимального количества итераций?

---

## Практическое задание

### Задание: «Статистический калькулятор»

Напишите программу, которая:

1. Считывает с `std::cin` последовательность целых чисел до тех пор, пока пользователь не введёт `0` (используйте `do-while`).
2. Для введённой последовательности вычисляет:
   - Количество чисел (не считая завершающий ноль)
   - Сумму, среднее арифметическое (с точностью 2 знака после запятой)
   - Минимум и максимум
   - Количество чётных и нечётных чисел
3. Выводит результат в отформатированной таблице с использованием `std::setw`, `std::left`, `std::fixed`, `std::setprecision`.
4. Если пользователь сразу ввёл `0` — выведите сообщение «Пустая последовательность» (используйте `if` с проверкой).
5. Дополнительно (C++17): используйте structured bindings для пары min/max, если знакомы с `std::pair`.

**Требования:**
- Компиляция с `-std=c++20 -Wall -Wextra`
- Никаких глобальных переменных
- Используйте `std::numeric_limits<int>::min()` / `max()` для инициализации min/max

---

## Ссылки на примеры

| Тема | Пример |
|------|--------|
| Арифметические операторы | [01_arithmetic](01_arithmetic) |
| Побитовые операторы | [02_bitwise](02_bitwise) |
| Логические операторы и операторы сравнения | [03_logical_comparison](03_logical_comparison) |
| Приоритет, ассоциативность, инкремент/декремент | [04_precedence_increment](04_precedence_increment) |
| Составные операторы присваивания. `std::numeric_limits` | [05_compound_numeric_limits](05_compound_numeric_limits) |
| Математические функции `<cmath>` | [06_cmath](06_cmath) |
| Потоковый ввод-вывод: `std::cin`, `std::cout`, манипуляторы | [07_stream_io](07_stream_io) |
| Условные конструкции: `if`, init-`if`, `switch` | [08_conditionals](08_conditionals) |
| Циклы: `for`, range-based `for`, `while`, `do-while` | [09_loops](09_loops) |
| `break`, `continue`, оператор «запятая» | [10_break_continue_comma](10_break_continue_comma) |
| Неявные и явные преобразования типов | [11_type_conversions](11_type_conversions) |
