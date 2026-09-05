# Лекция 16. POCO: HTTP-сервер, архитектура фреймворка

## План лекции

1. Что такое POCO (история, лицензия, области применения)
2. Обзор модулей POCO (Foundation, Net, Util, JSON, Data)
3. Установка (brew / apt / vcpkg / из исходников)
4. Интеграция с CMake (find_package)
5. Архитектура HTTP-сервера (HTTPServer, Factory, Handler)
6. Модель пула потоков
7. Жизненный цикл HTTPRequestHandler
8. HTTPServerRequest и HTTPServerResponse
9. ServerApplication: жизненный цикл приложения
10. Полный рабочий пример: Hello World HTTP-сервер

---

## 1. Что такое POCO
> **Пример:** [01_what_is_poco](01_what_is_poco)

**POCO** (POrtable COmponents) — кроссплатформенная C++ библиотека, предоставляющая сетевые, файловые, XML/JSON и другие компоненты.

- **Год создания:** 2004, автор — Gunter Obiltschnig
- **Лицензия:** Boost Software License 1.0 (можно использовать в коммерческих проектах без ограничений)
- **Области применения:** IoT, встраиваемые системы, микросервисы, REST API
- **Философия:** C++ без «тяжёлых» зависимостей, в стиле стандартной библиотеки
- **Сайт:** https://pocoproject.org

**Почему POCO, а не Boost.Beast / cpp-httplib / другие?**
- Цельный фреймворк: сеть, HTTP, JSON, логирование, потоки — всё в одной экосистеме.
- Стабильный API, совместимый с C++14/17/20.
- Активно используется в embedded и IoT (macchina.io).

---

## 2. Обзор модулей POCO
> **Пример:** [02_modules_overview](02_modules_overview)

| Модуль | Назначение | Ключевые классы |
|--------|-----------|----------------|
| **Foundation** | Базовые утилиты: строки, потоки, логирование, таймеры | `Logger`, `Thread`, `Mutex`, `Path`, `URI` |
| **Net** | TCP/UDP сокеты, HTTP-клиент и сервер, WebSocket | `HTTPServer`, `HTTPClientSession`, `ServerSocket` |
| **Util** | Серверные приложения, парсинг конфигураций, опции CLI | `ServerApplication`, `Option`, `AbstractConfiguration` |
| **JSON** | Парсинг и генерация JSON | `JSON::Parser`, `JSON::Object`, `JSON::Array` |
| **Data** | Доступ к БД (SQLite, MySQL, PostgreSQL, ODBC) | `Session`, `Statement`, `RecordSet` |
| **XML** | SAX и DOM парсеры | `SAXParser`, `DOMParser`, `XMLWriter` |
| **Crypto** | Шифрование, хэширование, X.509 | `Cipher`, `DigestEngine`, `X509Certificate` |

Для HTTP-сервера нам потребуются: **Foundation**, **Net** и **Util**.

---

## 3. Установка POCO
> **Пример:** [03_installation](03_installation)

### macOS (Homebrew)

```bash
brew install poco
```

### Ubuntu / Debian

```bash
sudo apt install libpoco-dev
```

### vcpkg

```bash
vcpkg install poco
```

### Из исходников

```bash
git clone https://github.com/pocoproject/poco.git
cd poco
mkdir cmake-build && cd cmake-build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
sudo cmake --install .
```

---

## 4. Интеграция с CMake
> **Пример:** [04_cmake_integration](04_cmake_integration)

### CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.14)
project(hello_http_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Найти POCO
find_package(Poco REQUIRED COMPONENTS Foundation Net Util)

add_executable(server main.cpp)

# Линковка
target_link_libraries(server
    Poco::Foundation
    Poco::Net
    Poco::Util
)
```

**Если POCO установлен через vcpkg**, добавьте в вызов CMake:
```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
```

---

## 5. Архитектура HTTP-сервера
> **Пример:** [05_server_architecture](05_server_architecture)

POCO HTTP-сервер построен на трёх ключевых классах:

```
HTTPServer
    └── HTTPRequestHandlerFactory   (фабрика — создаёт обработчики)
            └── HTTPRequestHandler  (обработчик — обрабатывает один запрос)
```

### Поток обработки запроса

1. `HTTPServer` слушает сокет и принимает TCP-соединения.
2. Для каждого соединения из пула потоков выделяется поток.
3. Сервер вызывает `HTTPRequestHandlerFactory::createRequestHandler(request)`.
4. Фабрика анализирует URI/метод и возвращает подходящий `HTTPRequestHandler`.
5. Сервер вызывает `handler->handleRequest(request, response)`.
6. После завершения обработки объект `handler` **уничтожается** (один экземпляр на один запрос).

### Связь с ООП

- **Factory Method (GoF):** `HTTPRequestHandlerFactory::createRequestHandler` — классический фабричный метод.
- **Наследование:** ваш обработчик наследует `HTTPRequestHandler` и переопределяет `handleRequest`.
- **RAII:** `ServerApplication` управляет жизненным циклом сервера.

---

## 6. Модель пула потоков
> **Пример:** [06_thread_pool_model](06_thread_pool_model)

`HTTPServer` использует пул потоков для обработки запросов:

```
[Клиент 1] ──TCP──┐
[Клиент 2] ──TCP──┤     ┌──────────────────┐
[Клиент 3] ──TCP──┼────>│   HTTPServer     │
[Клиент N] ──TCP──┘     │   (ServerSocket) │
                         └──────┬───────────┘
                                │
                    ┌───────────┼───────────┐
                    │           │           │
               [Thread 1] [Thread 2] [Thread N]
                    │           │           │
               Handler     Handler     Handler
              (создан)    (создан)    (создан)
                 ...         ...         ...
              (удалён)    (удалён)    (удалён)
```

Параметры пула настраиваются через `HTTPServerParams`:

```cpp
auto params = new Poco::Net::HTTPServerParams;
params->setMaxQueued(100);    // максимум запросов в очереди
params->setMaxThreads(16);    // максимум потоков в пуле
params->setKeepAlive(true);   // HTTP Keep-Alive
params->setTimeout(Poco::Timespan(15, 0));  // таймаут 15 секунд
```

---

## 7. Жизненный цикл HTTPRequestHandler
> **Пример:** [07_handler_lifecycle](07_handler_lifecycle)

**Один экземпляр обработчика = один HTTP-запрос.** Это важно:

```cpp
class MyHandler : public Poco::Net::HTTPRequestHandler {
    // Конструктор вызывается фабрикой (Factory::createRequestHandler)
public:
    void handleRequest(
        Poco::Net::HTTPServerRequest& request,
        Poco::Net::HTTPServerResponse& response) override
    {
        // Обработка запроса
        // После возврата из этого метода объект УНИЧТОЖАЕТСЯ
    }
    // Деструктор вызывается автоматически после handleRequest
};
```

**Следствия:**
- Нет необходимости думать о потокобезопасности внутри обработчика — он не разделяется между потоками.
- Если нужно разделяемое состояние (кэш, счётчик), храните его в фабрике или во внешнем объекте, переданном по ссылке.
- Не пишите тяжёлую инициализацию в конструкторе обработчика — он создаётся на каждый запрос.

---

## 8. HTTPServerRequest и HTTPServerResponse
> **Пример:** [08_request_response](08_request_response)

### HTTPServerRequest — входящий запрос

```cpp
void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    // Метод запроса: GET, POST, PUT, DELETE, ...
    std::string method = request.getMethod();

    // URI запроса: /api/users?page=2
    std::string uri = request.getURI();

    // Заголовки
    std::string content_type = request.get("Content-Type", "");
    std::string auth = request.get("Authorization", "");

    // Тело запроса (для POST/PUT)
    std::istream& body_stream = request.stream();
    std::string body(std::istreambuf_iterator<char>(body_stream), {});

    // Информация о клиенте
    std::string client_ip = request.clientAddress().toString();
}
```

### HTTPServerResponse — ответ

```cpp
void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) {
    // Устанавливаем статус и заголовки ДО вызова send()
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    response.setContentType("application/json");
    response.set("X-Custom-Header", "value");

    // send() возвращает поток — записываем тело ответа
    std::ostream& out = response.send();
    out << R"({"message": "Hello, World!"})";

    // После send() нельзя менять статус и заголовки!
}
```

**Важно:** вызов `response.send()` отправляет заголовки клиенту. После этого изменить статус или добавить заголовки невозможно.

---

## 9. ServerApplication: жизненный цикл
> **Пример:** [09_server_application](09_server_application)

`Poco::Util::ServerApplication` — базовый класс для серверных приложений. Он управляет:

- Разбором аргументов командной строки
- Логированием
- Демонизацией (на Linux)
- Корректным завершением по сигналу (SIGINT/SIGTERM)

### Жизненный цикл

```
                 ┌───────────────────┐
                 │   Конструктор     │
                 └────────┬──────────┘
                          │
                 ┌────────▼──────────┐
                 │   initialize()    │  ← загрузка конфигурации, логирование
                 └────────┬──────────┘
                          │
                 ┌────────▼──────────┐
                 │      main()       │  ← основная логика, запуск HTTPServer
                 │                   │
                 │ waitForTermination│  ← блокируется до Ctrl+C / SIGTERM
                 │    Request()      │
                 └────────┬──────────┘
                          │
                 ┌────────▼──────────┐
                 │  uninitialize()   │  ← освобождение ресурсов
                 └───────────────────┘
