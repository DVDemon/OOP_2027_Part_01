# 07. Жизненный цикл HTTPRequestHandler

**Пункт плана:** 7 — жизненный цикл `HTTPRequestHandler`.

## Что показывает раздел

Обзорный материал без кода (сборка не требуется). Главное правило:

> **Один экземпляр обработчика = один HTTP-запрос.**

```cpp
class MyHandler : public Poco::Net::HTTPRequestHandler {
public:
    // 1. Конструктор вызывает фабрика (Factory::createRequestHandler).
    void handleRequest(
        Poco::Net::HTTPServerRequest& request,
        Poco::Net::HTTPServerResponse& response) override
    {
        // 2. Обработка запроса.
        //    После возврата из метода объект УНИЧТОЖАЕТСЯ.
    }
    // 3. Деструктор вызывается автоматически после handleRequest.
};
```

## Следствия

* **Нет гонок внутри обработчика** — он не разделяется между потоками, у каждого
  запроса свой экземпляр.
* **Разделяемое состояние** (кэш, счётчик, БД) храните в фабрике или во внешнем
  объекте и передавайте по ссылке; защищайте `std::mutex`, т.к. разные
  обработчики бегут в разных потоках.
* **Не делайте тяжёлую инициализацию в конструкторе обработчика** — он создаётся
  на каждый запрос.

## Где живёт общее состояние

```cpp
class CountingFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(
        const Poco::Net::HTTPServerRequest&) override
    {
        // Счётчик живёт в фабрике (она создаётся один раз),
        // а не в обработчике (он создаётся на каждый запрос).
        return new MyHandler(counter_);
    }
private:
    std::atomic<long> counter_{0};
};
```

## Плохо → Хорошо

**Плохо** — хранить состояние между запросами в поле обработчика:

```cpp
class Handler : public Poco::Net::HTTPRequestHandler {
    int requests_seen_ = 0;  // всегда 0: новый объект на каждый запрос!
};
```

**Хорошо** — состояние в фабрике (создаётся один раз), передаётся обработчику по
ссылке и защищается синхронизацией.
