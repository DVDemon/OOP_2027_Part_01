# 05. Архитектура HTTP-сервера: мини-модель без сети

**Пункт плана:** 5 — архитектура HTTP-сервера (HTTPServer, Factory, Handler).

## Что показывает пример

Учебную мини-модель архитектуры POCO HTTP-сервера на **чистом C++20, без сети и
без POCO**. Те же три роли и тот же поток управления:

```
HTTPServer
    └── HTTPRequestHandlerFactory   (фабрика — создаёт обработчики)
            └── HTTPRequestHandler  (обработчик — обрабатывает один запрос)
```

Поток обработки запроса в POCO (и в нашей модели):

1. Сервер принимает соединение.
2. Вызывает `Factory::createRequestHandler(request)` — **Factory Method (GoF)**.
3. Фабрика по URI/методу возвращает подходящий обработчик.
4. Сервер вызывает `handler->handleRequest(request, response)`.
5. После обработки обработчик **уничтожается** (один экземпляр на один запрос).

## Связь с ООП

* **Наследование:** `HelloHandler`/`NotFoundHandler` наследуют `RequestHandler`.
* **Полиморфизм:** фабрика возвращает указатель на базовый класс.
* **Factory Method:** `HandlerFactory::createRequestHandler` выбирает тип.
* **DIP:** `Server` зависит от абстракции фабрики, не от конкретных обработчиков.
* **OCP:** новый маршрут = новый класс-обработчик, сервер не меняется.

## Запуск

```bash
./build/05_server_architecture
```

Ожидаемый вывод:

```
GET /  ->  200 Hello, World! (GET /)
GET /hello  ->  200 Hello, World! (GET /hello)
POST /hello  ->  200 Hello, World! (POST /hello)
GET /api/users  ->  404 Not Found: /api/users
```

## Отличия от настоящего POCO

В POCO `createRequestHandler` возвращает **сырой** указатель `HTTPRequestHandler*`,
и владение переходит серверу (он удалит объект). Здесь для наглядности владение
выражено через `std::unique_ptr`. Реальный сервер ещё и распределяет запросы по
пулу потоков — это разбирается в разделе [`06_thread_pool_model`](../06_thread_pool_model).
