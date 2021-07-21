#include <project/windowManager.h>
#include <project/common.h>
#include <project/game.h>
#include <spdlog/spdlog.h>

WindowManager::WindowManager(){};
WindowManager *WindowManager::instance = nullptr;
void windowResizeCallback(GLFWwindow *window, int width, int height);
void inputCallback(GLFWwindow *window);

WindowManager *WindowManager::getInstance()
{
    if (!instance)
    {
        instance = new WindowManager();
    }
    return instance;
}

void WindowManager::createNewWindow(const std::string &windowName, const windowData &data)
{
    if (!glfwInit())
    {
        spdlog::error("Could not initialize glfw library!");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    window = glfwCreateWindow(data.width, data.height, windowName.c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        spdlog::error("Could not initialize glfw window!");
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, windowResizeCallback);
}

std::pair<double, double> WindowManager::getWindowSize()
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    width = w;
    height = h;
    return std::make_pair(width, height);
}

void WindowManager::setWindowSize(std::pair<double, double> newSize)
{
    glfwSetWindowSize(window, newSize.first, newSize.second);
}

void WindowManager::run()
{
    while (!glfwWindowShouldClose(window))
    {
        inputCallback(window);
        Game::getInstance()->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void WindowManager::windowResizeCallback(GLFWwindow *window, int w, int h)
{
    glViewport(0, 0, w, h);
}

void inputCallback(GLFWwindow *window)
{
}