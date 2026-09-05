# 03. Делегирующие конструкторы

**Пункт плана:** 3 — делегирующие конструкторы (C++11).

## Что показывает пример

* Один «полный» конструктор `Connection(host, port, ssl)` содержит всю логику.
* Остальные конструкторы делегируют ему работу, подставляя значения по
  умолчанию: `Connection(host, port)`, `Connection(host)`, `Connection()`.
* Цепочка делегирования: `Connection() -> Connection(host) -> ... ->`
  полный конструктор.

## Запуск

```bash
./build/03_delegating_constructors
```

## Плохо → Хорошо

**Плохо** — копировать одинаковую инициализацию и логику в каждый конструктор:

```cpp
Connection(const std::string& host, int port, bool ssl)
    : host_(host), port_(port), ssl_(ssl) { /* лог */ }

Connection(const std::string& host, int port)
    : host_(host), port_(port), ssl_(false) { /* тот же лог — дубль */ }

Connection(const std::string& host)
    : host_(host), port_(80), ssl_(false) { /* снова дубль */ }
```

**Хорошо** — делегировать одному конструктору:

```cpp
Connection(const std::string& host, int port)
    : Connection(host, port, false) {}

Connection(const std::string& host)
    : Connection(host, 80, false) {}
```

**Почему:** дублирование инициализации и логики легко рассинхронизировать при
правках. Делегирование оставляет единственную точку истины — изменение
поведения в одном месте автоматически отражается во всех конструкторах.

> При делегировании в списке инициализации можно указать **только** вызов
> другого конструктора — нельзя одновременно инициализировать поля.
