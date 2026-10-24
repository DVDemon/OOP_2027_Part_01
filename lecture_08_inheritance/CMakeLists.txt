cmake_minimum_required(VERSION 3.20)
project(lecture_08_inheritance LANGUAGES CXX)

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
add_lecture_example(01_inheritance_basics        01_inheritance_basics/main.cpp)
add_lecture_example(02_access_modifiers          02_access_modifiers/main.cpp)
add_lecture_example(03_construction_order        03_construction_order/main.cpp)
add_lecture_example(04_inheriting_constructors   04_inheriting_constructors/main.cpp)
add_lecture_example(05_name_hiding               05_name_hiding/main.cpp)
add_lecture_example(06_object_slicing            06_object_slicing/main.cpp)

# Пример с разделением на интерфейс/реализацию
add_lecture_example(07_private_vs_composition    07_private_vs_composition/main.cpp
                                                 07_private_vs_composition/stack.cpp)
target_include_directories(07_private_vs_composition PRIVATE 07_private_vs_composition)

add_lecture_example(08_final_and_composition     08_final_and_composition/main.cpp)
add_lecture_example(09_multiple_inheritance      09_multiple_inheritance/main.cpp)
add_lecture_example(10_diamond_problem           10_diamond_problem/main.cpp)
add_lecture_example(11_virtual_inheritance       11_virtual_inheritance/main.cpp)

# Пример с несколькими заголовками
add_lecture_example(12_mixins_and_alternatives   12_mixins_and_alternatives/main.cpp)
target_include_directories(12_mixins_and_alternatives PRIVATE 12_mixins_and_alternatives)
