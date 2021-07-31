cmake_minimum_required(VERSION 3.20)

project(Pacman VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

configure_file(
    ${CMAKE_SOURCE_DIR}/include/project/CMakeConfig.h.in
    ${CMAKE_SOURCE_DIR}/include/project/CMakeConfig.h
    @ONLY
)

file(
    GENERATE
    OUTPUT ${CMAKE_SOURCE_DIR}/include/project/CMakeConfig.h
    INPUT ${CMAKE_SOURCE_DIR}/include/project/CMakeConfig.h
    TARGET ${PROJECT_NAME}
)

set(${PROJECT_NAME}_SOURCES
    src/game.cpp
    src/ghost.cpp
    src/glad.c
    src/helpers.cpp
    src/main.cpp
    src/map.cpp
    src/pacman.cpp
    src/pellet.cpp
    src/resourceManager.cpp
    src/shader.cpp
    src/sprite.cpp
    src/texture.cpp
    src/windowManager.cpp
)
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS
    include/*.h
    include/*.hpp
    external/include/*.h
    external/include/*.hpp
)

set(SPDLOG_ENABLE_PCH ON CACHE BOOL "Build static or shared library using precompiled header to speed up compilation time")
add_subdirectory(${CMAKE_SOURCE_DIR}/external/spdlog)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
)

# Os specific configurations
if (UNIX)
    set(EXTRA_LIBS
        dl
    )
endif()

find_package(OpenGL REQUIRED)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    COMPILE_FEATURES cxx_std_17
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/external/include
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${EXTRA_LIBS}
    OpenGL::GL
    OpenGL::GLU
    PRIVATE
    glfw
    spdlog::spdlog
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
