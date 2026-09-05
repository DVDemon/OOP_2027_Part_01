# 08. Вложенные классы и `enum class`

**Пункт плана:** 8 — вложенные классы и `enum class`.

## Что показывает пример

* Вложенный `struct Node` — деталь реализации `LinkedList`, скрытая в `private`
  и недоступная снаружи.
* `enum class Error` (scoped enum), объявленный внутри класса: имена доступны как
  `LinkedList::Error::Empty`.
* `enum class` **не приводится** к `int` неявно (нужен `static_cast`) и не
  засоряет внешнее пространство имён.
* Значение `enum class` можно бросить как исключение и обработать.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/08_nested_enum
```

## Плохо → Хорошо

**Плохо** — старый небезопасный `enum` и публичный тип-деталь реализации:

```cpp
enum Error { Empty, OutOfRange, InvalidArg };  // имена утекают в общую область
struct Node { int data; Node* next; };          // деталь реализации видна всем

int x = Empty;            // неявное приведение к int — легко перепутать
if (Empty == 0) { /*...*/ }  // компилируется, хотя сравнение бессмысленно
```

**Почему плохо:** обычный `enum` сливает имена констант в окружающее
пространство имён (конфликты `Empty`, `OutOfRange`) и неявно приводится к `int`,
что прячет ошибки. Публичный `Node` раскрывает внутреннее устройство списка.

**Хорошо** — `enum class` и вложенный `private`-класс:

```cpp
class LinkedList {
public:
    enum class Error { Empty, OutOfRange, InvalidArg };  // имена в области Error
private:
    struct Node { int data; Node* next; };               // скрыт от клиента
};

auto e = LinkedList::Error::Empty;        // явная квалификация
int n = static_cast<int>(e);              // приведение только осознанно
```

**Вывод:** используйте `enum class` вместо `enum` (безопасность типов, нет утечки
имён), а типы-детали реализации делайте вложенными и `private`.
