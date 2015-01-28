cmake_minimum_required(VERSION 2.8)

project(romberg)
set(EXECUTABLE_NAME ${PROJECT_NAME})

set(SOURCES
    main.cpp
    math_parser.h
    math_parser.cpp
)

add_definitions(-D_USE_MATH_DEFINES)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(${EXECUTABLE_NAME} ${SOURCES})
