# 05. SRP — принцип единственной ответственности

**Пункт плана:** 5 — SRP (прогрессивный пример в 4 этапа).

> «У класса должна быть только одна причина для изменения» — Роберт Мартин.

## Что показывает пример

Декомпозиция God Object `Employee` на классы с единственной ответственностью.
`main.cpp` содержит финальный (этап 4) вариант:

* `Employee` — доменная модель (бизнес-логика и данные);
* `EmployeeRepository` — хранение;
* `EmployeeReportGenerator` — формирование отчёта;
* `EmployeeView` — вывод на экран.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/05_srp
```

## Плохо → Хорошо (4 этапа)

**Этап 1 (плохо) — God Object:**

```cpp
class Employee {
    std::string name_;  double salary_;
public:
    double calculate_pay() const;          // бизнес-логика
    void   save_to_db(Database&);          // персистентность
    std::string generate_report() const;   // отчётность
    void   display(Screen&);               // UI
};
```

**Почему плохо:** четыре несвязанные обязанности (логика, БД, отчёты, UI) —
четыре причины для изменения. Правка формата отчёта рискует задеть бизнес-
логику; класс невозможно переиспользовать частично и тяжело тестировать.

**Этап 2** — выносим хранение в `EmployeeRepository`.
**Этап 3** — выносим отчёт в `EmployeeReportGenerator`.
**Этап 4 (хорошо)** — финальная декомпозиция:

```cpp
class Employee { /* только данные и calculate_pay() */ };
class EmployeeRepository { /* save / load */ };
class EmployeeReportGenerator { /* generate */ };
class EmployeeView { /* display */ };
```

**Почему хорошо:** у каждого класса ровно одна причина для изменения.
Поменялся формат отчёта — трогаем только `EmployeeReportGenerator`; сменили
БД — только `EmployeeRepository`. SRP — основа для ISP и DIP.
