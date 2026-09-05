cmake_minimum_required(VERSION 3.20)
project(oop_cpp20_examples LANGUAGES CXX)

# Агрегирующая сборка всех лекций. GoogleTest есть только в lecture_01;
# по умолчанию тесты выключены, включаются:
#   cmake -S . -B build -DLECTURE_BUILD_TESTS=ON
option(LECTURE_BUILD_TESTS "Собирать примеры с модульными тестами (GoogleTest)" OFF)

# Включаем тестирование на верхнем уровне, чтобы ctest из корня агрегирующей
# сборки находил тесты lecture_01. enable_testing() внутри lecture_01 создаёт
# только её собственный CTestTestfile.cmake — без этого корневой файл не
# генерируется и ctest не спускается в поддиректории.
if(LECTURE_BUILD_TESTS)
    enable_testing()
endif()

set(LECTURES
    lecture_01_introduction
    lecture_02_operators_io
    lecture_03_pointers_references
    lecture_04_functions
    lecture_05_classes
    lecture_06_constructors_raii
    lecture_07_copy_move
    lecture_08_operators
    lecture_09_inheritance
    lecture_10_polymorphism
    lecture_11_multiple_inheritance
    lecture_12_exceptions
    lecture_13_smart_pointers
    lecture_14_solid
    lecture_15_stl
    lecture_16_poco_http
)

foreach(lecture IN LISTS LECTURES)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${lecture})
endforeach()
