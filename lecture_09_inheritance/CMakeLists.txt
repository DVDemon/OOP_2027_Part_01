cmake_minimum_required(VERSION 3.20)
project(lecture_09_inheritance LANGUAGES CXX)

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
add_lecture_example(08_final_specifier           08_final_specifier/main.cpp)

# Примеры с разделением на интерфейс/реализацию
add_lecture_example(07_private_vs_composition    07_private_vs_composition/main.cpp
                                                 07_private_vs_composition/stack.cpp)
target_include_directories(07_private_vs_composition PRIVATE 07_private_vs_composition)

add_lecture_example(09_prefer_composition        09_prefer_composition/main.cpp
                                                 09_prefer_composition/car.cpp)
target_include_directories(09_prefer_composition PRIVATE 09_prefer_composition)

add_lecture_example(10_best_practices            10_best_practices/main.cpp
                                                 10_best_practices/shapes.cpp)
target_include_directories(10_best_practices PRIVATE 10_best_practices)
