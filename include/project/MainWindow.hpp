#pragma once

#include <project/common.h>
#include <project/helpers.h>
#include <string>
#include <utility>

/**
 * @brief Abstraction for window management.
 * Allow swapping of window manager by swapping this class with equivalent class
 * for other library.
 */
class MainWindow {
   private:
    GLFWwindow* window;
    int width, height;

   public:
    MainWindow();
    GLFWwindow* getWindow();
    void createWindow(const std::string& windowName, const windowData& data);
    std::pair<double, double> getWindowSize();
    void setWindowSize(std::pair<double, double> newSize);
    void run();
    void exit();
};
