// Пример 08: noexcept как спецификатор и как оператор.
//
//   спецификатор noexcept — обещание не бросать (нарушение -> std::terminate);
//   оператор   noexcept(expr) — проверка на этапе компиляции, бросает ли expr.
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>

// Спецификатор: обещаем не бросать исключений.
int add(int a, int b) noexcept {
    return a + b;
}

// noexcept(false) — функция может бросать (это поведение по умолчанию).
int may_throw(int x) noexcept(false) {
    if (x < 0) throw std::runtime_error("отрицательное");
    return x;
}

struct A {
    void foo() noexcept {}
    void bar() {}
};

// Условный noexcept: функция noexcept ровно тогда, когда не бросает t.work().
template <typename T>
void process(T& t) noexcept(noexcept(t.work())) {
    t.work();
}

struct SafeWorker   { void work() noexcept {} };
struct UnsafeWorker { void work() {} };

int main() {
    std::cout << "add(2, 3) = " << add(2, 3) << '\n';
    std::cout << "may_throw(5) = " << may_throw(5) << '\n';

    // Оператор noexcept: спрашиваем у компилятора, бросает ли выражение.
    std::cout << std::boolalpha;
    std::cout << "noexcept(add(1, 2))       = " << noexcept(add(1, 2)) << '\n';
    std::cout << "noexcept(may_throw(1))    = " << noexcept(may_throw(1)) << '\n';
    std::cout << "noexcept(A{}.foo())       = " << noexcept(A{}.foo()) << '\n';
    std::cout << "noexcept(A{}.bar())       = " << noexcept(A{}.bar()) << '\n';

    // Проверки на этапе компиляции.
    static_assert(noexcept(add(1, 2)));
    static_assert(!noexcept(may_throw(1)));

    // Условный noexcept «протекает» из метода work().
    SafeWorker sw;
    UnsafeWorker uw;
    static_assert(noexcept(process(sw)));
    static_assert(!noexcept(process(uw)));

    // Почему это важно для контейнеров: vector использует move при росте,
    // только если move-конструктор noexcept (иначе копирует ради строгой гарантии).
    std::cout << "move-конструктор std::vector<int> noexcept = "
              << std::is_nothrow_move_constructible_v<std::vector<int>> << '\n';

    return 0;
}
