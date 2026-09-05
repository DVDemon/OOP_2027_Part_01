// Пример 08: что такое HTTPServerRequest и HTTPServerResponse — на пальцах.
//
// БЕЗ сети и БЕЗ POCO — чистый C++20. Мы вручную разбираем «сырой» HTTP-запрос
// и собираем «сырой» HTTP-ответ, чтобы понять, какие именно данные прячут за
// собой классы Poco::Net::HTTPServerRequest и Poco::Net::HTTPServerResponse.
//
// В POCO у вас есть готовые методы:
//   request.getMethod(), request.getURI(), request.get("Header"), request.stream()
//   response.setStatus(...), response.setContentType(...), response.send()
// Здесь мы делаем то же самое руками, чтобы было видно «изнанку».

#include <iostream>
#include <map>
#include <sstream>
#include <string>

// Разобранный запрос — упрощённый аналог HTTPServerRequest.
struct ParsedRequest {
    std::string method;                          // GET / getMethod()
    std::string uri;                             // /path  / getURI()
    std::string version;                         // HTTP/1.1
    std::map<std::string, std::string> headers;  // get("Header", "")
    std::string body;                            // stream()
};

// Разбор «сырого» HTTP-запроса. Формат:
//   <METHOD> <URI> <VERSION>\r\n
//   Header: value\r\n
//   ...\r\n
//   \r\n
//   <body>
ParsedRequest parse_request(const std::string& raw) {
    ParsedRequest req;
    std::istringstream in(raw);
    std::string line;

    // 1. Стартовая строка.
    if (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        std::istringstream start(line);
        start >> req.method >> req.uri >> req.version;
    }

    // 2. Заголовки до пустой строки.
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) {
            break;  // пустая строка отделяет заголовки от тела
        }
        const std::size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string name = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            // Убираем ведущий пробел в значении.
            if (!value.empty() && value.front() == ' ') {
                value.erase(value.begin());
            }
            req.headers[name] = value;
        }
    }

    // 3. Остаток — тело запроса.
    std::ostringstream body;
    body << in.rdbuf();
    req.body = body.str();

    return req;
}

// Сборка «сырого» HTTP-ответа — упрощённый аналог HTTPServerResponse + send().
// Важно: статус и заголовки формируются ДО тела (в POCO — до вызова send()).
std::string build_response(int status, const std::string& reason,
                           const std::string& content_type,
                           const std::string& body) {
    std::ostringstream out;
    out << "HTTP/1.1 " << status << ' ' << reason << "\r\n";
    out << "Content-Type: " << content_type << "\r\n";
    out << "Content-Length: " << body.size() << "\r\n";
    out << "\r\n";  // пустая строка: дальше тело
    out << body;
    return out.str();
}

int main() {
    const std::string raw_request =
        "POST /echo HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: 18\r\n"
        "\r\n"
        R"({"name": "Alice"})";

    std::cout << "=== Разбор запроса (аналог HTTPServerRequest) ===\n";
    const ParsedRequest req = parse_request(raw_request);
    std::cout << "method : " << req.method << '\n';   // request.getMethod()
    std::cout << "uri    : " << req.uri << '\n';      // request.getURI()
    std::cout << "version: " << req.version << '\n';
    std::cout << "headers:\n";
    for (const auto& [name, value] : req.headers) {   // request.get("Header")
        std::cout << "  " << name << " = " << value << '\n';
    }
    std::cout << "body   : " << req.body << '\n';     // request.stream()

    std::cout << "\n=== Сборка ответа (аналог HTTPServerResponse + send()) ===\n";
    const std::string body = R"({"echo": ")" + req.body + R"("})";
    const std::string response =
        build_response(200, "OK", "application/json", body);
    std::cout << response << '\n';

    return 0;
}
