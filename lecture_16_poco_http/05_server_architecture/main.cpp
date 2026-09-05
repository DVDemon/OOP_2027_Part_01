// Пример 05: архитектура HTTP-сервера POCO в виде учебной мини-модели.
//
// БЕЗ сети и БЕЗ POCO — чистый C++20. Цель — показать связи трёх классов,
// на которых держится HTTP-сервер POCO:
//
//   HTTPServer
//       └── HTTPRequestHandlerFactory   (фабрика — создаёт обработчики)
//               └── HTTPRequestHandler  (обработчик — обрабатывает один запрос)
//
// Здесь воспроизведены те же роли и тот же поток управления, но вместо TCP
// мы просто «прогоняем» список запросов через сервер.

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ===== Упрощённые аналоги запроса и ответа POCO =====
struct Request {
    std::string method;  // GET, POST, ...
    std::string uri;     // /, /hello, ...
};

struct Response {
    int status = 200;
    std::string body;
};

// ===== Аналог Poco::Net::HTTPRequestHandler =====
// Абстрактный обработчик одного запроса. В POCO метод называется handleRequest
// и принимает HTTPServerRequest& и HTTPServerResponse&.
class RequestHandler {
public:
    virtual ~RequestHandler() = default;
    virtual void handleRequest(const Request& request, Response& response) = 0;
};

// Конкретный обработчик: отвечает приветствием (аналог HelloHandler).
class HelloHandler : public RequestHandler {
public:
    void handleRequest(const Request& request, Response& response) override {
        response.status = 200;
        response.body = "Hello, World! (" + request.method + " " + request.uri + ")";
    }
};

// Конкретный обработчик: 404 (аналог NotFoundHandler).
class NotFoundHandler : public RequestHandler {
public:
    void handleRequest(const Request& request, Response& response) override {
        response.status = 404;
        response.body = "Not Found: " + request.uri;
    }
};

// ===== Аналог Poco::Net::HTTPRequestHandlerFactory =====
// Factory Method (GoF): по запросу выбирает и создаёт нужный обработчик.
// В POCO метод называется createRequestHandler и возвращает сырой указатель,
// которым затем владеет сервер. Здесь используем unique_ptr — владение явное.
class HandlerFactory {
public:
    std::unique_ptr<RequestHandler> createRequestHandler(const Request& request) {
        if (request.uri == "/" || request.uri == "/hello") {
            return std::make_unique<HelloHandler>();
        }
        return std::make_unique<NotFoundHandler>();
    }
};

// ===== Аналог Poco::Net::HTTPServer =====
// Сервер зависит от АБСТРАКЦИИ фабрики (в POCO — от базового класса фабрики),
// а не от конкретных обработчиков. Это принцип DIP: добавление нового маршрута
// не требует менять сам сервер.
class Server {
public:
    explicit Server(HandlerFactory factory) : factory_(factory) {}

    // В реальном POCO здесь был бы цикл приёма соединений и пул потоков.
    // Мы же просто обрабатываем один «принятый» запрос.
    Response serve(const Request& request) {
        // 1. Фабрика создаёт обработчик под этот запрос.
        std::unique_ptr<RequestHandler> handler = factory_.createRequestHandler(request);
        // 2. Сервер вызывает обработчик.
        Response response;
        handler->handleRequest(request, response);
        // 3. unique_ptr уничтожает обработчик здесь — «один экземпляр на запрос».
        return response;
    }

private:
    HandlerFactory factory_;
};

int main() {
    std::cout << "Учебная мини-модель архитектуры HTTP-сервера POCO\n";
    std::cout << "HTTPServer -> Factory -> Handler (один обработчик на запрос)\n\n";

    Server server(HandlerFactory{});

    const std::vector<Request> incoming{
        {"GET", "/"},
        {"GET", "/hello"},
        {"POST", "/hello"},
        {"GET", "/api/users"},  // нет такого маршрута -> 404
    };

    for (const Request& request : incoming) {
        Response response = server.serve(request);
        std::cout << request.method << ' ' << request.uri
                  << "  ->  " << response.status << ' ' << response.body << '\n';
    }

    return 0;
}
