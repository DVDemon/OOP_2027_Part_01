# 03. Иерархия `std::exception`

**Пункт плана:** 3 — иерархия `std::exception`.

## Что показывает пример

* Наследование собственных исключений от стандартных классов.
* Перехват производного исключения по ссылке на базовый класс.
* Готовый `what()` от `std::runtime_error`.

## Запуск

```bash
./build/03_exception_hierarchy
```

## Фрагмент иерархии

```
std::exception
├── std::logic_error
│   ├── std::invalid_argument
│   ├── std::out_of_range
│   └── std::length_error
├── std::runtime_error
│   ├── std::range_error
│   └── std::overflow_error
└── std::bad_alloc
```

## Плохо → Хорошо

**Плохо** — бросать «голые» типы, не входящие в иерархию:

```cpp
throw 42;                 // или throw "строка";
// ...
catch (const std::exception& e) { /* не поймает int! */ }
```

ПОЧЕМУ плохо: `int`/`const char*` не наследуются от `std::exception`, у них
нет `what()`; единый обработчик `catch (const std::exception&)` их пропустит.

**Хорошо** — наследовать свой класс от стандартного:

```cpp
class DatabaseError : public std::runtime_error {
public:
    DatabaseError(const std::string& msg, int code)
        : std::runtime_error(msg), error_code_(code) {}
};
```

ПОЧЕМУ хорошо: исключение встраивается в общую иерархию, ловится по
`const std::exception&`, имеет готовый `what()` и собственные данные (код ошибки).
