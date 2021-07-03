#ifndef WINDOW_MANAGER_HPP_GUARD
#define WINDOW_MANAGER_HPP_GUARD

#include <string>
#include <project/helpers.h>

class WindowManager
{
private:
    WindowManager();
    static WindowManager *instance;

public:
    static WindowManager *getInstance();
    void createNewWindow(const std::string &windowName, const freeglutData &data);
    COORD getWindowSize();
    void setWindowSize(COORD newSize);
    void run();
};

#endif
