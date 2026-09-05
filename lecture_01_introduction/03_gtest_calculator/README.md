# 03. CMake + Google Test: проект «Калькулятор»

**Пункты плана:** 6 — система сборки CMake; 7 — Google Test.

## Что показывает пример

* Разделение на интерфейс (`calculator.h`), реализацию (`calculator.cpp`),
  демо (`main.cpp`) и тесты (`tests.cpp`).
* Подключение Google Test через `FetchContent` (см. `../CMakeLists.txt`).
* Регистрацию тестов в CTest через `gtest_discover_tests`.
* Проверку `constexpr`-функции через `static_assert`.

## Сборка и запуск

```bash
cmake -S .. -B ../build       # из папки лекции
cmake --build ../build
./../build/03_gtest_calculator_demo
cd ../build && ctest --output-on-failure
```

Цели:

* `03_gtest_calculator_demo` — демонстрация в консоли;
* `03_gtest_calculator_tests` — тесты Google Test.

## Подключение Google Test (фрагмент `CMakeLists.txt`)

```cmake
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
)
FetchContent_MakeAvailable(googletest)

add_executable(tests tests.cpp calculator.cpp)
target_link_libraries(tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
```

## Плохо → Хорошо

**Плохо** — проверять логику «на глаз», запуская `main` и читая вывод:

```cpp
std::cout << add(2, 3) << '\n';   // 5? проверяю глазами при каждом запуске
```

**Хорошо** — автоматические тесты, которые падают при регрессии:

```cpp
TEST(CalculatorTest, Add) {
    EXPECT_EQ(calc::add(2, 3), 5);
}
```

## Макросы Google Test в примере

| Макрос | Назначение |
|--------|------------|
| `EXPECT_EQ(a, b)` | равенство (тест продолжается при провале) |
| `EXPECT_DOUBLE_EQ(a, b)` | сравнение `double` с допуском |
| `EXPECT_TRUE(cond)` | условие истинно |
| `static_assert(...)` | проверка на этапе компиляции (для `constexpr`) |
