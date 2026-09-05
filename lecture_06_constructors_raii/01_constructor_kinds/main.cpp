// Пример 01: четыре вида конструкторов.
//   - по умолчанию;
//   - параметризованный;
//   - копирующий;
//   - конвертирующий (с одним «значимым» параметром).
#include <iostream>
#include <string>

class Point {
    double x_;
    double y_;

public:
    // Конструктор по умолчанию: без аргументов.
    Point() : x_(0.0), y_(0.0) {
        std::cout << "Point() по умолчанию\n";
    }

    // Параметризованный конструктор.
    Point(double x, double y) : x_(x), y_(y) {
        std::cout << "Point(" << x_ << ", " << y_ << ") параметризованный\n";
    }

    // Копирующий конструктор: создаёт объект как копию другого.
    // Сигнатура T(const T&).
    Point(const Point& other) : x_(other.x_), y_(other.y_) {
        std::cout << "Point(const Point&) копирующий\n";
    }

    double x() const { return x_; }
    double y() const { return y_; }
};

// Конвертирующий конструктор: один значимый параметр => возможна
// неявная конверсия double -> Temperature.
class Temperature {
    double celsius_;

public:
    Temperature(double celsius) : celsius_(celsius) {
        std::cout << "Temperature(" << celsius_ << ") конвертирующий\n";
    }

    double celsius() const { return celsius_; }
};

void describe(const Temperature& t) {
    std::cout << "  температура = " << t.celsius() << " C\n";
}

int main() {
    std::cout << "--- по умолчанию ---\n";
    Point p0;        // конструктор по умолчанию
    Point p0b{};     // то же самое (value/list initialization)
    (void)p0b;

    std::cout << "--- параметризованный ---\n";
    Point p1(3.0, 4.0);
    Point p2{1.0, 2.0};  // direct-list-initialization

    std::cout << "--- копирующий ---\n";
    Point p3 = p1;   // вызов копирующего конструктора
    Point p4(p2);    // то же самое
    std::cout << "p3 = (" << p3.x() << ", " << p3.y() << ")\n";
    std::cout << "p4 = (" << p4.x() << ", " << p4.y() << ")\n";

    std::cout << "--- конвертирующий ---\n";
    describe(36.6);  // неявная конверсия double -> Temperature
    Temperature t = 20.0;  // copy-initialization тоже разрешена (без explicit)
    std::cout << "t = " << t.celsius() << " C\n";

    return 0;
}