```

### Макрос POCO_SERVER_MAIN

```cpp
POCO_SERVER_MAIN(MyServerApp)
```

Этот макрос разворачивается в `main()`, создаёт экземпляр `MyServerApp` и вызывает `run(argc, argv)`, который последовательно вызывает `initialize()`, `main()`, `uninitialize()`.

---

## 10. Полный рабочий пример: Hello World HTTP-сервер
> **Пример:** [10_hello_http_server](10_hello_http_server)

### Файл: main.cpp

```cpp
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/JSON/Object.h>

#include <iostream>
#include <string>

// ===== Обработчик запросов =====
class HelloHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(
        Poco::Net::HTTPServerRequest& request,
        Poco::Net::HTTPServerResponse& response) override
    {
        // Логируем запрос
        std::cout << request.getMethod() << " " << request.getURI()
                  << " from " << request.clientAddress().toString()
                  << std::endl;

        // Формируем JSON-ответ
        Poco::JSON::Object json;
        json.set("message", "Hello, World!");
        json.set("method", request.getMethod());
        json.set("uri", request.getURI());

        // Отправляем ответ
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        json.stringify(out);
    }
};

// ===== Обработчик 404 =====
class NotFoundHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(
        Poco::Net::HTTPServerRequest& request,
        Poco::Net::HTTPServerResponse& response) override
    {
        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        out << R"({"error": "Not Found", "path": ")" << request.getURI() << "\"}";
    }
};

// ===== Фабрика обработчиков (Factory Method паттерн) =====
class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(
        const Poco::Net::HTTPServerRequest& request) override
    {
        // Маршрутизация по URI
        const std::string& uri = request.getURI();

        if (uri == "/" || uri == "/hello") {
            return new HelloHandler;
        }

        return new NotFoundHandler;
    }
};

// ===== Серверное приложение =====
class HelloServerApp : public Poco::Util::ServerApplication {
protected:
    // Инициализация — вызывается перед main()
    void initialize(Poco::Util::Application& self) override {
        // Загрузить конфигурацию по умолчанию
        loadConfiguration();
        ServerApplication::initialize(self);
        logger().information("Server initializing...");
    }

    // Основная логика
    int main(const std::vector<std::string>& args) override {
        // Порт из конфигурации или 8080 по умолчанию
        unsigned short port = static_cast<unsigned short>(
            config().getUInt("http.port", 8080)
        );

        // Настройки сервера
        auto params = new Poco::Net::HTTPServerParams;
        params->setMaxQueued(100);
        params->setMaxThreads(4);

        // Создаём серверный сокет
        Poco::Net::ServerSocket socket(port);

        // Создаём HTTP-сервер с нашей фабрикой
        Poco::Net::HTTPServer server(
            new HandlerFactory,   // фабрика (сервер владеет ей)
            socket,
            params                // параметры (сервер владеет ими)
        );

        // Запускаем
        server.start();
        logger().information("Server started on port " + std::to_string(port));
        std::cout << "HTTP server running on http://localhost:" << port << std::endl;
        std::cout << "Press Ctrl+C to stop." << std::endl;

        // Ждём сигнала завершения (Ctrl+C или SIGTERM)
        waitForTerminationRequest();

        // Корректное завершение
        server.stop();
        logger().information("Server stopped.");

        return Application::EXIT_OK;
    }

    // Освобождение ресурсов
    void uninitialize() override {
        logger().information("Server uninitializing...");
        ServerApplication::uninitialize();
    }
};

// Точка входа
POCO_SERVER_MAIN(HelloServerApp)
```

### Файл: CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.14)
project(hello_http_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

add_executable(server main.cpp)

target_link_libraries(server
    Poco::Foundation
    Poco::Net
    Poco::Util
    Poco::JSON
)
```

### Сборка и запуск

```bash
mkdir build && cd build
cmake ..
cmake --build .
./server
```

### Тестирование с curl

```bash
# GET на корень
curl -v http://localhost:8080/
# {"message":"Hello, World!","method":"GET","uri":"\/"}

# GET на /hello
curl http://localhost:8080/hello
# {"message":"Hello, World!","method":"GET","uri":"\/hello"}

# POST запрос
curl -X POST http://localhost:8080/hello
# {"message":"Hello, World!","method":"POST","uri":"\/hello"}

# Несуществующий маршрут
curl http://localhost:8080/api/users
# {"error": "Not Found", "path": "/api/users"}
```

---

## Связь с теорией ООП

Этот пример демонстрирует ключевые концепции курса:

