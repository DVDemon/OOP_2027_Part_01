# 01. Связанность (Coupling) и связность (Cohesion)

**Пункт плана:** 1 — связанность и связность.

## Что показывает пример

* **Coupling** — степень зависимости одного модуля от другого. Высокая
  связанность плоха: изменение в одном классе каскадно ломает другие.
* **Cohesion** — степень, в которой элементы внутри модуля относятся к одной
  задаче. Высокая связность хороша: класс делает одно дело.
* Цель проектирования: **low coupling, high cohesion**.
* Здесь `Report` зависит только от абстракций `IDataSource` и `IFormatter`,
  а конкретные реализации подставляются через конструктор.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/01_coupling_cohesion
```

## Плохо → Хорошо

**Плохо** — `Report` содержит конкретные `Database` и `PdfFormatter`:

```cpp
class Report {
    Database db_;           // прямая зависимость от конкретной БД
    PdfFormatter fmt_;      // прямая зависимость от конкретного формата
public:
    void generate() {
        auto data = db_.query("SELECT * FROM sales");
        fmt_.render(data);
    }
};
```

**Почему плохо:** `Report` жёстко связан и с `Database`, и с `PdfFormatter`.
Любое изменение в этих классах затронет `Report`; подставить другую БД или
другой формат (например, для теста) невозможно.

**Хорошо** — зависимость от абстракций:

```cpp
class Report {
    const IDataSource& source_;
    const IFormatter& formatter_;
public:
    Report(const IDataSource& src, const IFormatter& fmt)
        : source_(src), formatter_(fmt) {}
    void generate() const {
        auto data = source_.fetch();
        formatter_.render(data);
    }
};
```

**Почему хорошо:** `Report` зависит только от контрактов. Можно подставить
любой источник данных и любой формат, не трогая код `Report`. Связанность
снизилась, связность осталась высокой — класс по-прежнему делает одно дело.
