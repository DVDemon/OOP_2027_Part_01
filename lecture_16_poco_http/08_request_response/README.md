# 08. HTTPServerRequest и HTTPServerResponse: разбор вручную

**Пункт плана:** 8 — `HTTPServerRequest` и `HTTPServerResponse`.

## Что показывает пример

На **чистом C++20, без сети и без POCO** разбирает «сырой» HTTP-запрос и собирает
«сырой» HTTP-ответ. Цель — увидеть, какие данные стоят за методами POCO:

| Метод POCO | Что это в «сыром» HTTP |
|------------|------------------------|
| `request.getMethod()` | первое слово стартовой строки (`GET`, `POST`, ...) |
| `request.getURI()` | второе слово стартовой строки (`/echo`) |
| `request.get("Content-Type", "")` | строка заголовка `Content-Type: ...` |
| `request.stream()` | тело после пустой строки `\r\n\r\n` |
| `response.setStatus(...)` | код и причина в первой строке ответа |
| `response.setContentType(...)` | заголовок `Content-Type:` ответа |
| `response.send()` | отправка заголовков + начало тела |

## Структура HTTP-сообщения

```
POST /echo HTTP/1.1\r\n     <- стартовая строка: метод, URI, версия
Host: localhost:8080\r\n    <- заголовки
Content-Type: application/json\r\n
\r\n                        <- пустая строка отделяет заголовки от тела
{"name": "Alice"}           <- тело
```

## Запуск

```bash
./build/08_request_response
```

Выводит разобранные поля запроса и собранный текст ответа.

## Точка невозврата: response.send()

В POCO заголовки и статус задают **до** `response.send()`; вызов `send()`
отправляет их клиенту, после чего изменить статус или заголовки нельзя. В нашей
модели это отражено порядком: `build_response` сначала пишет статус и заголовки,
затем тело — поменять порядок уже нельзя.

## Плохо → Хорошо

**Плохо** — попытаться изменить заголовок после отправки:

```cpp
std::ostream& out = response.send();   // заголовки уже ушли клиенту
response.setContentType("text/plain"); // игнорируется!
out << "data";
```

**Хорошо** — сначала весь статус и заголовки, потом `send()` и тело:

```cpp
response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
response.setContentType("application/json");
std::ostream& out = response.send();
out << R"({"ok": true})";
```
