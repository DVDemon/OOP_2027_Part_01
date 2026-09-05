cmake_minimum_required(VERSION 3.20)
project(lecture_08_operators LANGUAGES CXX)

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
add_lecture_example(01_member_vs_free        01_member_vs_free/main.cpp)
add_lecture_example(02_arithmetic_compound   02_arithmetic_compound/main.cpp)
add_lecture_example(03_increment_decrement   03_increment_decrement/main.cpp)
add_lecture_example(04_comparison_spaceship  04_comparison_spaceship/main.cpp)
add_lecture_example(05_io_operators          05_io_operators/main.cpp)
add_lecture_example(06_subscript             06_subscript/main.cpp)
add_lecture_example(07_call_functor          07_call_functor/main.cpp)
add_lecture_example(08_arrow_proxy           08_arrow_proxy/main.cpp)
add_lecture_example(09_conversions           09_conversions/main.cpp)
add_lecture_example(10_literals_canonical    10_literals_canonical/main.cpp)
