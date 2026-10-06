cmake_minimum_required(VERSION 3.16)
project(latexcalc VERSION 1.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

add_library(latexcalc_core
    src/lexer.cpp
    src/parser.cpp
    src/evaluator.cpp
    src/calculator.cpp
    src/line_editor.cpp
)
target_include_directories(latexcalc_core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
if(MSVC)
    target_compile_options(latexcalc_core PRIVATE /W4)
else()
    target_compile_options(latexcalc_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(latexcalc src/main.cpp)
target_link_libraries(latexcalc PRIVATE latexcalc_core)

option(LATEXCALC_BUILD_TESTS "Build unit tests" ON)
if(LATEXCALC_BUILD_TESTS)
    enable_testing()
    add_executable(latexcalc_tests tests/test_main.cpp)
    target_link_libraries(latexcalc_tests PRIVATE latexcalc_core)
    add_test(NAME latexcalc_tests COMMAND latexcalc_tests)
endif()
