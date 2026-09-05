// Пример 06: оператор индексирования [] (const / non-const).
#include <iostream>

#include "matrix.h"

// Принимаем по const-ссылке: внутри доступна только const-версия [].
void print(const Matrix& m) {
    for (std::size_t i = 0; i < m.rows(); ++i) {
        for (std::size_t j = 0; j < m.cols(); ++j) {
            std::cout << m[i][j] << '\t';   // const operator[] -> ConstRow
        }
        std::cout << '\n';
    }
}

int main() {
    Matrix m(2, 3);

    // non-const operator[] -> Row -> запись.
    m[0][0] = 1.0;
    m[0][2] = 2.0;
    m[1][1] = 3.0;

    std::cout << "Матрица:\n";
    print(m);   // здесь работает const-версия

    std::cout << "m[1][1] = " << m[1][1] << '\n';

    return 0;
}
