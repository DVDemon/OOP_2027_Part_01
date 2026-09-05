// Пример 09: жизненный цикл Poco::Util::ServerApplication.
//
// ТРЕБУЕТ POCO (Foundation, Net, Util). Собирается только при Poco_FOUND.
//
// Здесь нет HTTP-сервера — только демонстрация фаз жизненного цикла серверного
// приложения POCO:
//
//   Конструктор -> initialize() -> main() -> [waitForTerminationRequest()]
//                                                       |
//                                              uninitialize()
//
// Запуск с флагом --help печатает справку (опции CLI разбирает Util).
// Запуск без флагов: приложение «работает» и ждёт Ctrl+C / SIGTERM,
// затем корректно завершается через uninitialize().

#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>

#include <iostream>
#include <string>
#include <vector>

class LifecycleApp : public Poco::Util::ServerApplication {
protected:
    // Фаза 1. initialize() вызывается перед main().
    // Здесь грузят конфигурацию, настраивают логирование.
    void initialize(Poco::Util::Application& self) override {
        loadConfiguration();                 // загрузка конфигурации по умолчанию
        ServerApplication::initialize(self); // обязательно вызвать базовую реализацию
        std::cout << "[initialize] приложение инициализировано\n";
    }

    // Регистрация CLI-опций (разбираются модулем Util).
    void defineOptions(Poco::Util::OptionSet& options) override {
        ServerApplication::defineOptions(options);
        options.addOption(
            Poco::Util::Option("help", "h", "вывести справку и выйти")
                .required(false)
                .repeatable(false)
                .callback(Poco::Util::OptionCallback<LifecycleApp>(
                    this, &LifecycleApp::handleHelp)));
    }

    void handleHelp(const std::string&, const std::string&) {
        Poco::Util::HelpFormatter formatter(options());
        formatter.setCommand(commandName());
        formatter.setUsage("[options]");
        formatter.setHeader("Демонстрация жизненного цикла ServerApplication.");
        formatter.format(std::cout);
        stopOptionsProcessing();
        help_requested_ = true;
    }

    // Фаза 2. main() — основная логика приложения.
    int main(const std::vector<std::string>&) override {
        if (help_requested_) {
            return Application::EXIT_OK;  // --help: справка уже напечатана
        }

        std::cout << "[main] приложение запущено; ждём Ctrl+C (SIGINT/SIGTERM)\n";

        // В реальном сервере здесь был бы server.start().
        // waitForTerminationRequest() блокируется до сигнала завершения.
        waitForTerminationRequest();

        // В реальном сервере здесь был бы server.stop().
        std::cout << "[main] получен сигнал завершения, выходим\n";
        return Application::EXIT_OK;
    }

    // Фаза 3. uninitialize() — освобождение ресурсов после main().
    void uninitialize() override {
        std::cout << "[uninitialize] ресурсы освобождены\n";
        ServerApplication::uninitialize();
    }

private:
    bool help_requested_ = false;
};

// Макрос разворачивается в main(): создаёт LifecycleApp и вызывает run(argc, argv),
// который последовательно проходит initialize() -> main() -> uninitialize().
POCO_SERVER_MAIN(LifecycleApp)
