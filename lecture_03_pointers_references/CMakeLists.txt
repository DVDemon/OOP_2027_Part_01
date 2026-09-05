cmake_minimum_required(VERSION 3.20)
project(lecture_03_pointers_references LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# Режим по умолчанию — Debug. Отладочная информация (-g) ставится всегда,
# независимо от типа сборки (см. oop_flags ниже).
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Тип сборки: Debug | Release | RelWithDebInfo" FORCE)
endif()

# Опциональные санитайзеры (см. пример 07_sanitizers).
# Включение: cmake -S . -B build -DENABLE_SANITIZERS=ON
option(ENABLE_SANITIZERS "Собирать с AddressSanitizer + UndefinedBehaviorSanitizer" OFF)

# Общие флаги: предупреждения + отладочная информация для отладчика.
function(oop_flags target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /Zi)
        target_link_options(${target} PRIVATE /DEBUG)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -g)
        if(ENABLE_SANITIZERS)
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined)
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
        endif()
    endif()
endfunction()

function(add_lecture_example name)
    add_executable(${name} ${ARGN})
    oop_flags(${name})
endfunction()

# --- Примеры (по одному исполняемому файлу на поддиректорию) ---
add_lecture_example(01_memory_model            01_memory_model/main.cpp)
add_lecture_example(02_address_deref           02_address_deref/main.cpp)
add_lecture_example(03_pointers_arrays         03_pointers_arrays/main.cpp)
add_lecture_example(04_const_pointers_nullptr  04_const_pointers_nullptr/main.cpp)
add_lecture_example(05_dynamic_memory          05_dynamic_memory/main.cpp
                                               05_dynamic_memory/dynamic_array.cpp)
target_include_directories(05_dynamic_memory   PRIVATE 05_dynamic_memory)
add_lecture_example(06_memory_errors           06_memory_errors/main.cpp)
add_lecture_example(07_sanitizers              07_sanitizers/main.cpp)
add_lecture_example(08_references              08_references/main.cpp)
add_lecture_example(09_value_categories        09_value_categories/main.cpp)
add_lecture_example(10_type_casts              10_type_casts/main.cpp)
