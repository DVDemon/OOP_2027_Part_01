# Лекция 3. Указатели, ссылки, категории значений

## План лекции

1. Модель памяти процесса: стек, куча, статическая память, сегмент кода
2. Адрес (`&`) и разыменование (`*`). Арифметика указателей
3. Указатели и массивы
4. `const`-указатели и указатели на `const`. `nullptr`
5. Динамическая память: `new`/`delete`, `new[]`/`delete[]`
6. Типичные ошибки: dangling pointer, memory leak, double free
7. Санитайзеры (`-fsanitize`)
8. lvalue-ссылки. Указатели vs ссылки. Константные ссылки
9. Range-for со ссылками. Категории значений (lvalue, prvalue, xvalue)
10. Приведение типов: неявное, `static_cast`, `const_cast`, `reinterpret_cast`

---

## 1. Модель памяти процесса

При запуске программы операционная система выделяет процессу виртуальное адресное пространство, разделённое на сегменты:

```
Высокие адреса
┌─────────────────────┐
│       Стек (stack)   │ ← локальные переменные, адреса возврата
│         ↓            │   растёт вниз
│                      │
│         ↑            │
│       Куча (heap)    │ ← динамическая память (new/delete)
├─────────────────────┤
│   Статическая память │ ← глобальные и static переменные
├─────────────────────┤
│   Сегмент кода       │ ← машинные инструкции (read-only)
└─────────────────────┘
Низкие адреса
```

- **Стек** — быстрое выделение/освобождение (LIFO). Размер ограничен (обычно 1-8 МБ).
- **Куча** — гибкий размер, но медленнее. Программист управляет временем жизни.
- **Статическая память** — время жизни = время жизни программы (глобальные, `static`).
- **Сегмент кода** — машинные инструкции, как правило, только для чтения.

```cpp
int global_var = 10;              // статическая память

void foo() {
    int local_var = 20;           // стек
    static int static_var = 30;   // статическая память
    int* heap_var = new int(40);  // указатель на стеке, данные в куче
    delete heap_var;
}
```

> **Основные тезисы:**
> - Время жизни объекта определяется его сегментом: автоматическое (стек, до конца блока), динамическое (куча, до `delete`), статическое (на всё время программы).
> - Стек выделяет и освобождает быстро (LIFO), но его размер ограничен (~1–8 МБ); большие и долгоживущие данные — в кучу.
> - Только куча требует ручного управления временем жизни: здесь возможны утечки, двойное освобождение и dangling pointer.
> - Предпочитайте объекты на стеке — RAII-деструктор уничтожит их автоматически при выходе из области видимости.
> - **Стандарт:** `[basic.stc]`, `[basic.stc.auto]`, `[basic.stc.dynamic]`, `[basic.stc.static]`.
> - **Core Guidelines:** [R.1](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r1) — управляйте ресурсами автоматически (RAII); [R.5](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r5) — предпочитайте scoped-объекты, не выделяйте в куче без необходимости.

---

## 2. Адрес и разыменование. Арифметика указателей

> **Примеры на GitHub:** [01_DeclaringAndUsingPointers](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/01_DeclaringAndUsingPointers)

Указатель — переменная, хранящая адрес другой переменной.

```cpp
#include <iostream>

int main() {
    int x = 42;
    int* ptr = &x;   // ptr хранит адрес x

    std::cout << "Значение x:        " << x     << std::endl;  // 42
    std::cout << "Адрес x:           " << &x    << std::endl;  // 0x7fff...
    std::cout << "Значение ptr:      " << ptr   << std::endl;  // тот же адрес
    std::cout << "Разыменование ptr: " << *ptr  << std::endl;  // 42

    *ptr = 100;  // изменяем значение по адресу
    std::cout << "x после *ptr=100:  " << x     << std::endl;  // 100

    // Размер указателя зависит от платформы, не от типа данных
    std::cout << "sizeof(int*)    = " << sizeof(int*)    << std::endl;  // 8 на 64-bit
    std::cout << "sizeof(double*) = " << sizeof(double*) << std::endl;  // 8 на 64-bit

    return 0;
}
```

### Арифметика указателей

