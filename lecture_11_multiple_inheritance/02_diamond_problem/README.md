# 02. Проблема ромба (diamond problem)

**Пункт плана:** 2 — проблема ромба.

## Что показывает пример

* Иерархию-ромб: `Device` → `Printer`, `Scanner` → `Copier`.
* Что при обычном (не виртуальном) наследовании `Copier` содержит **две**
  независимые копии `Device`.
* Неоднозначность при обращении к `c.id` и обход через явную квалификацию.

```
        Device          Device
          |               |
       Printer          Scanner
          \              /
             Copier        <- содержит ДВЕ копии Device
```

## Запуск

```bash
./build/02_diamond_problem
```

Ожидаемый вывод (адреса различаются от запуска к запуску):

```
Device(1)
Device(101)
c.Printer::id = 1
c.Scanner::id = 101
&c.Printer::id = 0x...
&c.Scanner::id = 0x...   <- другой адрес: это РАЗНЫЕ объекты
Printer id=1
doc from scanner id=101
```

## Плохо → Хорошо

**Плохо** — обычное наследование от общего предка и «лечение» симптомов
квалификацией имён:

```cpp
class Copier : public Printer, public Scanner { /* ... */ };

c.id;          // ОШИБКА: неоднозначно
c.Printer::id; // приходится выбирать копию вручную — а какая «настоящая»?
```

**Почему плохо:** два конструктора `Device` вызываются дважды, данные
дублируются и могут рассинхронизироваться (`id` равен то 1, то 101).
Логически `Copier` — одно устройство с одним `id`, а в памяти их два.

**Хорошо** — виртуальное наследование, дающее единственную копию `Device`
(см. пример [`03_virtual_inheritance`](../03_virtual_inheritance)):

```cpp
class Printer : virtual public Device { /* ... */ };
class Scanner : virtual public Device { /* ... */ };
class Copier  : public Printer, public Scanner { /* ... */ };

c.id;  // OK: единственная копия, неоднозначности нет
```
