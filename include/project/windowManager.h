#ifndef WINDOW_MANAGER_HPP_GUARD
#define WINDOW_MANAGER_HPP_GUARD

#include <project/common.h>
#include <project/helpers.h>
#include <string>
#include <utility>

/**
 * @brief Abstraction for window management.
 * Allow swapping of window manager by swapping this class with equivalent class
 * for other library such as glfw.
 */
class WindowManager {
   private:
    GLFWwindow* window;
    WindowManager();
    int width, height;
    static WindowManager* instance;
    static void windowResizeCallback(GLFWwindow* window, int width, int height);

   public:
    /**
     * @brief Get the current instance if it exists and create one if it doesn't.
     * Enforce singleton pattern.
     * @return WindowManager*
     */
    static WindowManager* getInstance();
    GLFWwindow* getWindow();
    void createNewWindow(const std::string& windowName, const windowData& data);
    std::pair<double, double> getWindowSize();
    void setWindowSize(std::pair<double, double> newSize);
    void run();
};

#endif
