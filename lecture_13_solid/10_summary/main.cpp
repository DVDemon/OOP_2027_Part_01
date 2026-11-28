// Пример 10: итоги — как принципы SOLID работают вместе.
//
// Небольшая система уведомлений, в которой одновременно задействованы все
// пять принципов. В комментариях помечено, где именно проявляется каждый.

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// SRP: форматирование сообщения — отдельная ответственность.
// ISP: узкий интерфейс ровно с одним методом.
class IMessageFormatter {
public:
    virtual ~IMessageFormatter() = default;
    virtual std::string format(const std::string& text) const = 0;
};

class PlainTextFormatter : public IMessageFormatter {
public:
    std::string format(const std::string& text) const override { return text; }
};

class HtmlFormatter : public IMessageFormatter {
public:
    std::string format(const std::string& text) const override {
        return "<p>" + text + "</p>";
    }
};

// SRP/ISP: отправка сообщения — отдельная узкая роль.
class INotificationSender {
public:
    virtual ~INotificationSender() = default;
    virtual void send(const std::string& message) = 0;
};

// LSP: каждый канал честно выполняет контракт send().
class EmailSender : public INotificationSender {
public:
    void send(const std::string& message) override {
        std::cout << "[Email] " << message << '\n';
    }
};

class SmsSender : public INotificationSender {
public:
    void send(const std::string& message) override {
        std::cout << "[SMS] " << message << '\n';
    }
};

// OCP: новый канал добавляется без изменения NotificationService.
class TelegramSender : public INotificationSender {
public:
    void send(const std::string& message) override {
        std::cout << "[Telegram] " << message << '\n';
    }
};

// DIP: сервис зависит от абстракций (форматтер + список отправителей),
// а не от конкретных каналов.
class NotificationService {
    std::unique_ptr<IMessageFormatter> formatter_;
    std::vector<std::unique_ptr<INotificationSender>> senders_;

public:
    explicit NotificationService(std::unique_ptr<IMessageFormatter> fmt)
        : formatter_(std::move(fmt)) {}

    void add_sender(std::unique_ptr<INotificationSender> sender) {
        senders_.push_back(std::move(sender));
    }

    // TDA: говорим сервису «разошли», а не вытаскиваем каналы наружу.
    void notify(const std::string& text) const {
        const std::string message = formatter_->format(text);
        for (const auto& s : senders_) {
            s->send(message);
        }
    }
};

int main() {
    // Composition Root: собираем зависимости в одном месте.
    NotificationService service(std::make_unique<HtmlFormatter>());
    service.add_sender(std::make_unique<EmailSender>());
    service.add_sender(std::make_unique<SmsSender>());
    service.add_sender(std::make_unique<TelegramSender>());  // OCP: добавили канал

    service.notify("Заказ отгружен");

    return 0;
}
