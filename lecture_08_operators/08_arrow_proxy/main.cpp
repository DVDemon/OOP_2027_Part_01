// Пример 08: оператор -> и proxy-объекты.
#include <iostream>
#include <utility>

// Объект, к которому будем обращаться через ->.
struct Widget {
    int id;
    void hello() const { std::cout << "Widget #" << id << " says hello\n"; }
};

// --- Простой умный указатель: operator-> возвращает T* ---
template <typename T>
class SmartPtr {
    T* ptr_;
public:
    explicit SmartPtr(T* p = nullptr) : ptr_(p) {}
    ~SmartPtr() { delete ptr_; }

    SmartPtr(const SmartPtr&) = delete;
    SmartPtr& operator=(const SmartPtr&) = delete;

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }   // p->member раскрывается в ptr_->member
};

// --- Proxy для логирования: operator-> возвращает объект-прокси ---
// Компилятор повторяет ->, пока не дойдёт до настоящего указателя ("drill-down").
template <typename T>
class LoggingProxy {
    T* ptr_;
public:
    explicit LoggingProxy(T* p) : ptr_(p) {
        std::cout << "[proxy] доступ начат\n";
    }
    ~LoggingProxy() {
        std::cout << "[proxy] доступ завершён\n";
    }
    T* operator->() const { return ptr_; }
};

template <typename T>
class LoggingWrapper {
    T obj_;
public:
    template <typename... Args>
    explicit LoggingWrapper(Args&&... args) : obj_(std::forward<Args>(args)...) {}

    // Возвращает временный прокси: его деструктор сработает после вызова метода.
    LoggingProxy<T> operator->() { return LoggingProxy<T>(&obj_); }
};

int main() {
    SmartPtr<Widget> p(new Widget{1});
    p->hello();        // ptr_->hello()

    std::cout << "---\n";

    LoggingWrapper<Widget> w(Widget{2});
    // w.operator->() -> LoggingProxy -> .operator->() -> Widget* -> hello()
    w->hello();        // вокруг вызова печатаются "доступ начат/завершён"

    return 0;
}
