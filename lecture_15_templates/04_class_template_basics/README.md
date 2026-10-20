# 04. Базовый шаблон класса

**Пункт плана:** 7 — шаблон класса.

## Что показывает пример

* Синтаксис `template<typename T> class`.
* Хранение значения любого типа.
* Методы get/set.
* Вложенные шаблоны (`Box<Box<int>>`).

## Запуск

```bash
./build/01_basic_class_template
```

## Плохо → Хорошо

**Плохо** — отдельный класс для каждого типа:

```cpp
class IntBox { int v; /* ... */ };
class DoubleBox { double v; /* ... */ };
```

**Хорошо** — один шаблон для всех типов:

```cpp
template <typename T>
class Box { T v; /* ... */ };
```
