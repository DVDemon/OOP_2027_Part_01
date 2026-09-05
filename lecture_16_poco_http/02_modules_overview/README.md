# 02. Обзор модулей POCO

**Пункт плана:** 2 — обзор модулей (Foundation, Net, Util, JSON, Data).

## Что показывает раздел

Обзорный материал без кода (сборка не требуется). POCO разбит на модули; каждый
поставляется отдельной библиотекой и отдельной импортируемой целью CMake.

| Модуль | Назначение | Ключевые классы | CMake-цель |
|--------|-----------|-----------------|------------|
| **Foundation** | Строки, логирование, потоки, таймеры, пути, URI | `Logger`, `Thread`, `Mutex`, `Path`, `URI` | `Poco::Foundation` |
| **Net** | TCP/UDP-сокеты, HTTP-клиент и сервер, WebSocket | `HTTPServer`, `HTTPClientSession`, `ServerSocket` | `Poco::Net` |
| **Util** | Серверные приложения, конфигурации, CLI-опции | `ServerApplication`, `Option`, `AbstractConfiguration` | `Poco::Util` |
| **JSON** | Парсинг и генерация JSON | `JSON::Parser`, `JSON::Object`, `JSON::Array` | `Poco::JSON` |
| **Data** | Доступ к БД (SQLite, MySQL, PostgreSQL, ODBC) | `Session`, `Statement`, `RecordSet` | `Poco::Data` |
| **XML** | SAX- и DOM-парсеры | `SAXParser`, `DOMParser`, `XMLWriter` | `Poco::XML` |
| **Crypto** | Шифрование, хэширование, X.509 | `Cipher`, `DigestEngine`, `X509Certificate` | `Poco::Crypto` |

## Что нужно для HTTP-сервера

Минимально — три модуля:

* **Foundation** — фундамент, от него зависят все остальные.
* **Net** — собственно `HTTPServer`, обработчики, сокеты.
* **Util** — `ServerApplication` и разбор конфигурации.

Для формирования JSON-ответов добавляют **JSON**. Именно эти четыре цели
линкуются в примере [`10_hello_http_server`](../10_hello_http_server).

## Зависимости между модулями

Net и Util зависят от Foundation. Поэтому в `target_link_libraries` важно
перечислить все нужные цели — CMake-цели POCO сами не «подтягивают» друг друга,
кроме явных транзитивных зависимостей.
