# Лекция 3. Указатели, ссылки, категории значений

Примеры к лекции. Покрывают все пункты плана: модель памяти процесса, адрес и
разыменование, арифметику указателей, связь указателей и массивов, `const`-указатели
и `nullptr`, динамическую память `new`/`delete`, типичные ошибки памяти, санитайзеры,
lvalue-ссылки, категории значений и операторы приведения типов.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Сборка с санитайзерами (AddressSanitizer + UBSan):

```bash
cmake -S . -B build -DENABLE_SANITIZERS=ON
cmake --build build
```

Запуск примеров:

```bash
./build/01_memory_model
./build/02_address_deref
./build/03_pointers_arrays
./build/04_const_pointers_nullptr
./build/05_dynamic_memory
./build/06_memory_errors
./build/07_sanitizers
./build/08_references
./build/09_value_categories
./build/10_type_casts
```

## Примеры

| Папка | Пункт плана | О чём |
|-------|-------------|-------|
| [`01_memory_model`](01_memory_model) | 1 | Сегменты памяти: стек, куча, статическая, код |
| [`02_address_deref`](02_address_deref) | 2 | `&` и `*`, арифметика указателей |
| [`03_pointers_arrays`](03_pointers_arrays) | 3 | Указатели и массивы, array-to-pointer decay |
| [`04_const_pointers_nullptr`](04_const_pointers_nullptr) | 4 | Четыре комбинации `const`, `nullptr` |
| [`05_dynamic_memory`](05_dynamic_memory) | 5 | `new`/`delete`, `new[]`/`delete[]`, ручной массив |
| [`06_memory_errors`](06_memory_errors) | 6 | Dangling pointer, leak, double free (безопасно) |
| [`07_sanitizers`](07_sanitizers) | 7 | `-fsanitize=address,undefined` |
| [`08_references`](08_references) | 8 | lvalue-ссылки, указатели vs ссылки, `const&` |
| [`09_value_categories`](09_value_categories) | 9 | Range-for со ссылками, lvalue/prvalue/xvalue |
| [`10_type_casts`](10_type_casts) | 10 | Неявные, `static_cast`, `const_cast`, `reinterpret_cast` |

## Ключевые идеи лекции

1. **Указатель** хранит адрес. `&` — взятие адреса, `*` — разыменование.
2. **Арифметика указателей** учитывает размер типа: `p + 1` сдвигает на `sizeof(*p)` байт; допустима только в пределах массива.
3. **`nullptr`** — типобезопасная замена `NULL` и `0` для указателей.
4. **`new`/`delete`** и **`new[]`/`delete[]`** — парные операторы. Смешивание — UB.
5. **Ошибки памяти** (dangling pointer, leak, double free) — следствие ручного владения; решение — RAII и умные указатели.
6. **Санитайзеры** (ASan, UBSan) превращают тихое UB в явный отчёт — обязательный инструмент разработки.
7. **Ссылка** — псевдоним для существующего объекта. Не `nullptr`, не переназначается, без арифметики.
8. **`const&`** не копирует объект и продлевает жизнь временного; принимает и lvalue, и rvalue.
9. **Категории значений:** lvalue (имеет адрес), prvalue (временное), xvalue (готово к перемещению).
10. **Именованные приведения** (`static_cast`/`const_cast`/`reinterpret_cast`) предпочтительнее C-style: явное намерение и контроль компилятора.
