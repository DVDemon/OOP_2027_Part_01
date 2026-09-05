# Лекция 1. Введение в C++. Инструменты разработки

Примеры к лекции. Покрывают все пункты плана: структуру программы и `main`,
этапы трансляции и ODR, систему сборки CMake, Google Test, базовые типы,
формы инициализации, `const`/`constexpr`/`auto` и пространства имён.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск примеров:

```bash
./build/01_first_program "arg one" "arg two"
./build/04_basic_types
./build/05_initialization
./build/06_const_constexpr
./build/07_auto
./build/08_namespaces
./build/02_translation_units
./build/03_gtest_calculator_demo
```

Запуск тестов:

```bash
cd build && ctest --output-on-failure
```

> Если нет доступа в интернет (GoogleTest подтягивается через FetchContent),
> отключите тестовый пример:
> `cmake -S . -B build -DLECTURE_BUILD_TESTS=OFF`

## Примеры

| Папка | Пункт плана | О чём |
|-------|-------------|-------|
| [`01_first_program`](01_first_program) | 3 | Структура программы, точка входа `main`, `argc`/`argv` |
| [`02_translation_units`](02_translation_units) | 4, 5 | Этапы трансляции, единицы трансляции, ODR, include guard |
| [`03_gtest_calculator`](03_gtest_calculator) | 6, 7 | CMake + Google Test: проект «Калькулятор» с тестами |
| [`04_basic_types`](04_basic_types) | 8 | Базовые типы, литералы, `sizeof`, типы фиксированного размера |
| [`05_initialization`](05_initialization) | 9 | Copy/direct/list/value-инициализация, сужение, most vexing parse |
| [`06_const_constexpr`](06_const_constexpr) | 10 | `const` vs `constexpr`, `static_assert`, макросы — это плохо |
| [`07_auto`](07_auto) | 10 | Вывод типа `auto`: где помогает, где вредит |
| [`08_namespaces`](08_namespaces) | 10 | Пространства имён, `using`-объявление vs `using`-директива |

## Ключевые идеи лекции

1. **C++ — мультипарадигменный язык.** Поддерживает процедурное, ООП, обобщённое и функциональное программирование.
2. **Этапы сборки:** препроцессор → компилятор → линковщик. Каждый `.cpp` — отдельная единица трансляции.
3. **ODR** — правило одного определения; нарушение → неопределённое поведение.
4. **Фигурная инициализация `{}`** защищает от сужения и от most vexing parse.
5. **`const`** — неизменяемость в рантайме, **`constexpr`** — вычислимость на этапе компиляции.
6. **CMake** — стандарт де-факто для сборки C++-проектов.
