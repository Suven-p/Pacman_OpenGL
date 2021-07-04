#ifndef WINDOW_MANAGER_HPP_GUARD
#define WINDOW_MANAGER_HPP_GUARD

#include <string>
#include <utility>
#include <project/helpers.h>

class WindowManager
{
private:
    WindowManager();
    static WindowManager *instance;

public:
    static WindowManager *getInstance();
    void createNewWindow(const std::string &windowName, const freeglutData &data);
    std::pair<double, double> getWindowSize();
    void setWindowSize(std::pair<double, double> newSize);
    void run();
};

#endif
