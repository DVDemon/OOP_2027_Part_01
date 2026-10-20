# 06. `dynamic_pointer_cast` / `static_pointer_cast`

**Пункт плана:** 6.

## Что показывает пример

* `static_pointer_cast` — приведение без проверки типа.
* `dynamic_pointer_cast` — приведение с проверкой; при неудаче возвращает
  пустой `shared_ptr`.
* Все полученные приведением `shared_ptr` разделяют один control block
  (общий счётчик ссылок).

## Запуск

```bash
./build/06_pointer_cast
```

## Сравнение

| | `static_pointer_cast` | `dynamic_pointer_cast` |
|---|---|---|
| Проверка типа в рантайме | нет | да |
| При неверном типе | UB | пустой `shared_ptr` |
| Аналог для сырых указателей | `static_cast` | `dynamic_cast` |

## Плохо → Хорошо

**Плохо** — `static_pointer_cast`, когда тип не гарантирован:

```cpp
auto d = std::static_pointer_cast<Derived>(base);  // тип не проверяется
d->special();   // UB, если base на самом деле не Derived
```

**Хорошо** — `dynamic_pointer_cast` с проверкой результата:

```cpp
if (auto d = std::dynamic_pointer_cast<Derived>(base)) {
    d->special();   // безопасно: сюда попадём только при верном типе
}
```

**Почему:** `static_pointer_cast` доверяет программисту и не проверяет
фактический тип — ошибка приводит к неопределённому поведению. Используйте его
лишь когда тип известен достоверно. Если уверенности нет —
`dynamic_pointer_cast` честно вернёт пустой указатель, который легко проверить.
