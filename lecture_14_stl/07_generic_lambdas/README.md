# 07. Шаблонные (generic) лямбды

**Блок плана:** 7 — шаблонные лямбды и функции, принимающие лямбду.

## Что показывает пример

* **`[](auto x)`** — generic-лямбда: компилятор генерирует отдельный шаблонный
  `operator()<T>` под каждый тип. Одна лямбда работает с `int`, `double`,
  `std::string`.
* **C++20-синтаксис** `[]<class T>(const T& a, const T& b)` — явный список
  параметров шаблона.
* **Generic-компаратор** для `std::sort`: `[](const auto& a, const auto& b)`.
* **Шаблонная функция** `template<class F> auto apply_twice(F f, int x)` принимает
  любую лямбду; шаблон `transformed` применяет лямбду к контейнеру через
  `std::transform`.
* **`std::function`** стирает тип: в `vector<std::function<int(int)>>` можно
  хранить разные лямбды (в том числе с захватом).
* **Цена `std::function`:** `sizeof(function)` заметно больше `sizeof(lambda)`;
  для горячего кода лучше `auto`/шаблон, для хранения — `std::function`.

## Сборка и запуск

```bash
cmake -S .. -B ../build
cmake --build ../build
./../build/07_generic_lambdas
```

## Плохо → Хорошо

**Плохо** — копировать лямбду под каждый тип:

```cpp
auto twice_int    = [](int x)    { return x + x; };
auto twice_double = [](double x) { return x + x; };
auto twice_string = [](std::string x) { return x + x; };
```

**Почему плохо:** одна и та же логика дублируется; при добавлении типа нужно
не забыть ещё одну копию, а исправление приходится вносить во все версии.

**Хорошо** — generic-лямбда:

```cpp
auto twice = [](auto x) { return x + x; };   // operator()<T> для каждого T
```

**Почему хорошо:** тело одно; компилятор сам инстанцирует `operator()<int>`,
`operator()<double>`, `operator()<std::string>`. Ограничения типа при этом остаются:
если у `T` нет `operator+`, ошибка возникнет при инстанцировании, а не в рантайме.

## Фрагмент вывода

```
=== 1. Generic-лямбда: один код — много типов ===
twice(21)          = 42
twice(1.5)         = 3
twice(string("ab")) = abab
для каждого типа инстанцируется свой operator()<T>

=== 4. Шаблонная функция принимает лямбду ===
apply_twice(+10, 1) = 21
apply_twice(twice, 3) = 12

=== 7. Цена std::function ===
sizeof(lambda)        = 1
sizeof(std::function) = 32
```

`sizeof(std::function)` зависит от реализации; на большинстве платформ это
32–64 байта против 1 байта у лямбды без захвата.
