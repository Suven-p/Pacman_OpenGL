#include <project/main.h>
#include <spdlog/spdlog.h>
#ifdef SPDLOG_HEADER_ONLY
#warning USING HEADER ONLY SPDLOG LIBRARY
#endif

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
    windowData windowData;
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
        spdlog::error("Failed to initialize GLAD");
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
    spdlog::info(
        "OpenGL::{}\n"
        "\tSource: {}\n"
        "\tType: {}\n"
        "\tId: {}\n"
        "\tSeverity: {}\n"
        "\tMessage: {}\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "GL DEBUG"),
        source,
        type,
        id,
        severity,
        message

    );
}