| Концепция | Где в коде |
|-----------|-----------|
| **Наследование** | `HelloHandler : public HTTPRequestHandler` |
| **Полиморфизм** | `createRequestHandler` возвращает `HTTPRequestHandler*`, а реально это `HelloHandler` или `NotFoundHandler` |
| **Factory Method (паттерн)** | `HandlerFactory::createRequestHandler` — выбирает тип обработчика на основе URI |
| **RAII** | `ServerApplication` управляет жизненным циклом: `initialize` -> `main` -> `uninitialize` |
| **SRP** | Каждый обработчик отвечает за один маршрут |
| **OCP** | Добавление нового маршрута = новый класс Handler, без изменения существующих |
| **DIP** | `HTTPServer` зависит от абстракции `HTTPRequestHandlerFactory`, а не от конкретной фабрики |

---

## Расширенный пример: обработка POST с телом запроса

```cpp
class EchoHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(
        Poco::Net::HTTPServerRequest& request,
        Poco::Net::HTTPServerResponse& response) override
    {
        // Читаем тело запроса
        std::istream& body_stream = request.stream();
        std::string body(std::istreambuf_iterator<char>(body_stream), {});

        // Формируем ответ
        Poco::JSON::Object json;
        json.set("method", request.getMethod());
        json.set("uri", request.getURI());
        json.set("content_type", request.getContentType());
        json.set("body", body);
        json.set("body_length", static_cast<int>(body.size()));

        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        json.stringify(out);
    }
};
```

Добавьте в фабрику:

```cpp
if (uri == "/echo") {
    return new EchoHandler;
}
```

Тест:

```bash
curl -X POST http://localhost:8080/echo \
     -H "Content-Type: application/json" \
     -d '{"name": "Alice"}'
# {"body":"{\"name\": \"Alice\"}","body_length":18,"content_type":"application/json","method":"POST","uri":"\/echo"}
```

---

## HTTPServerParams: тонкая настройка

```cpp
auto params = new Poco::Net::HTTPServerParams;

// Пул потоков
params->setMaxThreads(16);       // максимум рабочих потоков
params->setMaxQueued(256);       // максимум запросов в очереди ожидания

// Keep-Alive
params->setKeepAlive(true);
params->setKeepAliveTimeout(Poco::Timespan(10, 0)); // 10 секунд

// Таймауты
params->setTimeout(Poco::Timespan(30, 0));  // таймаут соединения 30 сек

// Имя сервера (заголовок Server:)
params->setSoftwareVersion("MyApp/1.0");
```

---

## Ключевые концепции

- **POCO** — зрелый C++ фреймворк для сетевых приложений, IoT и микросервисов.
- **HTTPServer** использует модель «пул потоков + фабрика обработчиков».
- **HTTPRequestHandler** — один экземпляр на один запрос, уничтожается после `handleRequest`.
- **HTTPRequestHandlerFactory** — классический Factory Method; маршрутизация запросов.
- **ServerApplication** — управляет жизненным циклом серверного приложения (RAII).
- **response.send()** — точка невозврата: после неё заголовки уже отправлены.

---

## Типичные ошибки

1. **Изменение заголовков после `response.send()`** — заголовки уже отправлены, изменения будут проигнорированы.
2. **Хранение состояния в обработчике между запросами** — невозможно, каждый запрос получает новый экземпляр.
3. **Забыли `loadConfiguration()`** — `config().getUInt(...)` вернёт значение по умолчанию, или упадёт с исключением.
4. **`new HandlerFactory` без понимания владения** — `HTTPServer` берёт владение и удаляет фабрику. Не удаляйте вручную.
5. **Блокирующие операции в обработчике** — занимают поток из пула, снижая пропускную способность.

---

## Вопросы для самопроверки

1. Какие три основных класса составляют архитектуру POCO HTTP-сервера?
2. Сколько раз используется один экземпляр `HTTPRequestHandler`?
3. Почему нельзя менять заголовки ответа после вызова `send()`?
4. Какой паттерн GoF реализует `HTTPRequestHandlerFactory`?
5. Что произойдёт, если все потоки пула заняты, а приходит новый запрос?
6. Зачем нужен `waitForTerminationRequest()`?
7. Как передать общее состояние (например, подключение к БД) в обработчик?

---

## Практическое задание

Создайте HTTP-сервер «Телефонная книга» (Contact Book API):

1. **GET /contacts** — вернуть список всех контактов (JSON-массив).
2. **GET /contacts?name=...** — поиск контакта по имени (подстрока).
3. **POST /contacts** — добавить контакт (JSON: `{"name": "...", "phone": "..."}`).
4. **DELETE /contacts?name=...** — удалить контакт по имени.
5. Храните контакты в `std::vector`, защищённом `std::mutex` (потокобезопасность).
6. Передайте хранилище контактов в фабрику через конструктор, а из фабрики — в каждый обработчик.
7. Добавьте логирование: каждый запрос должен выводить метод, URI и время обработки.

Соберите проект с CMake. Протестируйте все эндпоинты с помощью curl.
