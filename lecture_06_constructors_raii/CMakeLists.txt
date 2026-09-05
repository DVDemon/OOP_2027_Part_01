cmake_minimum_required(VERSION 3.20)
project(lecture_06_constructors_raii LANGUAGES CXX)

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
add_lecture_example(01_constructor_kinds        01_constructor_kinds/main.cpp)
add_lecture_example(02_member_init_list         02_member_init_list/main.cpp)
add_lecture_example(03_delegating_constructors  03_delegating_constructors/main.cpp)
add_lecture_example(04_explicit                 04_explicit/main.cpp)
add_lecture_example(05_default_delete           05_default_delete/main.cpp)
add_lecture_example(06_destructors_order        06_destructors_order/main.cpp)
add_lecture_example(07_raii                     07_raii/main.cpp
                                                07_raii/timer.cpp)
add_lecture_example(08_object_lifetime          08_object_lifetime/main.cpp)
add_lecture_example(09_singleton                09_singleton/main.cpp)
add_lecture_example(10_rule_of_five             10_rule_of_five/main.cpp
                                                10_rule_of_five/dynamic_array.cpp)
target_include_directories(10_rule_of_five PRIVATE 10_rule_of_five)
