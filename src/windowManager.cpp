#include <project/windowManager.h>
#include <project/common.h>
#include <project/game.h>

void redisplayfunc(int value);

WindowManager::WindowManager(){};
WindowManager *WindowManager::instance = nullptr;

WindowManager *WindowManager::getInstance()
{
    if (!instance)
    {
        instance = new WindowManager();
    }
    return instance;
}

void WindowManager::createNewWindow(const std::string &windowName, const freeglutData &data)
{
    glutInit(data.argc, data.argv);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(data.width, data.height);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(windowName.c_str());

    auto renderfunc = []()
    {
        Game::getInstance()->render();
    };
    auto reshapefunc = [](int width, int height)
    {
        glViewport(0, 0, width, height);
    };
    glutDisplayFunc(renderfunc);
    glutReshapeFunc(reshapefunc);
    glutKeyboardFunc(&Game::key_down);
    glutKeyboardUpFunc(&Game::key_up);
    glutSpecialFunc(&Game::special_key_down);
    glutSpecialUpFunc(&Game::special_key_up);
    glutTimerFunc(data.refreshInterval, redisplayfunc, data.refreshInterval);
}

std::pair<double, double> WindowManager::getWindowSize()
{
    return std::make_pair(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
}

void WindowManager::setWindowSize(std::pair<double, double> newSize)
{
    auto currentSize = getWindowSize();
    int w = (newSize.first > 0) ? newSize.first : currentSize.first;
    int h = (newSize.second > 0) ? newSize.second : currentSize.second;
    glutReshapeWindow(w, h);
    glViewport(0, 0, w, h);
}

void WindowManager::run()
{
    glutMainLoop();
}

void redisplayfunc(int value)
{
    glutPostRedisplay();
    glutTimerFunc(value, redisplayfunc, value);
};
