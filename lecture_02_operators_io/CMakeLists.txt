cmake_minimum_required(VERSION 3.20)
project(lecture_02_operators_io LANGUAGES CXX)

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
add_lecture_example(01_arithmetic               01_arithmetic/main.cpp
                                                01_arithmetic/safe_arith.cpp)
add_lecture_example(02_bitwise                  02_bitwise/main.cpp)
add_lecture_example(03_logical_comparison       03_logical_comparison/main.cpp)
add_lecture_example(04_precedence_increment     04_precedence_increment/main.cpp)
add_lecture_example(05_compound_numeric_limits  05_compound_numeric_limits/main.cpp)
add_lecture_example(06_cmath                    06_cmath/main.cpp
                                                06_cmath/float_compare.cpp)
add_lecture_example(07_stream_io                07_stream_io/main.cpp)
add_lecture_example(08_conditionals             08_conditionals/main.cpp)
add_lecture_example(09_loops                    09_loops/main.cpp)
add_lecture_example(10_break_continue_comma     10_break_continue_comma/main.cpp)
add_lecture_example(11_type_conversions         11_type_conversions/main.cpp)
