# 10. Полный рабочий пример: Hello World HTTP-сервер

**Пункт плана:** 10 — полный рабочий пример: Hello World HTTP-сервер.

> **Требует POCO** (Foundation, Net, Util, JSON). Цель `10_hello_http_server`
> собирается только при `Poco_FOUND` (`brew install poco`).

## Что показывает пример

Собирает воедино всю лекцию:

* `ServerApplication` — жизненный цикл приложения (раздел 09).
* `HTTPServer` + `HandlerFactory` (Factory Method) + обработчики (разделы 05–07).
* `HTTPServerRequest` / `HTTPServerResponse` и JSON-ответы (раздел 08).
* Пул потоков через `HTTPServerParams` (раздел 06).

## Маршруты

| Метод | URI | Ответ |
|-------|-----|-------|
| GET / POST | `/`, `/hello` | `{"message":"Hello, World!", ...}` |
| POST | `/echo` | эхо тела запроса в JSON |
| любой | прочее | 404 `{"error":"Not Found", ...}` |

## Сборка и запуск

```bash
# нужен установленный POCO: brew install poco
cmake -S . -B build && cmake --build build
./build/10_hello_http_server
# HTTP server running on http://localhost:8080
```

## Тестирование с curl

```bash
curl http://localhost:8080/
# {"message":"Hello, World!","method":"GET","uri":"\/"}

curl http://localhost:8080/hello
# {"message":"Hello, World!","method":"GET","uri":"\/hello"}

curl -X POST http://localhost:8080/echo \
     -H "Content-Type: application/json" -d '{"name":"Alice"}'
# {"body":"{\"name\":\"Alice\"}","body_length":16,...}

curl http://localhost:8080/api/users
# {"error":"Not Found","path":"\/api\/users"}
```

## Связь с ООП

| Концепция | Где в коде |
|-----------|-----------|
| **Наследование** | `HelloHandler : public HTTPRequestHandler` |
| **Полиморфизм** | `createRequestHandler` возвращает `HTTPRequestHandler*` |
| **Factory Method** | `HandlerFactory::createRequestHandler` выбирает тип по URI |
| **RAII** | `ServerApplication`: `initialize` → `main` → `uninitialize` |
| **SRP** | один обработчик = один маршрут |
| **OCP** | новый маршрут = новый класс, без правки существующих |
| **DIP** | `HTTPServer` зависит от абстракции фабрики |

## Плохо → Хорошо

**Плохо** — удалить фабрику вручную: `HTTPServer` владеет ею и удалит сам —
получите двойное удаление.

```cpp
auto* factory = new HandlerFactory;
Poco::Net::HTTPServer server(factory, socket, params);
delete factory;  // ОШИБКА: сервер уже владеет фабрикой
```

**Хорошо** — передать `new HandlerFactory` напрямую и не трогать владение:

```cpp
Poco::Net::HTTPServer server(new HandlerFactory, socket, params);
// сервер сам удалит фабрику и params
```
