# 07. Примеси (mixins) через CRTP

**Пункт плана:** 7 — примеси (mixins).

## Что показывает пример

* Добавление функциональности через множественное наследование от шаблонных
  примесей `Printable<T>` и `Comparable<T>`.
* Безопасный вид MI: примеси **не содержат данных** и не образуют ромб.
* Паттерн CRTP (Curiously Recurring Template Pattern) — статический полиморфизм
  без виртуальных вызовов (нулевые рантайм-затраты).
* Достаточно реализовать `toString()`, `==`, `<` — методы `print`, `!=`, `>`,
  `<=`, `>=` примеси дают сами.

## Состав примера

* `mixins.h` — шаблонные примеси `Printable<T>`, `Comparable<T>`;
* `student.h` — класс `Student`, подмешивающий обе примеси;
* `main.cpp` — демонстрация в консоли (цель `07_mixins_demo`).

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/07_mixins_demo
```

## Плохо → Хорошо

**Плохо** — копировать однотипные операторы сравнения в каждый класс:

```cpp
class Student {
    bool operator==(const Student&) const;
    bool operator<(const Student&) const;
    bool operator!=(const Student& o) const { return !(*this == o); }  // copy-paste
    bool operator>(const Student& o) const  { return o < *this; }      // copy-paste
    bool operator<=(const Student& o) const { return !(*this > o); }   // copy-paste
    bool operator>=(const Student& o) const { return !(*this < o); }   // copy-paste
};
// то же самое придётся повторить в Teacher, Course, ...
```

**Хорошо** — вынести производные операторы в примесь и подмешать её:

```cpp
class Student : public Comparable<Student> {
    bool operator==(const Student&) const;  // только базовые два оператора
    bool operator<(const Student&) const;
};
// !=, >, <=, >= получены автоматически и переиспользуются любым классом
```

**Почему:** CRTP-примесь выражает производную логику **один раз** и через
`static_cast<const T&>(*this)` обращается к конкретному типу без виртуальных
вызовов. Это устраняет дублирование, не добавляет рантайм-накладных расходов и,
в отличие от классического MI с данными, не порождает проблему ромба, так как
примеси не хранят состояния.
