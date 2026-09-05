# 04. Указатель `this` и fluent interface

**Пункт плана:** 4 — указатель `this` и fluent interface.

## Что показывает пример

* `this` — неявный указатель на текущий объект; тип `QueryBuilder* const`
  (в `const`-методе — `const QueryBuilder* const`).
* Возврат `*this` из настраивающих методов даёт **fluent interface** —
  цепочку вызовов `from(...).where(...).limit(...).build()`.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/04_this_fluent
```

Цели: `04_this_fluent` — демонстрация в консоли.

## Плохо → Хорошо

**Плохо** — настраивать объект «по шагам», возвращая `void`:

```cpp
QueryBuilder q;
q.from("users");
q.where("age > 18");
q.limit(10);
std::string sql = q.build();   // четыре отдельных оператора, шумно
```

**Почему плохо:** код многословен, легко забыть шаг или применить методы к
не тому объекту; читателю труднее увидеть, что это единая настройка одного запроса.

**Хорошо** — каждый метод возвращает `*this`, вызовы соединяются в цепочку:

```cpp
QueryBuilder& from(const std::string& table) {
    table_ = table;
    return *this;   // ссылка на себя
}

auto sql = QueryBuilder{}.from("users").where("age > 18").limit(10).build();
```

**Вывод:** возвращайте `*this` (ссылку, не копию!) из методов-«сеттеров»,
когда хотите выразительную и компактную настройку объекта.
