cmake_minimum_required(VERSION 3.20)
project(lecture_04_functions LANGUAGES CXX)

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
add_lecture_example(01_declaration_definition  01_declaration_definition/main.cpp
                                               01_declaration_definition/math_utils.cpp)
add_lecture_example(02_parameter_passing       02_parameter_passing/main.cpp)
add_lecture_example(03_overloading             03_overloading/main.cpp)
add_lecture_example(04_overload_resolution     04_overload_resolution/main.cpp)
add_lecture_example(05_default_arguments       05_default_arguments/main.cpp)
add_lecture_example(06_inline_constexpr        06_inline_constexpr/main.cpp)
add_lecture_example(07_rvo_nrvo                07_rvo_nrvo/main.cpp)
add_lecture_example(08_function_pointers       08_function_pointers/main.cpp)
add_lecture_example(09_recursion               09_recursion/main.cpp
                                               09_recursion/recursion.cpp)
add_lecture_example(10_adl                     10_adl/main.cpp)

target_include_directories(01_declaration_definition PRIVATE 01_declaration_definition)
target_include_directories(09_recursion             PRIVATE 09_recursion)
