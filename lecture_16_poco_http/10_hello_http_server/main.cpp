// Пример 10: полноценный Hello World HTTP-сервер на POCO.
//
// ТРЕБУЕТ POCO (Foundation, Net, Util, JSON). Собирается только при Poco_FOUND.
//
// Объединяет всё из лекции: ServerApplication (жизненный цикл), HTTPServer +
// фабрика обработчиков (Factory Method) + обработчики (один на запрос),
// HTTPServerRequest/Response и JSON-ответы.
//
// Маршруты:
//   GET|POST /        -> приветствие (JSON)
//   GET|POST /hello   -> приветствие (JSON)
//   POST     /echo    -> эхо тела запроса (JSON)
//   всё прочее        -> 404 (JSON)

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
#include <iterator>
#include <string>
#include <vector>

// ===== Обработчик приветствия (один экземпляр на один запрос) =====
class HelloHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override {
        std::cout << request.getMethod() << ' ' << request.getURI()
                  << " from " << request.clientAddress().toString() << '\n';

        Poco::JSON::Object json;
        json.set("message", "Hello, World!");
        json.set("method", request.getMethod());
        json.set("uri", request.getURI());

        // Статус и заголовки — ДО send().
        response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
        response.setContentType("application/json");

        std::ostream& out = response.send();  // точка невозврата
        json.stringify(out);
    }
};

// ===== Эхо-обработчик: читает тело запроса =====
class EchoHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override {
        // Тело запроса доступно как входной поток.
        std::istream& body_stream = request.stream();
        const std::string body(std::istreambuf_iterator<char>(body_stream), {});

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

// ===== Обработчик 404 =====
class NotFoundHandler : public Poco::Net::HTTPRequestHandler {
public:
    void handleRequest(Poco::Net::HTTPServerRequest& request,
                       Poco::Net::HTTPServerResponse& response) override {
        Poco::JSON::Object json;
        json.set("error", "Not Found");
        json.set("path", request.getURI());

        response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        json.stringify(out);
    }
};

// ===== Фабрика обработчиков (Factory Method, GoF) =====
// Маршрутизация по URI: возвращает обработчик, сервер берёт владение и удалит его.
class HandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
public:
    Poco::Net::HTTPRequestHandler* createRequestHandler(
        const Poco::Net::HTTPServerRequest& request) override {
        const std::string& uri = request.getURI();
        if (uri == "/" || uri == "/hello") {
            return new HelloHandler;
        }
        if (uri == "/echo") {
            return new EchoHandler;
        }
        return new NotFoundHandler;
    }
};

// ===== Серверное приложение =====
class HelloServerApp : public Poco::Util::ServerApplication {
protected:
    void initialize(Poco::Util::Application& self) override {
        loadConfiguration();
        ServerApplication::initialize(self);
    }

    int main(const std::vector<std::string>&) override {
        // Порт из конфигурации (http.port) или 8080 по умолчанию.
        const auto port = static_cast<unsigned short>(
            config().getUInt("http.port", 8080));

        // Настройки пула потоков (сервер берёт владение объектом params).
        auto* params = new Poco::Net::HTTPServerParams;
        params->setMaxQueued(100);
        params->setMaxThreads(4);

        Poco::Net::ServerSocket socket(port);
        Poco::Net::HTTPServer server(new HandlerFactory, socket, params);

        server.start();
        std::cout << "HTTP server running on http://localhost:" << port << '\n';
        std::cout << "Press Ctrl+C to stop." << '\n';

        waitForTerminationRequest();  // блокируется до Ctrl+C / SIGTERM

        server.stop();
        return Application::EXIT_OK;
    }
};

POCO_SERVER_MAIN(HelloServerApp)
