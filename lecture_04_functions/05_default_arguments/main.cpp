// Пример 05: аргументы по умолчанию.
#include <iostream>
#include <string>

// Аргументы по умолчанию задаются СПРАВА НАЛЕВО и только в конце списка.
// Указываются один раз (здесь — в определении; обычно — в объявлении в .h).
void log(const std::string& message,
         const std::string& level = "INFO",
         bool timestamp = true) {
    if (timestamp) {
        std::cout << "[12:00] ";
    }
    std::cout << "[" << level << "] " << message << '\n';
}

int main() {
    log("Сервер запущен");                  // level="INFO", timestamp=true
    log("Соединение потеряно", "WARNING");  // timestamp=true
    log("Отладочные данные", "DEBUG", false);  // все аргументы заданы явно

    return 0;
}
