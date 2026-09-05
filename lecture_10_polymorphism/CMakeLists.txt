cmake_minimum_required(VERSION 3.20)
project(lecture_10_polymorphism LANGUAGES CXX)

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
add_lecture_example(01_polymorphism_kinds       01_polymorphism_kinds/main.cpp)
add_lecture_example(02_virtual_functions        02_virtual_functions/main.cpp)
add_lecture_example(03_vtable                    03_vtable/main.cpp)
add_lecture_example(04_override_final            04_override_final/main.cpp)
add_lecture_example(06_virtual_destructor        06_virtual_destructor/main.cpp)
add_lecture_example(07_dynamic_cast_rtti         07_dynamic_cast_rtti/main.cpp)
add_lecture_example(08_virtual_in_ctor           08_virtual_in_ctor/main.cpp)
add_lecture_example(09_devirtualization          09_devirtualization/main.cpp)

# Примеры с библиотекой (интерфейс + реализация) и демонстрацией
add_lecture_example(05_abstract_interfaces_demo
    05_abstract_interfaces/main.cpp
    05_abstract_interfaces/shapes.cpp)
target_include_directories(05_abstract_interfaces_demo
    PRIVATE 05_abstract_interfaces)

add_lecture_example(10_polymorphic_collections_demo
    10_polymorphic_collections/main.cpp
    10_polymorphic_collections/shapes.cpp)
target_include_directories(10_polymorphic_collections_demo
    PRIVATE 10_polymorphic_collections)
