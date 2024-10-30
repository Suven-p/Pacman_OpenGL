# Use an official image with C++17 support
FROM ubuntu:latest

# Install system dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    freeglut3-dev \
    libglfw3-dev \
    git

# Set working directory
WORKDIR /app

# Clone the repository
RUN git clone https://github.com/Suven-p/Pacman_OpenGL pacman
WORKDIR /app/pacman

# Generate CMake build files
RUN cmake -B build -S . -G "Unix Makefiles"

# Build the project
RUN cmake --build build

# Set entry point to the executable
CMD ["./bin/pacman"]
