cmake_minimum_required(VERSION 3.20)
project(lecture_07_copy_move LANGUAGES CXX)

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
add_lecture_example(01_shallow_copy_problem  01_shallow_copy_problem/main.cpp)
add_lecture_example(02_copy_ctor_assign      02_copy_ctor_assign/main.cpp)
add_lecture_example(03_rule_of_three         03_rule_of_three/main.cpp)
add_lecture_example(04_copy_and_swap         04_copy_and_swap/main.cpp)
add_lecture_example(05_value_categories      05_value_categories/main.cpp)
add_lecture_example(06_move_ctor_assign      06_move_ctor_assign/main.cpp)
add_lecture_example(07_move_swap_exchange    07_move_swap_exchange/main.cpp)
add_lecture_example(08_rule_of_five_zero     08_rule_of_five_zero/main.cpp)
add_lecture_example(09_noexcept_move         09_noexcept_move/main.cpp)

# Демонстрационный исполняемый файл (показывает работу класса String).
add_lecture_example(10_string_class_demo
    10_string_class/main.cpp
    10_string_class/lec_string.cpp)
target_include_directories(10_string_class_demo PRIVATE 10_string_class)
