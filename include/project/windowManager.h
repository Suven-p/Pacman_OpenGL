#ifndef WINDOW_MANAGER_HPP_GUARD
#define WINDOW_MANAGER_HPP_GUARD

#include <string>
#include <utility>
#include <project/helpers.h>

/**
 * @brief Abstraction for window management.
 * Allow swapping of window manager by swapping this class with equivalent class
 * for other library such as glfw.
 */
class WindowManager
{
private:
    WindowManager();
    static WindowManager *instance;

public:
    /**
     * @brief Get the current instance if it exists and create one if it doesn't.
     * Enforce singleton pattern.
     * @return WindowManager*
     */
    static WindowManager *getInstance();
    void createNewWindow(const std::string &windowName, const freeglutData &data);
    std::pair<double, double> getWindowSize();
    void setWindowSize(std::pair<double, double> newSize);
    void run();
};

#endif
