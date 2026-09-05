// Пример 05: const-методы и mutable.
#include <iostream>
#include <string>

// Класс с дорогим вычислением и кэшем. determinant() логически не меняет
// матрицу (это const-метод), но кэширует результат через mutable-поля.
class Matrix2x2 {
    double a_, b_, c_, d_;  // [a b; c d]

    // mutable: разрешено менять даже из const-метода.
    mutable double cached_det_ = 0.0;
    mutable bool det_valid_ = false;
    mutable int compute_count_ = 0;  // сколько раз реально считали определитель

public:
    Matrix2x2(double a, double b, double c, double d) : a_(a), b_(b), c_(c), d_(d) {}

    // const-метод: this имеет тип const Matrix2x2*. Наблюдаемое состояние
    // (значения матрицы) не меняется, поэтому метод корректно помечен const.
    double determinant() const {
        if (!det_valid_) {
            cached_det_ = a_ * d_ - b_ * c_;  // OK: cached_det_ объявлено mutable
            det_valid_ = true;
            ++compute_count_;
        }
        return cached_det_;
    }

    // Неконстантный метод: меняет состояние и инвалидирует кэш.
    void setA(double a) {
        a_ = a;
        det_valid_ = false;  // кэш устарел
    }

    int computeCount() const { return compute_count_; }
};

// const-ссылка может вызывать ТОЛЬКО const-методы.
void printDet(const Matrix2x2& m) {
    std::cout << "det = " << m.determinant() << '\n';  // determinant() — const, OK
    // m.setA(0);  // ОШИБКА: setA не const, через const& недоступен
}

int main() {
    Matrix2x2 m{1.0, 2.0, 3.0, 4.0};  // det = 1*4 - 2*3 = -2

    printDet(m);  // первый вызов реально считает
    printDet(m);  // второй вызов берёт из кэша
    printDet(m);  // тоже из кэша

    std::cout << "реальных вычислений det: " << m.computeCount() << '\n';  // ожидаем 1

    m.setA(5.0);  // det = 5*4 - 2*3 = 14, кэш инвалидирован
    printDet(m);
    std::cout << "реальных вычислений det: " << m.computeCount() << '\n';  // ожидаем 2

    return 0;
}
