cmake_minimum_required(VERSION 3.5)
project(MathInterpreterTest VERSION 0.1.0 LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 17)



include_directories(include)
set(
    _sources
        include/Token.hpp
        include/Tokenizer.hpp
        include/Parser.hpp
        include/Evaluator.hpp
        include/Interpreter.hpp

        source/Token.cpp
        source/Tokenizer.cpp
        source/Parser.cpp
        source/Evaluator.cpp
    )

add_executable(
    main
        ${_sources}
        source/main.cpp
    )

# TESTING

find_package(GTest REQUIRED)
include(GoogleTest)
enable_testing()

add_executable(
    interpreter_tests
        ${_sources}        
        test/interpreter_tests.cc
    )

target_link_libraries(
    interpreter_tests
        GTest::GTest
        GTest::Main
    )

gtest_discover_tests(interpreter_tests)




