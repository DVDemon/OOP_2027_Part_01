cmake_minimum_required(VERSION 3.20)
project(lecture_11_multiple_inheritance LANGUAGES CXX)

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
add_lecture_example(01_basic_syntax         01_basic_syntax/main.cpp)
add_lecture_example(02_diamond_problem      02_diamond_problem/main.cpp)
add_lecture_example(03_virtual_inheritance  03_virtual_inheritance/main.cpp)
add_lecture_example(04_most_derived_ctor    04_most_derived_ctor/main.cpp)
add_lecture_example(05_object_layout        05_object_layout/main.cpp)
add_lecture_example(06_pointer_adjustment   06_pointer_adjustment/main.cpp)
add_lecture_example(08_alternatives         08_alternatives/main.cpp)
add_lecture_example(09_when_justified       09_when_justified/main.cpp)
add_lecture_example(10_recommendations      10_recommendations/main.cpp)

# Демонстрационный исполняемый файл примера про примеси (mixins/CRTP).
add_lecture_example(07_mixins_demo
    07_mixins/main.cpp)
target_include_directories(07_mixins_demo
    PRIVATE 07_mixins)
