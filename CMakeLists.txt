cmake_minimum_required(VERSION 3.20)

project(Pacman VERSION 1.0)

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

if (UNIX)
    set(EXTRA_LIBS
        dl
    )
elseif(WIN32)
    set(EXTRA_LIBS)
    set(CMAKE_PREFIX_PATH "freeglut")
else()
    message(WARNING "This OS might not be supported!!!")
endif()

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17 )
target_include_directories(${PROJECT_NAME} PUBLIC
    "include"
    ${OPENGL_INCLUDE_DIR}
    ${GLUT_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${EXTRA_LIBS}
    OpenGL::GL
    OpenGL::GLU
    ${GLUT_LIBRARIES}
)

message(STATUS
    "Include directory are: ${OPENGL_INCLUDE_DIR}\n"
    "OpenGL libraries are: ${OPENGL_LIBRARIES}\n"
    "Glut include directory is: ${GLUT_INCLUDE_DIR}\n"
    "Glut libraries are: ${GLUT_LIBRARIES}\n"
)
