# 04. Интеграция с CMake (find_package)

**Пункт плана:** 4 — интеграция с CMake.

## Что показывает раздел

Обзорный материал без кода (сборка не требуется). Как подключить POCO к проекту и
как сделать так, чтобы проект собирался **даже без установленного POCO**.

## Базовый вариант

```cmake
cmake_minimum_required(VERSION 3.20)
project(hello_http_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

add_executable(server main.cpp)
target_link_libraries(server PRIVATE
    Poco::Foundation Poco::Net Poco::Util Poco::JSON)
```

`find_package(Poco REQUIRED ...)` остановит конфигурацию, если POCO не найден.

## Условный вариант (как в этой лекции)

Чтобы лекция собиралась и без POCO, используем `QUIET` и проверяем `Poco_FOUND`:

```cmake
find_package(Poco QUIET COMPONENTS Foundation Net Util JSON)

if(Poco_FOUND)
    message(STATUS "POCO найден — собираем примеры HTTP-сервера")
    add_executable(server main.cpp)
    target_link_libraries(server PRIVATE
        Poco::Foundation Poco::Net Poco::Util Poco::JSON)
else()
    message(STATUS "POCO не найден — примеры HTTP-сервера пропущены")
endif()
```

Корневой [`CMakeLists.txt`](../CMakeLists.txt) лекции построен именно так.

## Плохо → Хорошо

**Плохо** — жёсткий `REQUIRED` в учебном репозитории: проект не конфигурируется
ни у кого без POCO, и не-сетевые примеры тоже становятся недоступны.

```cmake
find_package(Poco REQUIRED COMPONENTS Net)   # упадёт без POCO
```

**Хорошо** — `QUIET` + `if(Poco_FOUND)`: сетевые цели опциональны, остальные
примеры собираются всегда.

```cmake
find_package(Poco QUIET COMPONENTS Net)
if(Poco_FOUND)
    # ... сетевые цели
endif()
```

## Как CMake находит POCO

`find_package(Poco ...)` ищет файл `PocoConfig.cmake`, который ставится вместе с
библиотекой (brew/apt/vcpkg/install). Если он лежит в нестандартном месте, путь
передают через `-DPoco_DIR=...` (см. раздел [`03_installation`](../03_installation)).
