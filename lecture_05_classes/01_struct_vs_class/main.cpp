// Пример 01: единственное отличие struct от class — доступ по умолчанию.
#include <iostream>

// struct: члены по умолчанию public — удобно для простых агрегатов данных (POD).
struct Point {
    double x;  // public по умолчанию
    double y;  // public по умолчанию
};

// class: члены по умолчанию private — поля скрыты, доступ только через методы.
class PointClass {
    double x;  // private по умолчанию
    double y;  // private по умолчанию
public:
    PointClass(double x_, double y_) : x(x_), y(y_) {}
    double getX() const { return x; }
    double getY() const { return y; }
};

// Доказательство, что отличие ТОЛЬКО в доступе по умолчанию:
// в struct можно явно сделать private, в class — явно public.
struct Counter {       // struct, но с инвариантом
private:               // явно закрываем поле
    int value_ = 0;
public:
    void inc() { ++value_; }
    int get() const { return value_; }
};

int main() {
    // struct Point: к полям обращаемся напрямую.
    Point p{3.0, 4.0};
    std::cout << "Point (struct): x=" << p.x << " y=" << p.y << '\n';

    // class PointClass: поля закрыты, читаем через геттеры.
    PointClass pc{3.0, 4.0};
    std::cout << "PointClass (class): x=" << pc.getX() << " y=" << pc.getY() << '\n';
    // pc.x = 1.0;  // ОШИБКА компиляции: 'x' is private

    // struct с явным private ведёт себя как class.
    Counter c;
    c.inc();
    c.inc();
    std::cout << "Counter (struct + private): " << c.get() << '\n';

    return 0;
}
