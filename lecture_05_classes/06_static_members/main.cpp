// Пример 06: статические поля и методы, inline static (C++17).
#include <iostream>
#include <string>

// Logger ведёт общий для всех объектов счётчик экземпляров и общий префикс.
class Logger {
    // C++17: inline static — определение прямо в заголовке/теле класса,
    // отдельная строка в .cpp не нужна (нет ODR-нарушения).
    inline static int instance_count_ = 0;
    inline static std::string prefix_ = "[LOG]";

    std::string name_;  // нестатическое поле — у каждого объекта своё

public:
    explicit Logger(std::string name) : name_(std::move(name)) {
        ++instance_count_;  // меняем общее статическое поле
    }

    ~Logger() { --instance_count_; }

    // Статический метод: нет this, нет доступа к нестатическим полям (name_).
    // Работает с классом в целом, а не с конкретным объектом.
    static int getInstanceCount() { return instance_count_; }
    static void setPrefix(const std::string& p) { prefix_ = p; }

    // Обычный метод: имеет доступ и к name_, и к статическому prefix_.
    void log(const std::string& msg) const {
        std::cout << prefix_ << " [" << name_ << "] " << msg << '\n';
    }
};

int main() {
    // Статический метод вызывается через имя класса, без объекта.
    std::cout << "экземпляров в начале: " << Logger::getInstanceCount() << '\n';

    Logger a{"net"};
    Logger b{"db"};
    std::cout << "экземпляров: " << Logger::getInstanceCount() << '\n';  // 2

    a.log("connecting");
    Logger::setPrefix("[APP]");  // меняем общий префикс для ВСЕХ объектов
    b.log("query");              // увидим новый префикс

    {
        Logger tmp{"temp"};
        std::cout << "внутри блока: " << Logger::getInstanceCount() << '\n';  // 3
    }  // tmp уничтожен -> счётчик уменьшится

    std::cout << "после блока: " << Logger::getInstanceCount() << '\n';  // 2

    return 0;
}
