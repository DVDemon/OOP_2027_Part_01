# 04. Операторы сравнения и `operator<=>` (C++20)

**Пункт плана:** 4.

## Что показывает пример

* `Date` с `auto operator<=>(const Date&) const = default;` — одна строка
  заменяет шесть операторов (`<`, `<=`, `>`, `>=`, `==`, `!=`).
* `CaseInsensitiveString` с ПОЛЬЗОВАТЕЛЬСКИМ `<=>`, возвращающим
  `std::strong_ordering`.
* Почему при пользовательском `<=>` нужно отдельно определить `==`.

## Запуск

```bash
./build/04_comparison_spaceship
```

## Плохо → Хорошо

**Плохо** (до C++20) — вручную писать все шесть операторов:

```cpp
bool operator==(const Date& o) const { /* ... */ }
bool operator!=(const Date& o) const { return !(*this == o); }
bool operator<(const Date& o)  const { /* ... */ }
bool operator>(const Date& o)  const { return o < *this; }
bool operator<=(const Date& o) const { return !(o < *this); }
bool operator>=(const Date& o) const { return !(*this < o); }
```

ПОЧЕМУ плохо: шесть функций, легко ошибиться в одной (перепутать знак),
нарушить согласованность (`a < b`, но `a <= b` врёт).

**Хорошо** (C++20) — один трёхпозиционный оператор:

```cpp
auto operator<=>(const Date&) const = default;
```

ПОЧЕМУ хорошо: компилятор выводит все сравнения из одного источника —
противоречий быть не может. Поля сравниваются лексикографически в порядке
объявления.

> Для нестандартной логики пишем `<=>` вручную и возвращаем тип упорядочения
> (`std::strong_ordering` / `weak_ordering` / `partial_ordering`). В этом
> случае `operator==` нужно определить отдельно — компилятор его НЕ выводит
> из пользовательского `<=>`.
