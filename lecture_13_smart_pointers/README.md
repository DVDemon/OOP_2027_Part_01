# Лекция 13. Умные указатели

Примеры к лекции. Покрывают все пункты плана: мотивацию RAII, `std::unique_ptr`,
`std::shared_ptr`, `std::weak_ptr`, `enable_shared_from_this`, приведение типов
для умных указателей, учебные реализации `UniquePtr`/`SharedPtr`, работу с
полиморфизмом и практические рекомендации.

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
./build/05_enable_shared_from_this
./build/06_pointer_cast
./build/07_custom_unique_ptr
./build/08_custom_shared_ptr
./build/09_polymorphism
./build/10_best_practices
```

## Примеры

| Папка | Пункт плана | О чём |
|-------|-------------|-------|
| [`01_motivation`](01_motivation) | 1 | Ручной `new`/`delete` + исключения = утечки; RAII-решение |
| [`02_unique_ptr`](02_unique_ptr) | 2 | Эксклюзивное владение, `make_unique`, перемещение, пользовательский deleter |
| [`03_shared_ptr`](03_shared_ptr) | 3 | Разделяемое владение, `use_count`, `make_shared` vs конструктор |
| [`04_weak_ptr`](04_weak_ptr) | 4 | Циклические ссылки и их разрыв через `weak_ptr` |
| [`05_enable_shared_from_this`](05_enable_shared_from_this) | 5 | Безопасный `shared_ptr` на самого себя |
| [`06_pointer_cast`](06_pointer_cast) | 6 | `dynamic_pointer_cast` / `static_pointer_cast` |
| [`07_custom_unique_ptr`](07_custom_unique_ptr) | 7 | Учебная реализация `UniquePtr` |
| [`08_custom_shared_ptr`](08_custom_shared_ptr) | 8 | Учебная реализация `SharedPtr` |
| [`09_polymorphism`](09_polymorphism) | 9 | Умные указатели в полиморфных иерархиях, фабрика |
| [`10_best_practices`](10_best_practices) | 10 | Практические рекомендации и типичные ошибки |

## Ключевые идеи лекции

1. **`new`/`delete` в пользовательском коде — анахронизм.** Владение ресурсом выражают умными указателями (RAII): освобождение происходит в деструкторе автоматически, даже при исключениях.
2. **`unique_ptr` — выбор по умолчанию.** Эксклюзивное владение, нулевые накладные расходы, копирование запрещено, передача владения — только `std::move`.
3. **`shared_ptr` — разделяемое владение** через подсчёт ссылок. `make_shared` эффективнее (одна аллокация) и безопаснее конструктора от сырого указателя.
4. **`weak_ptr` разрывает циклы.** `shared_ptr` в обе стороны цикла → утечка; одна из связей должна быть `weak_ptr`.
5. **`enable_shared_from_this`** даёт объекту корректный `shared_ptr` на себя; нельзя звать из конструктора и при отсутствии управляющего `shared_ptr`.
6. **`make_unique`/`make_shared` всегда предпочтительнее** прямого `new`: исключение-безопасность и отсутствие «голого» `new` в коде.
