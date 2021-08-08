[![Build](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/build.yml/badge.svg?branch=glfw)](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/build.yml)
[![CodeQL](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/Suven-p/Pacman_OpenGL/actions/workflows/codeql-analysis.yml)

# Pacman
This is an attempt to recreate a clone of the classic game Pacman using glfw and OpenGL. This project is to be used as project work for our Computer Graphics course. This project aims to practically apply studied graphics concepts such as transformations and textures using OpenGL and also learn about teamwork and code organization.

## Build instructions
### Prerequisites
You will need:
 * A modern C++ compiler supporting C++17
 * CMake 3.2+
 * On linux install following packages:
   * `build-essentials` for compiler, debuggers and other basic libraries
   * `freeglut3-dev` for freeglut
   * [packages required by glfw](https://www.glfw.org/docs/3.3.4/compile_guide.html) 

Note(Windows only): Due to a bug in g++ related to std::filesystem([bug#737](https://sourceforge.net/p/mingw-w64/bugs/737/)) g++ version 9 or higher is required. It can be obtained from [msys](https://www.msys2.org/).
### Building The Project

#### Git Clone
First, check out the git repo:
```
❯ mkdir pacman
❯ git clone https://github.com/Suven-p/Pacman_OpenGL pacman
❯ cd pacman
```

#### Generate CMake build files
Generate build files using CMake.
On Windows:
```
❯ cmake -B build -S . -G "MinGW Makefiles"
```
On Linux:
```
❯ cmake -B build -S . -G "Unix Makefiles"
```
This command creates a directory named build and generated files are placed in the same directory. By default, the build system is Visual Studio for Windows and Make for Linux. Other generators can be used by specifying via -G flag. However, this has been tested using "MingW Makefiles" on Windows and "Unix Makefiles" on Linux. For codeblocks with MinGW use cmake -B build -S . -G "CodeBlocks - MinGW Makefiles". See [CMake Documentaton](https://cmake.org/documentation/https://cmake.org/documentation/) for details.

#### Build project
Build the project using CMake. This should work for all build systems.
```
❯ cmake --build build
```

## Running the project
The output executable files are located at `/path/to/pacman/bin/`.
