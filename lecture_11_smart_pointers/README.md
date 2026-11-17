# Лекция 11. Умные указатели

Примеры к лекции. Покрывают все пункты плана: мотивацию RAII, `std::unique_ptr`,
`std::shared_ptr`, `std::weak_ptr` и дедлоки на разделяемых ресурсах,
`enable_shared_from_this`, приведение типов для умных указателей, учебные
реализации `UniquePtr`/`SharedPtr`, работу с полиморфизмом.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск примеров:

```bash
./build/01_motivation
./build/02_unique_ptr
./build/03_shared_ptr
./build/04_weak_ptr
./build/05_shared_ptr_cycle
./build/06_enable_shared_from_this
./build/07_pointer_cast
./build/08_custom_unique_ptr
./build/09_custom_shared_ptr
./build/10_polymorphism
```

## Примеры

| Папка | Пункт плана | О чём |
|-------|-------------|-------|
| [`01_motivation`](01_motivation) | 1 | Ручной `new`/`delete` + исключения = утечки; RAII-решение |
| [`02_unique_ptr`](02_unique_ptr) | 2 | Эксклюзивное владение, `make_unique`, перемещение, пользовательский deleter |
| [`03_shared_ptr`](03_shared_ptr) | 3 | Разделяемое владение, `use_count`, `make_shared` vs конструктор |
| [`04_weak_ptr`](04_weak_ptr) | 4 | Циклические ссылки и их разрыв через `weak_ptr` |
| [`05_shared_ptr_cycle`](05_shared_ptr_cycle) | 5 | Цикл из двух `shared_ptr`: объекты не удаляются — утечка; обход через `weak_ptr` |
| [`06_enable_shared_from_this`](06_enable_shared_from_this) | 6 | Безопасный `shared_ptr` на самого себя |
| [`07_pointer_cast`](07_pointer_cast) | 7 | `dynamic_pointer_cast` / `static_pointer_cast` |
| [`08_custom_unique_ptr`](08_custom_unique_ptr) | 8 | Учебная реализация `UniquePtr` |
| [`09_custom_shared_ptr`](09_custom_shared_ptr) | 9 | Учебная реализация `SharedPtr` |
| [`10_polymorphism`](10_polymorphism) | 10 | Умные указатели в полиморфных иерархиях, фабрика |

## Ключевые идеи лекции

1. **`new`/`delete` в пользовательском коде — анахронизм.** Владение ресурсом выражают умными указателями (RAII): освобождение происходит в деструкторе автоматически, даже при исключениях.
2. **`unique_ptr` — выбор по умолчанию.** Эксклюзивное владение, нулевые накладные расходы, копирование запрещено, передача владения — только `std::move`.
3. **`shared_ptr` — разделяемое владение** через подсчёт ссылок. `make_shared` эффективнее (одна аллокация) и безопаснее конструктора от сырого указателя.
4. **`weak_ptr` разрывает циклы.** `shared_ptr` в обе стороны цикла → утечка; одна из связей должна быть `weak_ptr`.
5. **Цикл из двух `shared_ptr` — утечка.** Если объекты владеют друг другом, счётчики не падают до нуля: `reset` внешних ссылок не освобождает память. Обратную связь делают `weak_ptr`.
6. **`enable_shared_from_this`** даёт объекту корректный `shared_ptr` на себя; нельзя звать из конструктора и при отсутствии управляющего `shared_ptr`.
7. **`make_unique`/`make_shared` всегда предпочтительнее** прямого `new`: исключение-безопасность и отсутствие «голого» `new` в коде.
