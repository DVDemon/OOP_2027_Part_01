cmake_minimum_required(VERSION 3.20)
project(lecture_14_solid LANGUAGES CXX)

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
add_lecture_example(01_coupling_cohesion 01_coupling_cohesion/main.cpp)
add_lecture_example(02_pimpl              02_pimpl/main.cpp
                                          02_pimpl/widget.cpp)
add_lecture_example(03_tell_dont_ask      03_tell_dont_ask/main.cpp)
add_lecture_example(04_cqrs               04_cqrs/main.cpp)
add_lecture_example(05_srp                05_srp/main.cpp)
add_lecture_example(06_ocp                06_ocp/main.cpp)
add_lecture_example(07_lsp                07_lsp/main.cpp)
add_lecture_example(08_isp                08_isp/main.cpp)
add_lecture_example(09_dip                09_dip/main.cpp)
add_lecture_example(10_summary            10_summary/main.cpp)
