# 06. `enable_shared_from_this`

**Пункт плана:** 6.

## Что показывает пример

* Как объект безопасно получает `shared_ptr` на самого себя.
* Отложенная задача держит `shared_ptr` на объект и тем самым продлевает его
  жизнь до выполнения задачи; задача оформлена отдельным классом.
* Ограничение: объектом должен владеть `shared_ptr`, иначе `shared_from_this()`
  бросает `std::bad_weak_ptr` (в примере это показано на объекте со стека).

## Запуск

```bash
./build/06_enable_shared_from_this
```

## Ожидаемый вывод

```
Server(1) создан
Server(1): задача поставлена
main: use_count(server) = 2 (локальная ссылка + задача в очереди)
main: локальной ссылки нет, живых Server = 1, задач в очереди = 1
main: выполняем отложенные задачи
Server(1): операция завершена
Server(1) разрушен
main: очередь пуста, живых Server = 0

--- ловушка: объект не во владении shared_ptr ---
Server(2) создан
main: shared_from_this() бросил std::bad_weak_ptr
Server(2) разрушен
```

## Как устроено

Очередь задач хранит `unique_ptr<Task>`; задача — объект, а не замыкание:

```cpp
class Task {
public:
    virtual ~Task() = default;
    virtual void run() = 0;
};

class TaskQueue {
    std::vector<std::unique_ptr<Task>> tasks_;
};

class Server : public std::enable_shared_from_this<Server> {
    class CompleteTask : public Task {          // задача держит владельца
    public:
        explicit CompleteTask(std::shared_ptr<Server> server)
            : server_(std::move(server)) {}
        void run() override { server_->onComplete(); }
    private:
        std::shared_ptr<Server> server_;        // продлевает жизнь сервера
    };

    void startAsync(TaskQueue& queue) {
        queue.push(std::make_unique<CompleteTask>(shared_from_this()));
    }
};
```

**Почему это работает:** `shared_from_this()` возвращает `shared_ptr`,
разделяющий control block владельца, поэтому `use_count` становится 2 —
локальная ссылка плюс копия внутри задачи. Когда локальная ссылка исчезает,
объект жив, пока жива задача; после её выполнения и очистки очереди счётчик
падает до нуля и вызывается деструктор.

## Плохо → Хорошо

**Плохо** — собрать `shared_ptr` из `this` напрямую:

```cpp
void startAsync(TaskQueue& queue) {
    std::shared_ptr<Server> self(this);   // НОВЫЙ control block!
    queue.push(std::make_unique<CompleteTask>(self));
}   // два независимых владельца одного объекта -> двойной delete
```

**Хорошо** — `shared_from_this()`:

```cpp
void startAsync(TaskQueue& queue) {
    queue.push(std::make_unique<CompleteTask>(shared_from_this()));  // тот же блок
}
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
