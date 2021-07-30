#include <project/common.h>
#include <project/game.h>
#include <project/resourceManager.h>
#include <project/windowManager.h>
#include <spdlog/spdlog.h>

#include <project/stb_image.h>

WindowManager::WindowManager(){};
WindowManager* WindowManager::instance = nullptr;
void windowResizeCallback(GLFWwindow* window, int width, int height);
void inputCallback(GLFWwindow* window,
                   int key,
                   int scancode,
                   int action,
                   int mods);

WindowManager* WindowManager::getInstance() {
    if (!instance) {
        instance = new WindowManager();
    }
    return instance;
}

void WindowManager::createNewWindow(const std::string& windowName,
                                    const windowData& data) {
    if (!glfwInit()) {
        spdlog::error("Could not initialize glfw library!");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(
        data.width, data.height, windowName.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        spdlog::error("Could not initialize glfw window!");
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);
    glfwSetKeyCallback(window, inputCallback);
    glfwSwapInterval(1);

    GLFWimage icons[1];
    int nrChannels;
    stbi_set_flip_vertically_on_load(false);
    auto iconPath = ResourceManager::resolvePath("resources/pacman/1.png");
    auto pixels = stbi_load(iconPath.c_str(),
                            &(icons[0].width),
                            &(icons[0].height),
                            &nrChannels,
                            4);
    if (!pixels) {
        spdlog::error("Could not load icon file!!!");
    } else {
        spdlog::info("Width: {} Height: {}", icons[0].width, icons[0].height);
        icons[0].pixels = pixels;
        glfwSetWindowIcon(window, 1, icons);
        stbi_image_free(icons[0].pixels);
    }
}

std::pair<double, double> WindowManager::getWindowSize() {
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    width = w;
    height = h;
    return std::make_pair(width, height);
}

void WindowManager::setWindowSize(std::pair<double, double> newSize) {
    glfwSetWindowSize(window, newSize.first, newSize.second);
}

void WindowManager::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Game::getInstance()->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void WindowManager::windowResizeCallback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, w, h);
}

void inputCallback(GLFWwindow* window,
                   int key,
                   int scancode,
                   int action,
                   int mods) {
    switch (action) {
        case GLFW_PRESS: {
            if (key < 256) {
                Game::getInstance()->key_down(key, 0, 0);
            } else {
                Game::getInstance()->special_key_down(key, 0, 0);
            }
            break;
        }
        case GLFW_RELEASE: {
            if (key < 256) {
                Game::getInstance()->key_up(key, 0, 0);
            } else {
                Game::getInstance()->special_key_up(key, 0, 0);
            }
        }
    }
}

GLFWwindow* WindowManager::getWindow() {
    return window;
}