# Лекция 5. Классы: поля, методы, инкапсуляция

Примеры к лекции. Покрывают все пункты плана: отличие `struct` от `class`,
определение полей и методов, модификаторы доступа, указатель `this` и fluent
interface, `const`-методы и `mutable`, статические члены, `friend`, вложенные
классы и `enum class`, выравнивание и padding, инкапсуляцию и идиому PIMPL.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск примеров:

```bash
./build/01_struct_vs_class
./build/02_fields_methods
./build/03_access_modifiers
./build/04_this_fluent
./build/05_const_mutable
./build/06_static_members
./build/07_friend
./build/08_nested_enum
./build/09_sizeof_padding
./build/10_encapsulation_pimpl
```

## Примеры

| Папка | Пункт плана | О чём |
|-------|-------------|-------|
| [`01_struct_vs_class`](01_struct_vs_class) | 1 | Единственное отличие `struct` от `class` — доступ по умолчанию |
| [`02_fields_methods`](02_fields_methods) | 2 | Поля и методы; определение внутри класса и вынос в `.cpp` |
| [`03_access_modifiers`](03_access_modifiers) | 3 | `public`, `protected`, `private` и контроль инвариантов |
| [`04_this_fluent`](04_this_fluent) | 4 | Указатель `this` и fluent interface (цепочка вызовов) |
| [`05_const_mutable`](05_const_mutable) | 5 | `const`-методы и `mutable` для кэша/счётчиков |
| [`06_static_members`](06_static_members) | 6 | Статические поля и методы, `inline static` (C++17) |
| [`07_friend`](07_friend) | 7 | Дружественные функции, операторы и классы |
| [`08_nested_enum`](08_nested_enum) | 8 | Вложенные классы и `enum class` (scoped enum) |
| [`09_sizeof_padding`](09_sizeof_padding) | 9 | `sizeof`, выравнивание, padding, `alignas` |
| [`10_encapsulation_pimpl`](10_encapsulation_pimpl) | 10 | Инкапсуляция и идиома PIMPL |

## Ключевые идеи лекции

1. **`struct` и `class` различаются только доступом по умолчанию.** У `struct` члены `public`, у `class` — `private`.
2. **Инкапсуляция** — сокрытие реализации за стабильным интерфейсом; поля делают `private`, доступ дают через методы с проверкой инвариантов.
3. **`this`** — неявный указатель на текущий объект; возврат `*this` даёт fluent interface (цепочку вызовов).
4. **`const`-метод** обещает не менять наблюдаемое состояние; **`mutable`** — узаконенное исключение для кэшей и счётчиков.
5. **Статические члены** принадлежат классу, а не объекту; `inline static` (C++17) позволяет инициализировать их прямо в заголовке.
6. **`friend`** выборочно открывает доступ к `private`; не наследуется и не транзитивен — применять экономно.
7. **Порядок полей влияет на `sizeof`** из-за выравнивания и padding; сортировка по убыванию размера экономит память.
8. **PIMPL** прячет реализацию за указателем `unique_ptr<Impl>`: ускоряет компиляцию и стабилизирует ABI ценой косвенности и кучи.
