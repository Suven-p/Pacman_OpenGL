#include <project/common.h>
#include <project/game.h>
#include <project/resourceManager.h>
#include <project/MainWindow.hpp>
#include <spdlog/spdlog.h>

#include <stb_image/stb_image.h>
#include "GLFW/glfw3.h"

MainWindow::MainWindow() = default;
void framebufferResizeCallback(GLFWwindow* window, int width, int height);
void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

// clang-format off
std::unordered_map<int, int> key_map = {
    {GLFW_KEY_DOWN, int('s')},
    {GLFW_KEY_UP, int('w')},
    {GLFW_KEY_LEFT, int('a')},
    {GLFW_KEY_RIGHT, int('d')},
    {GLFW_KEY_ESCAPE, KEY_ESC},
    {GLFW_KEY_W, int('w')},
    {GLFW_KEY_A, int('a')},
    {GLFW_KEY_S, int('s')},
    {GLFW_KEY_D, int('d')},
    {GLFW_KEY_ENTER, int('\n')}
};
// clang-format on

void MainWindow::createWindow(const std::string& windowName, const windowData& data) {
    if (!glfwInit()) {
        spdlog::error("Could not initialize glfw library!");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    width = data.width;
    height = data.height;
    window = glfwCreateWindow(data.width, data.height, windowName.c_str(), nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        spdlog::error("Could not initialize glfw window!");
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, inputCallback);
    glfwSwapInterval(1);
    constexpr auto errorCallback = [](int code, const char* description) {
        spdlog::error("Received error {}(Code {}) from glfw", description, code);
    };
    glfwSetErrorCallback(errorCallback);

    GLFWimage icons[1];
    int nrChannels;
    stbi_set_flip_vertically_on_load(false);
    auto iconPath = ResourceManager::resolvePath("resources/pacman/1.png");
    auto pixels =
        stbi_load(iconPath.c_str(), &(icons[0].width), &(icons[0].height), &nrChannels, 4);
    if (!pixels) {
        spdlog::error("Could not load icon file!!!");
    } else {
        spdlog::info("Width: {} Height: {}", icons[0].width, icons[0].height);
        icons[0].pixels = pixels;
        glfwSetWindowIcon(window, 1, icons);
        stbi_image_free(icons[0].pixels);
    }

    // glfwSetWindowSize(window, 800, 800);
    glfwSetWindowAspectRatio(window, 28, 36);
}

std::pair<double, double> MainWindow::getWindowSize() {
    return std::make_pair(width, height);
}

void MainWindow::setWindowSize(std::pair<double, double> newSize) {
    width = newSize.first;
    height = newSize.second;
    glfwSetWindowSize(window, newSize.first, newSize.second);
}

void MainWindow::run() {
    while (glfwWindowShouldClose(window) == 0) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Game::render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void framebufferResizeCallback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS: {
            auto it = key_map.find(key);
            if (it != key_map.end()) {
                Game::key_down(it->second);
            }
        }
    }
}

GLFWwindow* MainWindow::getWindow() {
    return window;
}

void MainWindow::exit() {
    glfwSetWindowShouldClose(window, 1);
}
