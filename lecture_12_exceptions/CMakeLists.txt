cmake_minimum_required(VERSION 3.20)
project(lecture_12_exceptions LANGUAGES CXX)

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
add_lecture_example(01_error_codes_vs_exceptions  01_error_codes_vs_exceptions/main.cpp)
add_lecture_example(02_throw_try_catch            02_throw_try_catch/main.cpp)
add_lecture_example(03_exception_hierarchy        03_exception_hierarchy/main.cpp)
add_lecture_example(04_stack_unwinding            04_stack_unwinding/main.cpp)
add_lecture_example(05_safety_guarantees          05_safety_guarantees/main.cpp)
add_lecture_example(06_raii_and_exceptions        06_raii_and_exceptions/main.cpp)
add_lecture_example(07_copy_and_swap              07_copy_and_swap/main.cpp
                                                  07_copy_and_swap/buffer.cpp)
add_lecture_example(08_noexcept                   08_noexcept/main.cpp)
add_lecture_example(09_rethrow                    09_rethrow/main.cpp)
add_lecture_example(10_advanced                   10_advanced/main.cpp)

# Пример 10 использует std::thread — нужна библиотека потоков.
find_package(Threads REQUIRED)
target_link_libraries(10_advanced PRIVATE Threads::Threads)
