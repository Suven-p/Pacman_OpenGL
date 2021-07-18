#include <project/main.h>
#include <spdlog/spdlog.h>

using namespace std;

void reshape(int width, int height);
void render();
void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *message,
                                const void *userParam);

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
    spdlog::set_level(spdlog::level::debug);

    if (!gladLoadGL())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    auto game_obj = Game::getInstance();

    glEnable(GL_DEPTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    windowManagerPtr->run();
}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar *message,
                                const void *userParam)
{
    std::cerr << "Debug::OpenGL::" << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "GL DEBUG") << "\n"
              << "Source: " << source << "\n"
              << "Type: " << type << "\n"
              << "Id: " << id << "\n"
              << "Severity: " << severity << "\n"
              << "Message: " << message << "\n\n"
              << std::endl;
}
