cmake_minimum_required(VERSION 3.20)
project(lecture_14_stl LANGUAGES CXX)

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
add_lecture_example(01_containers      01_containers/main.cpp)
add_lecture_example(02_iterators       02_iterators/main.cpp)
add_lecture_example(03_array           03_array/main.cpp)
add_lecture_example(04_vector          04_vector/main.cpp)
add_lecture_example(05_list            05_list/main.cpp)
add_lecture_example(06_lambdas         06_lambdas/main.cpp)
add_lecture_example(07_generic_lambdas 07_generic_lambdas/main.cpp)
add_lecture_example(08_algorithms      08_algorithms/main.cpp)
add_lecture_example(09_map             09_map/main.cpp)
add_lecture_example(10_word_frequency  10_word_frequency/main.cpp)
