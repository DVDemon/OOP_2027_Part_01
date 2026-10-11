// Пример 01: единственное отличие struct от class — доступ по умолчанию.
#include <iostream>

// struct: члены по умолчанию public — удобно для простых агрегатов данных (POD).
struct Point {
    double x;  // public по умолчанию
    double y;  // public по умолчанию
};

// class: члены по умолчанию private — поля скрыты, доступ только через методы.
class PointClass {

    double x{0};  // private по умолчанию
    double y{0};  // private по умолчанию
    void foo() {}
public:
    PointClass(double x_, double y_) : x(x_), y(y_) {}
    double getX() const{
        return x; 
    }

    double getY() const { return y; }

    void boo() {
        foo();
    }
};

// Доказательство, что отличие ТОЛЬКО в доступе по умолчанию:
// в struct можно явно сделать private, в class — явно public.
struct Counter {       // struct, но с инвариантом
private:               // явно закрываем поле
    int value_ = 0;
public:
    void inc() { ++value_; }
    const int& get() { return value_; }
};

int main() {
    // struct Point: к полям обращаемся напрямую.
    // const Point p{3.0, 4.0};
    // std::cout << "Point (struct): x=" << p.x << " y=" << p.y << '\n';

    // // class PointClass: поля закрыты, читаем через геттеры.
    // const PointClass pc{3.0, 4.0};
    // std::cout << "PointClass (class): x=" << pc.getX() << " y=" << pc.getY() << '\n';
    // std::cout << "Sizeof p:" << sizeof(pc) << '\n';
    // // pc.x = 1.0;  // ОШИБКА компиляции: 'x' is private

    // struct с явным private ведёт себя как class.
    Counter c;
    std::cout << "Counter (struct + private): " << c.get() << '\n';

    return 0;
}
