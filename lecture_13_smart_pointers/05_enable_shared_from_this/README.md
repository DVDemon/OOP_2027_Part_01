# 05. `enable_shared_from_this`

**Пункт плана:** 5.

## Что показывает пример

* Как объект безопасно получает `shared_ptr` на самого себя.
* Захват `self` в лямбду продлевает жизнь объекта до выполнения задачи.
* Ограничение: объектом должен владеть `shared_ptr`.

## Запуск

```bash
./build/05_enable_shared_from_this
```

## Плохо → Хорошо

**Плохо** — собрать `shared_ptr` из `this` напрямую:

```cpp
void startAsync() {
    std::shared_ptr<Server> self(this);   // НОВЫЙ control block!
    asyncOperation([self]{ self->onComplete(); });
}   // два независимых владельца одного объекта -> двойной delete
```

**Хорошо** — `shared_from_this()`:

```cpp
class Server : public std::enable_shared_from_this<Server> {
    void startAsync() {
        auto self = shared_from_this();   // тот же control block
        asyncOperation([self]{ self->onComplete(); });
    }
};
```

**Почему:** `std::shared_ptr<Server>(this)` создаёт *отдельный* control block,
не зная о существующем. Появляются два независимых счётчика на один объект и,
как следствие, двойное освобождение. `enable_shared_from_this` хранит
внутренний `weak_ptr` на «настоящий» control block; `shared_from_this()`
возвращает `shared_ptr`, разделяющий именно его.

**Ограничения:**
* нельзя вызывать `shared_from_this()` из конструктора — `shared_ptr` ещё не создан;
* объектом обязан владеть `shared_ptr` (`std::make_shared<Server>(...)`),
  иначе будет брошено `std::bad_weak_ptr`.
