# Лекция 16. POCO: HTTP-сервер, архитектура фреймворка

Примеры к лекции. Покрывают все пункты плана: что такое POCO и его модули,
установку и интеграцию с CMake, архитектуру HTTP-сервера (HTTPServer / Factory /
Handler), модель пула потоков, жизненный цикл обработчика, классы запроса и
ответа, `ServerApplication` и полноценный пример «Hello World» HTTP-сервера.

## Что такое POCO

**POCO** (POrtable COmponents) — кроссплатформенная C++ библиотека сетевых,
файловых, JSON/XML и прочих компонентов.

* **Год создания:** 2004, автор — Gunter Obiltschnig.
* **Лицензия:** Boost Software License 1.0 (свободна для коммерческого использования).
* **Применение:** IoT, встраиваемые системы, микросервисы, REST API.
* **Философия:** C++ «в стиле стандартной библиотеки», без тяжёлых зависимостей.
* **Сайт:** <https://pocoproject.org>

## Обзор модулей POCO

| Модуль | Назначение | Ключевые классы |
|--------|-----------|-----------------|
| **Foundation** | Базовые утилиты: строки, логирование, потоки, таймеры | `Logger`, `Thread`, `Mutex`, `Path`, `URI` |
| **Net** | TCP/UDP-сокеты, HTTP-клиент и сервер, WebSocket | `HTTPServer`, `HTTPClientSession`, `ServerSocket` |
| **Util** | Серверные приложения, конфигурации, опции CLI | `ServerApplication`, `Option`, `AbstractConfiguration` |
| **JSON** | Парсинг и генерация JSON | `JSON::Parser`, `JSON::Object`, `JSON::Array` |
| **Data** | Доступ к БД (SQLite, MySQL, PostgreSQL, ODBC) | `Session`, `Statement`, `RecordSet` |

Для HTTP-сервера нужны **Foundation**, **Net**, **Util** (и **JSON** для ответов).

## Установка POCO

```bash
# macOS (Homebrew)
brew install poco

# Ubuntu / Debian
sudo apt install libpoco-dev

# vcpkg
vcpkg install poco
```

Из исходников:

```bash
git clone https://github.com/pocoproject/poco.git
cd poco && mkdir cmake-build && cd cmake-build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
sudo cmake --install .
```

## Интеграция с CMake (find_package)

POCO поставляет CMake-пакет с импортируемыми целями `Poco::Foundation`,
`Poco::Net`, `Poco::Util`, `Poco::JSON`:

```cmake
find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

add_executable(server main.cpp)
target_link_libraries(server PRIVATE
    Poco::Foundation Poco::Net Poco::Util Poco::JSON)
```

В этой лекции `CMakeLists.txt` использует `find_package(Poco QUIET ...)` и
оборачивает сетевые цели в `if(Poco_FOUND) ... endif()`. Поэтому конфигурация и
сборка **проходят успешно даже без установленного POCO** — просто пропускаются
POCO-зависимые цели. Если POCO стоит через vcpkg, добавьте к вызову CMake
`-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake`.

## Сборка

```bash
cmake -S . -B build
cmake --build build
```

Запуск примеров, не требующих POCO (собираются всегда):

```bash
./build/05_server_architecture
./build/08_request_response
```

Запуск POCO-примеров (только при установленном POCO, `brew install poco`):

```bash
./build/09_server_application
./build/10_hello_http_server          # затем: curl http://localhost:8080/
```

## Примеры

| Папка | Пункт плана | О чём | Нужен POCO |
|-------|-------------|-------|:----------:|
| [`01_what_is_poco`](01_what_is_poco) | 1 | Что такое POCO: история, лицензия, применение | нет (обзор) |
| [`02_modules_overview`](02_modules_overview) | 2 | Обзор модулей: Foundation, Net, Util, JSON, Data | нет (обзор) |
| [`03_installation`](03_installation) | 3 | Установка: brew / apt / vcpkg / из исходников | нет (обзор) |
| [`04_cmake_integration`](04_cmake_integration) | 4 | Интеграция с CMake через `find_package` | нет (обзор) |
| [`05_server_architecture`](05_server_architecture) | 5 | Архитектура HTTPServer/Factory/Handler — мини-модель без сети | **нет** |
| [`06_thread_pool_model`](06_thread_pool_model) | 6 | Модель пула потоков, `HTTPServerParams` | нет (обзор) |
| [`07_handler_lifecycle`](07_handler_lifecycle) | 7 | Жизненный цикл `HTTPRequestHandler` (один на запрос) | нет (обзор) |
| [`08_request_response`](08_request_response) | 8 | Разбор HTTP-запроса вручную и сборка ответа | **нет** |
| [`09_server_application`](09_server_application) | 9 | `ServerApplication`: жизненный цикл приложения | **да** |
| [`10_hello_http_server`](10_hello_http_server) | 10 | Полный пример: Hello World HTTP-сервер | **да** |

POCO-зависимые цели: `09_server_application`, `10_hello_http_server` — собираются
только при `Poco_FOUND`. Папки 01–04, 06, 07 — обзорные (README без сборки), они
иллюстрируют установку и понятия, которые проверяются глазами, а не компилятором.

## Ключевые идеи лекции

1. **POCO** — зрелый C++ фреймворк для сети, IoT и микросервисов; всё в одной экосистеме.
2. **Архитектура HTTP-сервера** — `HTTPServer` → `HTTPRequestHandlerFactory` → `HTTPRequestHandler`.
3. **Factory Method (GoF):** `createRequestHandler` выбирает обработчик по URI/методу.
4. **Один обработчик = один запрос:** объект создаётся фабрикой и уничтожается после `handleRequest`.
5. **Пул потоков:** каждое соединение обслуживается потоком из пула, настраиваемого `HTTPServerParams`.
6. **`response.send()` — точка невозврата:** после него заголовки и статус менять нельзя.
7. **`ServerApplication`** управляет жизненным циклом по RAII: `initialize` → `main` → `uninitialize`.
