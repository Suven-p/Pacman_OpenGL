#include <project/main.h>

using namespace std;

void reshape(int width, int height);
void render();

int main(int argc, char **argv)
{
    freeglutData windowData;
    windowData.argc = &argc;
    windowData.argv = argv;
    windowData.width = 448;
    windowData.height = 576;
    windowData.refreshInterval = 1000.0f / 60.0f;
    auto windowManagerPtr = WindowManager::getInstance();
    windowManagerPtr->createNewWindow(PROJECT_NAME, windowData);

    if (!gladLoadGL())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    auto game_obj = Game::getInstance();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    windowManagerPtr->run();
}
