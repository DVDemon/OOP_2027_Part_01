cmake_minimum_required(VERSION 3.20)
project(lecture_15_templates LANGUAGES CXX)

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
add_lecture_example(01_function_template_basics  01_function_template_basics/main.cpp)
add_lecture_example(02_template_argument_deduction 02_template_argument_deduction/main.cpp)
add_lecture_example(03_template_overloading      03_template_overloading/main.cpp)
add_lecture_example(04_class_template_basics     04_class_template_basics/main.cpp)
add_lecture_example(05_class_template_stack      05_class_template_stack/main.cpp)
add_lecture_example(06_template_methods          06_template_methods/main.cpp)
