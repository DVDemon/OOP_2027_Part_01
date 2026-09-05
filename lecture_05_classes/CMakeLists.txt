cmake_minimum_required(VERSION 3.20)
project(lecture_05_classes LANGUAGES CXX)

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
add_lecture_example(01_struct_vs_class       01_struct_vs_class/main.cpp)
add_lecture_example(02_fields_methods        02_fields_methods/main.cpp
                                             02_fields_methods/point.cpp)
target_include_directories(02_fields_methods PRIVATE 02_fields_methods)
add_lecture_example(03_access_modifiers      03_access_modifiers/main.cpp)
add_lecture_example(04_this_fluent           04_this_fluent/main.cpp)
add_lecture_example(05_const_mutable         05_const_mutable/main.cpp)
add_lecture_example(06_static_members        06_static_members/main.cpp)
add_lecture_example(07_friend                07_friend/main.cpp)
add_lecture_example(08_nested_enum           08_nested_enum/main.cpp)
add_lecture_example(09_sizeof_padding        09_sizeof_padding/main.cpp)
add_lecture_example(10_encapsulation_pimpl   10_encapsulation_pimpl/main.cpp
                                             10_encapsulation_pimpl/widget.cpp)
target_include_directories(10_encapsulation_pimpl PRIVATE 10_encapsulation_pimpl)
