This is text version of readme file. Markdown version is better displayed in github but might be difficult to read in text form.

Build instructions
Prerequisites
You will need:

A modern C++ compiler supporting C++17
CMake 3.2+
On linux install following packages:
build-essentials for compiler, debuggers and other basic libraries
freeglut3-dev for freeglut
packages required by glfw
Note(Windows only): Due to a bug in g++ related to std::filesystem(bug#737) g++ version 9 or higher is required. It can be obtained from msys.

Building The Project
Git Clone
First, check out the git repo:

❯ mkdir pacman
❯ git clone https://github.com/Suven-p/Pacman_OpenGL pacman
❯ cd pacman
Generate CMake build files
Generate build files using CMake.

❯ cmake -B build -S .
This command creates a directory named build and generated files are placed in the same directory. By default, the build system is Visual Studio for Windows and Make for Linux. Other generators can be used by specifying via -G flag. For codeblocks with MinGW use cmake -B build -S . -G "CodeBlocks - MinGW Makefiles". In most cases, it is sufficient to generate the files only once for each build system. If the build system is to be changed, delete the build/ directory and run this step again. See CMake Documentaton for details.

Build project
Build the project using CMake. This should work for all build systems.

❯ cmake --build build
Running the project
The output executable files are located at /path/to/pacman/bin/