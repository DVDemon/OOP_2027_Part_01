# Лекция 2. Операторы, ввод-вывод, управляющие конструкции

Примеры к лекции. Покрывают все пункты плана: арифметические, побитовые,
логические операторы и операторы сравнения; приоритет/ассоциативность и
инкремент; составные присваивания и `std::numeric_limits`; математические
функции `<cmath>`; потоковый ввод-вывод и манипуляторы; условные конструкции
`if`/`switch`; циклы; `break`/`continue` и оператор «запятая»; неявные и явные
преобразования типов (promotion, usual arithmetic conversions, `static_cast`).

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск примеров:

```bash
./build/01_arithmetic
./build/02_bitwise
./build/03_logical_comparison
./build/04_precedence_increment
./build/05_compound_numeric_limits
./build/06_cmath
./build/07_stream_io
./build/08_conditionals
./build/09_loops
./build/10_break_continue_comma
./build/11_type_conversions
```

## Примеры

| Папка | Пункт плана | О чём |
|-------|-------------|-------|
| [`01_arithmetic`](01_arithmetic) | 1 | Арифметика, целочисленное деление, остаток, защита от деления на ноль |
| [`02_bitwise`](02_bitwise) | 2 | Побитовые операторы, битовые флаги, ловушка приоритета `&` и `==` |
| [`03_logical_comparison`](03_logical_comparison) | 3 | Логические операторы, сравнения, short-circuit evaluation |
| [`04_precedence_increment`](04_precedence_increment) | 4 | Приоритет и ассоциативность; префиксный/постфиксный инкремент |
| [`05_compound_numeric_limits`](05_compound_numeric_limits) | 5 | Составные операторы присваивания; `std::numeric_limits`; переполнение |
| [`06_cmath`](06_cmath) | 6 | Функции `<cmath>`, `std::numbers`, сравнение `double` с допуском |
| [`07_stream_io`](07_stream_io) | 7 | `std::cin`/`std::cout`, манипуляторы, проверка ввода, `>>` vs `getline` |
| [`08_conditionals`](08_conditionals) | 8 | `if`, init-`if` (C++17), `switch`, `[[fallthrough]]` |
| [`09_loops`](09_loops) | 9 | `for`, range-based `for`, `while`, `do-while`, вложенные циклы |
| [`10_break_continue_comma`](10_break_continue_comma) | 10 | `break`, `continue`, оператор «запятая» |
| [`11_type_conversions`](11_type_conversions) | 11 | Неявные преобразования (promotion, usual arithmetic conversions, ловушки), `static_cast`, `{}`-инициализация против сужений |

## Ключевые идеи лекции

1. **Целочисленное деление** отбрасывает дробную часть: `17 / 5 == 3`. Деление на ноль — UB.
2. **Побитовые операции** — только над беззнаковыми типами; `&` имеет приоритет ниже, чем `==`.
3. **Short-circuit evaluation** в `&&`/`||` — правый операнд может не вычисляться (важно для безопасности).
4. **Приоритет и ассоциативность**: при сомнениях ставьте скобки; `++it` предпочтительнее `it++`.
5. **`std::numeric_limits`** — типобезопасная замена макросам `INT_MAX`/`DBL_MIN`; следите за переполнением signed.
6. **`double` нельзя сравнивать через `==`** — используйте сравнение с допуском (epsilon).
7. **Манипуляторы потоков** управляют форматом; всегда проверяйте результат `>>`.
8. **init-`if`/init-`switch`** (C++17) ограничивают область видимости вспомогательных переменных.
9. **Range-based `for` с `const auto&`** — идиоматический обход контейнера без копирования.
10. **`break`** выходит только из ближайшего цикла; **оператор «запятая»** уместен в `for`.
11. **Неявные сужающие преобразования** теряют данные незаметно — используйте `{}`-инициализацию и `static_cast`.
