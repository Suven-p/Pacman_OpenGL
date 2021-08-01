cmake_minimum_required(VERSION 3.20)

project(Glad LANGUAGES C)

add_library(glad
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c

    )
target_include_directories(glad PUBLIC include)