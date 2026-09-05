cmake_minimum_required(VERSION 3.20)
project(lecture_01_introduction LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Режим по умолчанию — Debug. Отладочная информация (-g) ставится всегда,
# независимо от типа сборки (см. oop_flags ниже).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Тип сборки: Debug | Release | RelWithDebInfo" FORCE)
endif()

# Общие флаги: предупреждения + отладочная информация для отладчика.
function(oop_flags target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /Zi)
        target_link_options(${target} PRIVATE /DEBUG)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -g)
    endif()
endfunction()

function(add_lecture_example name)
    add_executable(${name} ${ARGN})
    oop_flags(${name})
endfunction()

# --- Примеры (по одному исполняемому файлу на поддиректорию) ---
add_lecture_example(01_first_program        01_first_program/main.cpp)
add_lecture_example(02_translation_units    02_translation_units/main.cpp
                                            02_translation_units/math_utils.cpp)
add_lecture_example(04_basic_types          04_basic_types/main.cpp)
add_lecture_example(05_initialization       05_initialization/main.cpp)
add_lecture_example(06_const_constexpr      06_const_constexpr/main.cpp)
add_lecture_example(07_auto                 07_auto/main.cpp)
add_lecture_example(08_namespaces           08_namespaces/main.cpp)

# Демонстрационный калькулятор (знакомство с GoogleTest). Собирается всегда.
add_lecture_example(03_gtest_calculator_demo
    03_gtest_calculator/main.cpp
    03_gtest_calculator/calculator.cpp)
target_include_directories(03_gtest_calculator_demo PRIVATE 03_gtest_calculator)

include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # Windows: не менять CRT
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)

function(add_lecture_test name source_dir)
    add_executable(${name} ${source_dir}/tests.cpp ${ARGN})
    oop_flags(${name})
    target_include_directories(${name} PRIVATE ${source_dir})
    target_link_libraries(${name} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${name})
endfunction()

add_lecture_test(03_gtest_calculator_tests 03_gtest_calculator
                    03_gtest_calculator/calculator.cpp)

