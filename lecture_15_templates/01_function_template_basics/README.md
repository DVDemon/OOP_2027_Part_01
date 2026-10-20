# 01. Базовый шаблон функции

**Пункт плана:** 1 — базовый синтаксис.

## Что показывает пример

* Синтаксис `template<typename T>`.
* Шаблон `max` для int, double, string.
* Шаблон `swap`.
* Явное указание типа шаблона.

## Запуск

```bash
./build/01_basic_template
```

## Плохо → Хорошо

**Плохо** — копипаст для каждого типа:

```cpp
int max_int(int a, int b);
double max_double(double a, double b);
```

**Хорошо** — один шаблон для всех типов:

```cpp
template <typename T>
T max(T a, T b) { return (a > b) ? a : b; }
```
