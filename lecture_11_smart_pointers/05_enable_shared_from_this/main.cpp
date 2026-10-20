// Пример 05: enable_shared_from_this — безопасный shared_ptr на самого себя.
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

// Имитация очереди отложенных задач (как асинхронные операции).
std::vector<std::function<void()>> g_tasks;

void asyncOperation(std::function<void()> cb) {
    g_tasks.push_back(std::move(cb));  // «отложили» выполнение
}

class Server : public std::enable_shared_from_this<Server> {
public:
    explicit Server(int id) : id_(id) {
        std::cout << "Server(" << id_ << ") создан\n";
    }
    ~Server() {
        std::cout << "Server(" << id_ << ") разрушен\n";
    }

    void startAsync() {
        // shared_from_this() даёт shared_ptr, разделяющий тот же control block.
        // Захват self в лямбду продлевает жизнь объекта до выполнения задачи.
        auto self = shared_from_this();
        asyncOperation([self]() { self->onComplete(); });
        std::cout << "Server(" << id_ << "): задача поставлена, use_count = "
                  << self.use_count() << '\n';
    }

    void onComplete() {
        std::cout << "Server(" << id_ << "): операция завершена\n";
    }

private:
    int id_;
};

int main() {
    {
        // ОБЯЗАТЕЛЬНО создаём через shared_ptr, иначе shared_from_this()
        // бросит std::bad_weak_ptr.
        auto server = std::make_shared<Server>(1);
        server->startAsync();
        std::cout << "main: server.use_count = " << server.use_count() << '\n';
        // Локальный server вышел из области видимости, но объект жив,
        // потому что задача в g_tasks держит копию shared_ptr.
    }

    std::cout << "main: выполняем отложенные задачи\n";
    for (auto& task : g_tasks) task();
    g_tasks.clear();  // отпускаем последние shared_ptr — объект разрушится

    return 0;
}
