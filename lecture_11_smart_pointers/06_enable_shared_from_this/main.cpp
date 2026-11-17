// Пример 06: enable_shared_from_this — безопасный shared_ptr на самого себя.
//
// Объект получает shared_ptr на себя, чтобы отложенная задача могла держать
// его живым до своего выполнения: задача — отдельный класс, который хранит
// shared_ptr на сервер и вызывает нужный метод в run().
#include <iostream>
#include <memory>
#include <utility>
#include <vector>

// --- Отложенная задача: интерфейс и очередь ---------------------------------
class Task {
public:
    virtual ~Task() = default;
    virtual void run() = 0;
};

// Имитация очереди асинхронных операций: задачи выполняются позже.
class TaskQueue {
public:
    void push(std::unique_ptr<Task> task) { tasks_.push_back(std::move(task)); }

    std::size_t size() const { return tasks_.size(); }

    void runAll() {
        for (auto& task : tasks_) {
            task->run();
        }
        tasks_.clear();  // последние shared_ptr отпускаются здесь
    }

private:
    std::vector<std::unique_ptr<Task>> tasks_;
};

// --- Сервер ------------------------------------------------------------------
class Server : public std::enable_shared_from_this<Server> {
public:
    explicit Server(int id) : id_(id) {
        ++alive;
        std::cout << "Server(" << id_ << ") создан\n";
    }
    ~Server() {
        --alive;
        std::cout << "Server(" << id_ << ") разрушен\n";
    }

    // Задачу делает отдельный класс CompleteTask: он держит shared_ptr
    // на сервер и потому продлевает его жизнь до выполнения задачи.
    void startAsync(TaskQueue& queue) {
        // Тот же control block, что у владельца: счётчик становится 2.
        queue.push(std::make_unique<CompleteTask>(shared_from_this()));
        std::cout << "Server(" << id_ << "): задача поставлена\n";
    }

    void onComplete() {
        std::cout << "Server(" << id_ << "): операция завершена\n";
    }

    static int alive;  // сколько объектов живо прямо сейчас

private:
    // Задача «завершить операцию». Держит shared_ptr на сервер, поэтому
    // сервер не разрушится, пока задача не выполнена и не уничтожена.
    class CompleteTask : public Task {
    public:
        explicit CompleteTask(std::shared_ptr<Server> server)
            : server_(std::move(server)) {}

        void run() override { server_->onComplete(); }

    private:
        std::shared_ptr<Server> server_;
    };

    int id_;
};

int Server::alive = 0;

int main() {
    TaskQueue queue;

    {
        // ОБЯЗАТЕЛЬНО создаём через shared_ptr, иначе shared_from_this()
        // бросит std::bad_weak_ptr.
        auto server = std::make_shared<Server>(1);
        server->startAsync(queue);
        std::cout << "main: use_count(server) = " << server.use_count()
                  << " (локальная ссылка + задача в очереди)" << '\n';
    }  // локальный shared_ptr уничтожен, но объект жив: его держит задача

    std::cout << "main: локальной ссылки нет, живых Server = " << Server::alive
              << ", задач в очереди = " << queue.size() << '\n';

    std::cout << "main: выполняем отложенные задачи\n";
    queue.runAll();  // задача выполняется, затем отпускает shared_ptr
    std::cout << "main: очередь пуста, живых Server = " << Server::alive << '\n';

    std::cout << "\n--- ловушка: объект не во владении shared_ptr ---\n";
    Server local(2);  // объект на стеке, shared_ptr его не держит
    try {
        local.shared_from_this();  // управляющего shared_ptr нет
    } catch (const std::bad_weak_ptr&) {
        std::cout << "main: shared_from_this() бросил std::bad_weak_ptr\n";
    }

    return 0;
}
