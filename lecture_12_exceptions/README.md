# Лекция 12. Обработка исключений. Гарантии безопасности

Примеры к лекции. Покрывают все пункты плана: сравнение кодов ошибок и
исключений, синтаксис `throw`/`try`/`catch`, иерархию `std::exception`,
раскрутку стека, четыре уровня гарантий безопасности, RAII, идиому
copy-and-swap, спецификатор и оператор `noexcept`, корректный повторный
выброс и продвинутые темы (`exception_ptr`, `nested_exception`, исключения в
конструкторах и деструкторах).

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск примеров:

```bash
./build/01_error_codes_vs_exceptions
./build/02_throw_try_catch
./build/03_exception_hierarchy
./build/04_stack_unwinding
./build/05_safety_guarantees
./build/06_raii_and_exceptions
./build/07_copy_and_swap
./build/08_noexcept
./build/09_rethrow
./build/10_advanced
```

## Примеры

| Папка | Пункт плана | О чём |
|-------|-------------|-------|
| [`01_error_codes_vs_exceptions`](01_error_codes_vs_exceptions) | 1 | Коды ошибок vs исключения: почему код можно «забыть» проверить |
| [`02_throw_try_catch`](02_throw_try_catch) | 2 | Синтаксис `throw`/`try`/`catch`, порядок catch-блоков |
| [`03_exception_hierarchy`](03_exception_hierarchy) | 3 | Иерархия `std::exception`, свои классы исключений |
| [`04_stack_unwinding`](04_stack_unwinding) | 4 | Раскрутка стека: порядок разрушения локальных объектов |
| [`05_safety_guarantees`](05_safety_guarantees) | 5 | Гарантии: none / basic / strong / nothrow |
| [`06_raii_and_exceptions`](06_raii_and_exceptions) | 6 | RAII против утечек при раскрутке стека |
| [`07_copy_and_swap`](07_copy_and_swap) | 7 | Copy-and-swap для строгой гарантии |
| [`08_noexcept`](08_noexcept) | 8 | `noexcept`: спецификатор и оператор |
| [`09_rethrow`](09_rethrow) | 9 | `throw;` vs `throw e;` — срезка при повторном выбросе |
| [`10_advanced`](10_advanced) | 10 | `exception_ptr`, `nested_exception`, конструкторы/деструкторы |

## Ключевые идеи лекции

1. **Исключения нельзя «забыть».** В отличие от кода возврата, непойманное исключение раскрутит стек и завершит программу — ошибка не теряется молча.
2. **Раскрутка стека** разрушает локальные объекты в обратном порядке создания. На этом строится RAII.
3. **RAII** — единственный надёжный способ не допустить утечки ресурсов при исключении: освобождение в деструкторе.
4. **Четыре гарантии:** nothrow (никогда не бросает) → strong (commit-or-rollback) → basic (корректное, но неопределённое состояние) → none (плохой код).
5. **Copy-and-swap** даёт строгую гарантию: вся работа, которая может бросить, выполняется над копией, а финальный `swap` — `noexcept`.
6. **`noexcept`** не просто документация: нарушение обещания вызывает `std::terminate()`, а контейнеры используют move только для `noexcept`-операций.
7. **Повторный выброс — только `throw;`**. `throw e;` копирует объект по статическому типу и срезает производный класс.
8. **Деструкторы не бросают** (неявно `noexcept`); ловить исключения нужно по `const`-ссылке, иначе срезка.
