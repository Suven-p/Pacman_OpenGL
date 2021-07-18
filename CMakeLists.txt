cmake_minimum_required(VERSION 3.20)

project(Pacman VERSION 1.0 LANGUAGES CXX C)

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
    src/resourceManager.cpp
    src/shader.cpp
    src/sprite.cpp
    src/texture.cpp
    src/windowManager.cpp
)
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS include/*.h include/*.hpp)
file(GLOB_RECURSE
    SPDLOG_HEADERS
    ${CMAKE_SOURCE_DIR}/spdlog/include/*.h
    ${CMAKE_SOURCE_DIR}/spdlog/include/*.hpp
)

set(USE_32BIT False CACHE BOOL "Use 32 bit library")

add_executable(${PROJECT_NAME}
    ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS}
    ${SPDLOG_HEADERS}
)

# Os specific configurations
if (UNIX)
    set(EXTRA_LIBS
        dl
    )
elseif(WIN32)
    set(EXTRA_LIBS)
    if (${CMAKE_GENERATOR} MATCHES "^Visual Studio")
        set(FREEGLUT_DIR msvc)
    else()
        set(FREEGLUT_DIR mingw)
    endif()

    if (USE_32BIT)
        message(STATUS "Using 32 bit libraries for freeglut.")
        set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/freeglut/${FREEGLUT_DIR}/x86")
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/freeglut/${FREEGLUT_DIR}/x86/bin/freeglut.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/freeglut.dll
        )
    else()
         message(STATUS "Using 64 bit libraries for freeglut.")
        set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/freeglut/${FREEGLUT_DIR}/x64")
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/freeglut/${FREEGLUT_DIR}/x64/bin/freeglut.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>/freeglut.dll
        )
    endif()
else()
    message(WARNING "This OS might not be supported!!!")
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    COMPILE_FEATURES cxx_std_17
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
target_include_directories(${PROJECT_NAME} PUBLIC
    "include"
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/spdlog/include
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${EXTRA_LIBS}
    OpenGL::GL
    OpenGL::GLU
    ${GLUT_LIBRARIES}
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
