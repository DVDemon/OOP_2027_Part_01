cmake_minimum_required(VERSION 3.20)
project(lecture_13_smart_pointers LANGUAGES CXX)

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
add_lecture_example(01_motivation               01_motivation/main.cpp)
add_lecture_example(02_unique_ptr               02_unique_ptr/main.cpp)
add_lecture_example(03_shared_ptr               03_shared_ptr/main.cpp)
add_lecture_example(04_weak_ptr                 04_weak_ptr/main.cpp)
add_lecture_example(05_enable_shared_from_this  05_enable_shared_from_this/main.cpp)
add_lecture_example(06_pointer_cast             06_pointer_cast/main.cpp)
add_lecture_example(07_custom_unique_ptr        07_custom_unique_ptr/main.cpp)
add_lecture_example(08_custom_shared_ptr        08_custom_shared_ptr/main.cpp)
add_lecture_example(09_polymorphism             09_polymorphism/main.cpp)
add_lecture_example(10_best_practices_pointers  10_best_practices_pointers/main.cpp)
