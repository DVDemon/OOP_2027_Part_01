// Пример 08: перегрузка операторов * и -> (без шаблонов).
//
// *  — доступ к самому объекту (обычно возвращает ссылку);
// -> — доступ к членам объекта (возвращает указатель или объект
//      с собственным operator->).
#include <iostream>

// Объект, к которому обращаемся через указатель или прокси.
class Widget {
    int id_;
public:
    explicit Widget(int id) : id_(id) {}

    int id() const { return id_; }
    void hello() const { std::cout << "Widget #" << id_ << " says hello\n"; }
};

void print(const Widget& w) {
    std::cout << "print: Widget #" << w.id() << '\n';
}

// --- 1. Умный указатель: * возвращает ссылку, -> возвращает указатель ---
class WidgetPtr {
    Widget* ptr_;
public:
    explicit WidgetPtr(Widget* p = nullptr) : ptr_(p) {}
    ~WidgetPtr() { delete ptr_; }

    WidgetPtr(const WidgetPtr&) = delete;
    WidgetPtr& operator=(const WidgetPtr&) = delete;

    // *p — это сам объект (lvalue): можно читать и присваивать.
    Widget& operator*() { return *ptr_; }
    const Widget& operator*() const { return *ptr_; }

    // p->member раскрывается в ptr_->member.
    Widget* operator->() { return ptr_; }
    const Widget* operator->() const { return ptr_; }
};

// --- 2. operator-> может вернуть объект-прокси ---
// Компилятор повторяет -> по цепочке, пока не дойдёт до настоящего
// указателя ("drill-down").
class ArrowProxy {
    Widget* ptr_;
public:
    explicit ArrowProxy(Widget* p) : ptr_(p) {
        std::cout << "[->] доступ начат\n";
    }
    ~ArrowProxy() {
        std::cout << "[->] доступ завершён\n";
    }
    Widget* operator->() const { return ptr_; }
};

// --- 3. operator* тоже может вернуть объект-прокси ---
class DerefProxy {
    Widget& ref_;
public:
    explicit DerefProxy(Widget& r) : ref_(r) {
        std::cout << "[*] доступ начат\n";
    }
    ~DerefProxy() {
        std::cout << "[*] доступ завершён\n";
    }
    // Прокси ведёт себя как Widget: неявное преобразование в ссылку.
    operator Widget&() const { return ref_; }
};

// Владелец объекта, логирующий каждое обращение через * и ->.
class LoggingWidget {
    Widget obj_;
public:
    explicit LoggingWidget(int id) : obj_(id) {}

    // Временные прокси живут до конца полного выражения — их деструкторы
    // гарантированно сработают после использования объекта.
    DerefProxy operator*() { return DerefProxy(obj_); }
    ArrowProxy operator->() { return ArrowProxy(&obj_); }
};

int main() {
    std::cout << "=== 1. Умный указатель ===\n";
    WidgetPtr p(new Widget(1));

    p->hello();          // (p.operator->())->hello()
    (*p).hello();        // (p.operator*()).hello()

    *p = Widget(10);     // operator* вернул Widget& — можно присваивать
    std::cout << "id = " << p->id() << '\n';

    std::cout << "\n=== 2. const-перегрузки ===\n";
    const WidgetPtr cp(new Widget(3));
    cp->hello();         // выбран Widget* operator->() const
    std::cout << "id = " << (*cp).id() << '\n';

    std::cout << "\n=== 3. Прокси в operator-> ===\n";
    LoggingWidget w(2);
    // w.operator->() -> ArrowProxy -> ArrowProxy::operator->() -> Widget*
    w->hello();

    const int id = w->id();          // прокси живёт только на время вызова
    std::cout << "id = " << id << '\n';

    std::cout << "\n=== 4. Прокси в operator* ===\n";
    // DerefProxy неявно преобразуется в Widget&; его деструктор
    // сработает в конце полного выражения — после print.
    print(*w);

    return 0;
}
