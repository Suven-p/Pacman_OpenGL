# Use an Ubuntu base image
FROM ubuntu:20.04

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && \
    apt-get install -y \
    build-essential \
    cmake \
    freeglut3-dev \
    libglfw3-dev \
    libgl1-mesa-dev \
    libglu1-mesa-dev \
    git && \
    rm -rf /var/lib/apt/lists/*

# Set up the working directory
WORKDIR /usr/src/app

# Clone the Pacman repository
RUN git clone https://github.com/Suven-p/Pacman_OpenGL pacman

# Change to project directory
WORKDIR /usr/src/app/pacman

# Generate CMake build files
RUN cmake -B build -S . -G "Unix Makefiles"

# Build the project
CMD ["cmake --build build"]
