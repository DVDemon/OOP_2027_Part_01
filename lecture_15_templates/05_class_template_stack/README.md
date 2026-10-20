# 05. Шаблон стека

**Пункт плана:** 7–8 — шаблон класса и его методы.

## Что показывает пример

* Реализацию шаблонного класса `Stack<T>`.
* Методы push, pop, top, empty, size.
* Использование с int и string.

## Запуск

```bash
./build/02_stack_template
```

## Плохо → Хорошо

**Плохо** — стек только для int:

```cpp
class IntStack { /* ... */ };
```

**Хорошо** — шаблонный стек для любого типа:

```cpp
template <typename T>
class Stack { /* ... */ };
Stack<int> s1;
Stack<std::string> s2;
```
