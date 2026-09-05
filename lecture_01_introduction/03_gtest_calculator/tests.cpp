// Модульные тесты для калькулятора на Google Test.
#include <gtest/gtest.h>

#include <cmath>

#include "calculator.h"

TEST(CalculatorTest, Add) {
    EXPECT_EQ(calc::add(2, 3), 5);
    EXPECT_EQ(calc::add(-1, 1), 0);
    EXPECT_EQ(calc::add(0, 0), 0);
}

TEST(CalculatorTest, Subtract) {
    EXPECT_EQ(calc::subtract(5, 3), 2);
    EXPECT_EQ(calc::subtract(0, 7), -7);
}

TEST(CalculatorTest, Divide) {
    EXPECT_DOUBLE_EQ(calc::divide(10.0, 4.0), 2.5);
    EXPECT_TRUE(std::isinf(calc::divide(1.0, 0.0)));  // деление на ноль -> inf
}

TEST(CalculatorTest, Factorial) {
    EXPECT_EQ(calc::factorial(0), 1);
    EXPECT_EQ(calc::factorial(5), 120);
}

// factorial — constexpr: проверяем вычисление на этапе компиляции.
static_assert(calc::factorial(5) == 120, "factorial(5) должно быть 120");
