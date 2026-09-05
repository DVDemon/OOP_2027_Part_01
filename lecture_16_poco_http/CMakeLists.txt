cmake_minimum_required(VERSION 3.20)
project(lecture_16_poco_http LANGUAGES CXX)

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

# --- Примеры на чистом C++ (НЕ требуют POCO) ---
# Эти цели собираются всегда, даже если POCO не установлен.
add_lecture_example(05_server_architecture  05_server_architecture/main.cpp)
add_lecture_example(08_request_response      08_request_response/main.cpp)

# --- Примеры, требующие POCO ---
# Ищем POCO. Если найден — добавляем сетевые примеры; иначе пропускаем,
# чтобы лекция собиралась и без установленной библиотеки.
find_package(Poco QUIET COMPONENTS Foundation Net Util JSON)

if(Poco_FOUND)
    message(STATUS "POCO найден — собираем примеры HTTP-сервера")

    add_lecture_example(09_server_application 09_server_application/main.cpp)
    target_link_libraries(09_server_application PRIVATE
        Poco::Foundation
        Poco::Net
        Poco::Util)

    add_lecture_example(10_hello_http_server 10_hello_http_server/main.cpp)
    target_link_libraries(10_hello_http_server PRIVATE
        Poco::Foundation
        Poco::Net
        Poco::Util
        Poco::JSON)
else()
    message(STATUS "POCO не найден — примеры HTTP-сервера пропущены "
                   "(установите: brew install poco)")
endif()