```cpp
int arr[] = {10, 20, 30, 40, 50};
int* p = arr;         // указатель на первый элемент

std::cout << *p       << std::endl;  // 10
std::cout << *(p + 1) << std::endl;  // 20
std::cout << *(p + 2) << std::endl;  // 30

p += 3;               // сдвиг на 3 элемента (3 * sizeof(int) байт)
std::cout << *p       << std::endl;  // 40

// Разность указателей
int* begin = arr;
int* end = arr + 5;
std::ptrdiff_t diff = end - begin;   // 5 (количество элементов)
```

> **Правило:** арифметика указателей допустима только в пределах одного массива (или «один элемент за концом» массива). Иное — неопределённое поведение.

> **Основные тезисы:**
> - `&x` — взятие адреса, `*ptr` — разыменование; размер указателя зависит только от платформы (8 байт на 64-bit), а не от типа данных.
> - Арифметика указателей масштабируется на `sizeof(*p)`: `p + n` — сдвиг на `n` элементов, а не на `n` байт.
> - Сравнивать и вычитать указатели можно только в пределах одного массива (плюс «один элемент за концом»); разность даёт `std::ptrdiff_t`.
> - Выход за границы массива через арифметику указателей — UB; адресные санитайзеры ловят такие ошибки в рантайме.
> - **Стандарт:** `[expr.unary.op]`, `[expr.add]`.
> - **Core Guidelines:** [Bounds.1](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#bounds1) — не используйте арифметику указателей, применяйте `std::span`; [Bounds.2](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#bounds2) — не обращайтесь за пределы диапазона массива.

---

## 3. Указатели и массивы

> **Примеры на GitHub:** [02_PointerArithmeticAndArrays](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/02_PointerArithmeticAndArrays)

Имя массива в большинстве контекстов неявно преобразуется в указатель на первый элемент:

```cpp
int arr[5] = {1, 2, 3, 4, 5};

// Эквивалентные способы доступа
std::cout << arr[2]      << std::endl;  // 3
std::cout << *(arr + 2)  << std::endl;  // 3
std::cout << 2[arr]      << std::endl;  // 3 (да, это работает: a[b] == *(a+b))

// Передача массива в функцию — теряется информация о размере
void print_array(int* data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << data[i] << " ";
    }
}

// Или через шаблон (сохраняет размер)
template <size_t N>
void print_array_ref(int (&data)[N]) {
    for (size_t i = 0; i < N; ++i) {
        std::cout << data[i] << " ";
    }
}
```

> **Основные тезисы:**
> - Имя массива в большинстве выражений неявно «распадается» (decay) в указатель на первый элемент: `arr[i]` — это `*(arr + i)`.
> - При передаче массива в функцию размер теряется; передавайте его отдельно, используйте `std::span` либо ссылку на массив через шаблон `int (&arr)[N]`.
> - В современном C++ вместо C-массивов предпочтительны `std::array` (фиксированный размер) и `std::vector` (динамический размер).
> - **Стандарт:** `[dcl.array]`, `[conv.array]`.
> - **Core Guidelines:** [Bounds.3](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#bounds3) — не используйте C-массивы, применяйте `std::array`/`std::vector`; [Bounds.1](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#bounds1) — для передачи диапазона используйте `std::span`.

---

## 4. `const`-указатели и `nullptr`

> **Примеры на GitHub:** [03_ConstPointers](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/03_ConstPointers)

Четыре комбинации `const` с указателями:

```cpp
int x = 10, y = 20;

// 1. Обычный указатель (можно менять и значение, и адрес)
int* p1 = &x;
*p1 = 100;    // OK
p1 = &y;      // OK

// 2. Указатель на const (нельзя менять значение через указатель)
const int* p2 = &x;   // или: int const* p2 = &x;
// *p2 = 100;          // Ошибка!
p2 = &y;               // OK — сам указатель можно перенаправить

// 3. const-указатель (нельзя перенаправить)
int* const p3 = &x;
*p3 = 100;             // OK — значение можно менять
// p3 = &y;            // Ошибка!

// 4. const-указатель на const (ничего нельзя менять)
const int* const p4 = &x;
// *p4 = 100;          // Ошибка!
// p4 = &y;            // Ошибка!
```

**Мнемоника:** читайте справа налево. `const int* p` — «p это указатель на const int». `int* const p` — «p это const указатель на int».

### `nullptr` (C++11)

```cpp
int* ptr = nullptr;    // C++11: типобезопасный нулевой указатель

// Не используйте NULL или 0 для указателей в C++
// int* p = NULL;      // устарело
// int* p = 0;         // устарело

if (ptr == nullptr) {
    std::cout << "Указатель пуст" << std::endl;
}

// nullptr имеет собственный тип std::nullptr_t
// Это решает проблемы перегрузки:
void foo(int);
void foo(int*);
// foo(NULL);    // неоднозначность! NULL может быть 0 (int)
foo(nullptr);    // однозначно вызывает foo(int*)
```

> **Основные тезисы:**
> - Объявления читайте справа налево: `const int*` — указатель на `const int` (нельзя менять значение), `int* const` — константный указатель на `int` (нельзя переназначить).
> - По умолчанию добавляйте `const` — это документирует намерение, и компилятор проверит его нарушение.
> - `nullptr` (C++11) — типобезопасный нулевой указатель типа `std::nullptr_t`; `NULL` и `0` неоднозначны при перегрузке функций.
> - **Стандарт:** `[dcl.ptr]`, `[lex.nullptr]`.
> - **Core Guidelines:** [ES.47](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es47) — используйте `nullptr`, а не `0` или `NULL`; [ES.25](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es25) — объявляйте объект `const`/`constexpr`, если не планируете менять.

---

## 5. Динамическая память: `new`/`delete`

> **Примеры на GitHub:** [04_DynamicMemory](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/04_DynamicMemory)

```cpp
#include <iostream>

int main() {
    // Выделение одного объекта
    int* p = new int(42);
    std::cout << *p << std::endl;  // 42
    delete p;                       // освобождение

    // Выделение массива
    int* arr = new int[5]{10, 20, 30, 40, 50};
    for (int i = 0; i < 5; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
    delete[] arr;                   // освобождение массива

    // Важно: new/delete и new[]/delete[] — разные операторы!
    // Смешивание — неопределённое поведение:
    // int* bad = new int[10];
    // delete bad;    // UB! Нужно delete[]

    // Выделение с value initialization
    int* zeroed = new int{};       // гарантированно 0
    int* arr_z = new int[10]{};    // все элементы = 0
    delete zeroed;
    delete[] arr_z;

    return 0;
}
```

> **Важно:** в современном C++ прямое использование `new`/`delete` минимизируют. Предпочтительны умные указатели (`std::unique_ptr`, `std::shared_ptr`), которые мы изучим позже.

> **Основные тезисы:**
> - `new`/`delete` и `new[]`/`delete[]` — строго парные; смешивание форм — UB.
> - `new int{}` — value-initialization (обнуление), `new int{42}` — инициализация значением.
> - В современном C++ прямое управление памятью выносят в RAII-обёртки: контейнеры (`std::vector`) и умные указатели.
> - **Стандарт:** `[expr.new]`, `[expr.delete]`, `[basic.stc.dynamic]`.
> - **Core Guidelines:** [R.11](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r11) — избегайте явных `new`/`delete`; [R.20](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r20) — выражайте владение через `unique_ptr`/`shared_ptr`; [ES.61](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es61) — удаляйте массивы через `delete[]`.

---

## 6. Типичные ошибки работы с памятью

> **Примеры на GitHub:** [05_DanglingPointer](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/05_DanglingPointer)

### Dangling pointer (висячий указатель)

```cpp
int* create_value() {
    int local = 42;
    return &local;     // ОШИБКА: local уничтожается при выходе из функции
}

int* p = new int(42);
delete p;
// *p = 10;           // UB: обращение к освобождённой памяти
p = nullptr;           // хорошая практика: обнулить после delete
```

### Memory leak (утечка памяти)

```cpp
void leak() {
    int* p = new int(42);
    // забыли delete p — память утекла!
    // при каждом вызове leak() теряется 4 байта
}

void conditional_leak(bool flag) {
    int* p = new int(42);
    if (flag) return;   // утечка, если flag == true
    delete p;
}
```

### Double free (двойное освобождение)

```cpp
int* p = new int(42);
int* q = p;       // два указателя на одну память
delete p;
// delete q;       // UB: двойное освобождение!
```

> **Основные тезисы:**
> - Dangling pointer — обращение к памяти, время жизни которой закончилось: возврат адреса локальной переменной или использование после `delete`. После `delete` обнуляйте указатель (`p = nullptr`).
> - Memory leak — забытый `delete` (в т.ч. при раннем `return`): память теряется на всё время работы программы.
> - Double free — два независимых `delete` одной памяти: признак того, что у объекта два владельца.
> - Общий корень всех трёх ошибок — ручное управление владением; устраняется RAII и умными указателями.
> - **Стандарт:** `[expr.delete]`.
> - **Core Guidelines:** [ES.65](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es65) — не разыменовывайте невалидный указатель; [R.3](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#r3) — сырой указатель не владеет объектом; [ES.24](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es24) — храните указатели в `unique_ptr` (правило Lifetime).

---

## 7. Санитайзеры

Санитайзеры — инструменты компилятора для обнаружения ошибок в рантайме:

```bash
# AddressSanitizer — утечки, выходы за границы, use-after-free
g++ -std=c++20 -fsanitize=address -g main.cpp -o main

# UndefinedBehaviorSanitizer — undefined behavior
g++ -std=c++20 -fsanitize=undefined -g main.cpp -o main

# Можно комбинировать
g++ -std=c++20 -fsanitize=address,undefined -g main.cpp -o main

# LeakSanitizer (часть ASan на Linux)
g++ -std=c++20 -fsanitize=leak -g main.cpp -o main
```

Пример обнаружения ошибки:

```cpp
// test_asan.cpp
int main() {
    int* arr = new int[10];
    arr[10] = 42;    // выход за границу массива!
    delete[] arr;
    return 0;
}
```

```bash
$ g++ -fsanitize=address -g test_asan.cpp -o test_asan && ./test_asan
=================================================================
==12345==ERROR: AddressSanitizer: heap-buffer-overflow on address ...
WRITE of size 4 at ...
```

> **Рекомендация:** всегда компилируйте с санитайзерами в процессе разработки. Добавьте их в CMake:

```cmake
option(ENABLE_SANITIZERS "Enable ASan+UBSan" OFF)
if(ENABLE_SANITIZERS)
    add_compile_options(-fsanitize=address,undefined -g)
    add_link_options(-fsanitize=address,undefined)
endif()
```

> **Основные тезисы:**
> - AddressSanitizer ловит выход за границы, use-after-free и утечки памяти; UndefinedBehaviorSanitizer — прочие случаи UB; их объединяют флагом `-fsanitize=address,undefined`.
> - Собирайте с `-g` и без агрессивных оптимизаций, чтобы диагностика указывала точные строки.
> - Санитайзеры — инструмент разработки, а не продакшена: включаются опцией сборки (например, в CMake).
> - Пара «санитайзер + юнит-тесты» закрывает большинство ошибок из раздела 6.

---

## 8. lvalue-ссылки

> **Примеры на GitHub:** [08_References](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/08_References)

Ссылка — это альтернативное имя (alias) для существующей переменной:

```cpp
#include <iostream>

int main() {
    int x = 42;
    int& ref = x;    // ref — ссылка на x

    std::cout << "x   = " << x   << std::endl;  // 42
    std::cout << "ref = " << ref << std::endl;   // 42

    ref = 100;   // изменяем x через ссылку
    std::cout << "x   = " << x   << std::endl;  // 100

    // Ссылка и переменная имеют один адрес
    std::cout << "&x   = " << &x   << std::endl;
    std::cout << "&ref = " << &ref << std::endl;  // тот же адрес

    return 0;
}
```

### Свойства ссылок

1. Ссылка **обязана** быть инициализирована при создании.
2. Ссылку **нельзя** переназначить на другой объект.
3. Ссылка **не может** быть `nullptr`.
4. Нет «арифметики ссылок».

### Указатели vs ссылки

| Свойство | Указатель | Ссылка |
|----------|-----------|--------|
| Может быть `nullptr` | Да | Нет |
| Можно переназначить | Да | Нет |
| Требует разыменования | Да (`*ptr`) | Нет |
| Инициализация обязательна | Нет | Да |
| Арифметика | Да | Нет |
| Размер | `sizeof(T*)` | `sizeof(T)` (алиас) |

**Правило:** используйте ссылки, когда можете; указатели — когда нужно (nullable, переназначение, арифметика).

### Константные ссылки

```cpp
int x = 42;
const int& cref = x;    // можно читать, нельзя менять через cref
// cref = 100;           // Ошибка компиляции!

// Константная ссылка может быть привязана к rvalue:
const int& rref = 42;   // продлевает время жизни временного объекта

// Некоторая ссылка — нет:
// int& bad = 42;        // Ошибка! lvalue-ссылка не может быть привязана к rvalue
```

### Range-for со ссылками

```cpp
#include <vector>

std::vector<int> numbers = {1, 2, 3, 4, 5};

// Копия каждого элемента (неэффективно для больших объектов)
for (auto n : numbers) { /* n — копия */ }

// Ссылка (можно модифицировать)
for (auto& n : numbers) {
    n *= 2;  // удваиваем каждый элемент
}

// Константная ссылка (только чтение, без копирования)
for (const auto& n : numbers) {
    std::cout << n << " ";  // 2 4 6 8 10
}
```

> **Основные тезисы:**
> - Ссылка — алиас существующего объекта: обязана быть инициализирована, не переназначается и не может быть `nullptr`.
> - `const T&` связывается и с lvalue, и с rvalue, продлевая жизнь временного объекта; неконстантная `T&` к rvalue не привязывается.
> - Указатель выбирают, когда нужны `nullptr`, переназначение или арифметика; ссылку — когда объект гарантированно существует.
> - Range-for: `auto&` — модификация без копирования, `const auto&` — чтение без копирования, `auto` — копия каждого элемента.
> - **Стандарт:** `[dcl.ref]`, `[dcl.init.ref]`, `[stmt.ranged]`.
> - **Core Guidelines:** [F.60](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f60) — предпочитайте `T*` вместо `T&`, когда «нет аргумента» — валидный вариант; [F.16](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f16) — входные параметры: дешёво копируемые — по значению, остальные — по константной ссылке.

---

## 9. Категории значений

> **Примеры на GitHub:** [16_LvalueAndRValue](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/16_LvalueAndRValue)

В C++ каждое выражение имеет тип и **категорию значения**:

```
          выражение
         /         \
      glvalue      rvalue
      /    \       /    \
   lvalue  xvalue  xvalue  prvalue
```

### lvalue — «left value»

Выражение, обозначающее объект с устойчивым адресом в памяти:

```cpp
int x = 42;     // x — lvalue
int* p = &x;    // можно взять адрес — признак lvalue

std::string s = "hello";
s[0];            // lvalue (элемент строки)
```

### prvalue — «pure rvalue»

Временное значение, не имеющее устойчивого адреса:

```cpp
42;              // prvalue — литерал
x + 1;           // prvalue — результат вычисления
std::string("temp");  // prvalue — временный объект
```

### xvalue — «expiring value»

Объект, ресурсы которого можно «переместить» (подробно — в лекции о move-семантике):

```cpp
std::move(x);    // xvalue — x «готов к перемещению»
```

### Почему это важно

Категории значений определяют, что можно делать с выражением:

```cpp
int x = 10;
int& ref = x;        // OK: lvalue-ссылка к lvalue
// int& bad = 42;     // Ошибка: lvalue-ссылка к prvalue

const int& cref = 42; // OK: const-lvalue-ссылка продлевает жизнь временного
int&& rref = 42;      // OK: rvalue-ссылка к prvalue (C++11, подробнее позже)
```

> **Основные тезисы:**
> - Каждое выражение имеет тип и категорию значения; категория определяет, что с ним можно делать.
> - lvalue — объект с устойчивым адресом; prvalue — временное значение (литерал, результат выражения); xvalue — объект, «готовый к перемещению» (`std::move`).
> - lvalue-ссылка привязывается только к lvalue; `const T&` и `T&&` — к rvalue; `const T&` продлевает жизнь временного объекта.
> - Категории значений — фундамент перегрузки функций и move-семантики.
> - **Стандарт:** `[basic.lval]`, `[dcl.init.ref]`.
> - **Core Guidelines:** [F.18](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#f18) — для параметров «будет перемещён» используйте `X&&` и `std::move`.

---

## 10. Приведение типов

### Неявные преобразования

```cpp
int i = 42;
double d = i;          // int → double (расширение)
int j = 3.14;          // double → int (сужение, warning)
// int k{3.14};        // Ошибка при list-initialization!

// Pointer conversions
int* p = nullptr;
void* vp = p;          // любой указатель → void* (неявно)
// int* p2 = vp;       // void* → int* — ОШИБКА в C++ (в C — OK)
```

### `static_cast` — безопасное явное приведение

> **Примеры на GitHub:** [14_StaticCast](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/14_StaticCast)

```cpp
double pi = 3.14159;
int truncated = static_cast<int>(pi);  // 3 — явное и документированное

// Преобразование void* → T*
void* vp = &pi;
double* dp = static_cast<double*>(vp);

// Преобразование между связанными типами в иерархии классов
// (подробнее при изучении наследования)
```

### `const_cast` — снятие/добавление `const`

> **Примеры на GitHub:** [15_ConstCast](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/15_ConstCast)

```cpp
const int x = 42;
const int* cp = &x;

int* p = const_cast<int*>(cp);  // снимаем const
// *p = 100;  // UB! Объект x изначально const

// Допустимый случай: функция принимает non-const, но не модифицирует
void legacy_print(char* str);   // C-legacy функция
const char* msg = "hello";
legacy_print(const_cast<char*>(msg));  // OK, если legacy_print не пишет
```

> **Правило:** `const_cast` нужен крайне редко. Если вы его используете — скорее всего, в коде есть архитектурная проблема.

### `reinterpret_cast` — побитовая реинтерпретация

```cpp
int x = 42;
// Переинтерпретация битового представления
int* ip = &x;
char* cp = reinterpret_cast<char*>(ip);

// Просмотр байтов int
for (size_t i = 0; i < sizeof(int); ++i) {
    std::cout << static_cast<int>(cp[i]) << " ";
}
```

> **Правило:** `reinterpret_cast` — самый опасный. Используйте только для низкоуровневого кода (сериализация, работа с hardware).

### Сводка операторов приведения

| Оператор | Назначение | Безопасность |
|----------|------------|-------------|
| `static_cast` | Явные безопасные преобразования | Высокая |
| `const_cast` | Снятие/добавление `const` | Средняя |
| `reinterpret_cast` | Побитовая реинтерпретация | Низкая |
| `dynamic_cast` | Безопасное приведение в иерархии (позже) | Высокая |
| `(type)expr` | C-style (избегать!) | Непредсказуемая |

> **Основные тезисы:**
> - Неявное сужение (narrowing) — предупреждение компилятора; при list-initialization `{}` — ошибка компиляции.
> - `static_cast` — явное, документированное преобразование; предпочитайте его неявному сужению.
> - `const_cast` нужен крайне редко; запись через него в изначально `const`-объект — UB.
> - `reinterpret_cast` — побитовая переинтерпретация; только для низкоуровневого кода (сериализация, работа с железом).
> - C-style приведение `(T)expr` может выполнить любой из `static`/`const`/`reinterpret`/`dynamic` — его применение непрозрачно и опасно.
> - **Стандарт:** `[expr.static.cast]`, `[expr.const.cast]`, `[expr.reinterpret.cast]`, `[expr.cast]`, `[dcl.init.list]`.
> - **Core Guidelines:** [ES.48](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es48) — избегайте приведений; [ES.49](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es49) — если необходимо, используйте именованный `*_cast`; [ES.50](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es50) — не снимайте `const` со значения, объявленного как `const`.

---

## Ключевые концепции

1. **Указатель** хранит адрес. `&` — взятие адреса, `*` — разыменование.
2. **Арифметика указателей** учитывает размер типа: `p + 1` сдвигает на `sizeof(*p)` байт.
3. **`nullptr`** — типобезопасная замена `NULL` и `0` для указателей.
4. **`new`/`delete`** и **`new[]`/`delete[]`** — парные операторы. Смешивание — UB.
5. **Ссылка** — алиас для существующего объекта. Не может быть `nullptr`, не переназначается.
6. **`const&`** продлевает жизнь временного объекта и позволяет принимать как lvalue, так и rvalue.
7. **Категории значений:** lvalue (имеет адрес), prvalue (временное), xvalue (готово к перемещению).
8. **Санитайзеры** (ASan, UBSan) — обязательный инструмент при разработке.

---

## Распространённые ошибки

### 1. Возврат адреса локальной переменной

```cpp
int* bad() {
    int x = 42;
    return &x;   // x уничтожается — dangling pointer!
}
```

### 2. Забытый `delete` / `delete[]`

```cpp
void process() {
    int* data = new int[1000];
    if (error) return;    // утечка!
    delete[] data;
}
// Решение: используйте std::vector или std::unique_ptr
```

### 3. Смешивание `delete` и `delete[]`

```cpp
int* arr = new int[10];
delete arr;    // UB! Нужно delete[]
```

### 4. Использование указателя после `delete`

```cpp
int* p = new int(42);
delete p;
std::cout << *p;  // UB! Use-after-free
// Совет: p = nullptr; после delete
```

### 5. Путаница `const int*` и `int* const`

```cpp
const int* p;    // можно перенаправить, нельзя менять значение
int* const q = &x;  // нельзя перенаправить, можно менять значение
// Мнемоника: читайте справа налево
```

---

## Вопросы для самопроверки

1. Чем стек отличается от кучи? Когда использовать каждый?
2. Что хранит указатель? Чему равен `sizeof(int*)` на 64-битной платформе?
3. Что произойдёт при выходе за границы массива через указатель?
4. В чём разница между `const int*` и `int* const`?
5. Почему `nullptr` лучше, чем `NULL`?
6. Назовите три типичные ошибки при работе с динамической памятью.
7. Чем ссылка отличается от указателя?
8. Почему `const int& ref = 42;` допустимо, а `int& ref = 42;` — нет?
9. Что такое lvalue и prvalue? Приведите по два примера каждого.
10. В чём разница между `static_cast` и `reinterpret_cast`?

---

## Практическое задание

### Задание: «Динамический массив вручную»

Реализуйте набор функций для работы с динамическим массивом целых чисел **без использования `std::vector`**:

```cpp
// Создать массив заданного размера, заполненный нулями
int* create_array(size_t size);

// Изменить размер массива (аналог realloc), сохранив данные
int* resize_array(int* old_array, size_t old_size, size_t new_size);

// Вставить элемент в позицию pos, сдвинув остальные вправо
// Массив увеличивается на 1
int* insert_at(int* array, size_t& size, size_t pos, int value);

// Удалить элемент из позиции pos, сдвинув остальные влево
// Массив уменьшается на 1
int* remove_at(int* array, size_t& size, size_t pos);

// Вывести массив
void print_array(const int* array, size_t size);

// Освободить память
void destroy_array(int* array);
```

**Требования:**
1. Скомпилируйте с `-fsanitize=address,undefined` — ни одной ошибки.
2. Напишите тесты (Google Test), проверяющие каждую функцию.
3. Убедитесь, что `destroy_array` вызывается для каждого созданного массива.
4. Подумайте: какие проблемы у этого подхода по сравнению с `std::vector`?

---

## Ссылки на примеры

| Тема | Пример |
|------|--------|
| Объявление указателей | [01_DeclaringAndUsingPointers](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/01_DeclaringAndUsingPointers) |
| Арифметика указателей | [02_PointerArithmeticAndArrays](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/02_PointerArithmeticAndArrays) |
| const-указатели | [03_ConstPointers](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/03_ConstPointers) |
| Динамическая память | [04_DynamicMemory](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/04_DynamicMemory) |
| Dangling pointer | [05_DanglingPointer](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/05_DanglingPointer) |
| Ссылки | [08_References](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/08_References) |
| static_cast | [14_StaticCast](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/14_StaticCast) |
| const_cast | [15_ConstCast](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/15_ConstCast) |
| lvalue и rvalue | [16_LvalueAndRValue](https://github.com/DVDemon/mai_oop_examples_public/tree/main/examples/lection02_03/16_LvalueAndRValue) |
