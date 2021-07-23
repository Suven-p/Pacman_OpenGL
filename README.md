[![Build](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/build.yml/badge.svg?branch=glfw)](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/build.yml)
[![CodeQL](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/codeql-analysis.yml)

# Pacman
This is an attempt to recreate a clone of the classic game Pacman using freeglut and OpenGL. This project is to be used as project work for our Computer Graphics course. This project aims to practically apply studied graphics concepts such as transformations and textures using OpenGL and also learn about teamwork and code organization.

## Build instructions
### Prerequisites
You will need:
 * A modern C++ compiler supporting C++17
 * CMake 3.2+
 * On linux install following packages:
   * `build-essentials` for compiler, debuggers and other basic libraries
   * `freeglut3-dev` for freeglut
 
### Building The Project

#### Git Clone
First, check out the git repo:
```
❯ mkdir pacman
❯ git clone https://github.com/Suven-p/gl_project_2.git pacman
❯ cd pacman
```

#### Generate CMake build files
Generate build files using CMake.
```
❯ cmake -B build -S .
```
This command creates a directory named build and generated files are placed in the directory. It is recommended to place CMake files in a separate directory ignored by git.
By default, the build system is Visual Studio for Windows and Make for Linux. Other generators can be used by specifying via `-G` flag. For codeblocks with MinGW use `cmake -B build -S . -G "CodeBlocks - MinGW Makefiles"`. In most cases, it is sufficient to generate the files only once for each build system. If the build system is to be changed, 
delete the build/ directory and run this step again. 

#### Build project
Build the project using CMake. This should work for all build systems.
```
❯ cmake --build build
```

## Running the project
The output executable files are located at `/path/to/pacman/bin/`. CMake should also copy required assests(images, shaders and dll files) to the `bin` directory.
For codeblocks, open the generated project file and change the project execution directory to `/path/to/pacman/bin/` instead of `/path/to/pacman/build/`.

