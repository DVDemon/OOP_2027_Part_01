// Пример 02: единицы трансляции и ODR.
//
// Программа собирается из двух единиц трансляции:
//   main.cpp        — использует функции;
//   math_utils.cpp  — определяет add().
// Заголовок math_utils.h только ОБЪЯВЛЯЕТ add() и определяет inline square().
//
// Сборка вручную (что под капотом делает CMake):
//   g++ -std=c++20 -c math_utils.cpp -o math_utils.o
//   g++ -std=c++20 -c main.cpp       -o main.o
//   g++ math_utils.o main.o -o program     # линковщик связывает символы

#include <iostream>
#include "math_utils.h"

int main() {
    std::cout << "add(2, 3)    = " << math_utils::add(2, 3) << '\n';
    std::cout << "square(5)    = " << math_utils::square(5) << '\n';
    return 0;
}
