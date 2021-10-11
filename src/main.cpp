#include <project/main.h>
#include <spdlog/spdlog.h>
#include <project/gameState.hpp>
#include <project/resourceManager.h>
#ifdef SPDLOG_HEADER_ONLY
#warning USING HEADER ONLY SPDLOG LIBRARY
#endif

using namespace std;

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam);

int main(int argc, char** argv) {
    windowData windowData;
    windowData.argc = &argc;
    windowData.argv = argv;
    windowData.width = 448;
    windowData.height = 576;
    windowData.refreshInterval = 1000.0F / 60.0F;
    auto windowManagerPtr = ResourceManager::getMainWindow();
    windowManagerPtr->createWindow(PROJECT_NAME, windowData);

    if (gladLoadGL() == 0) {
        spdlog::error("Failed to initialize GLAD");
    }

    Game::initialize();  // NOLINT: required to initialize game class

    spdlog::set_level(spdlog::level::debug);
    spdlog::get("ResourceManager")->set_level(spdlog::level::err);

    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(MessageCallback, nullptr);

    // Blending should be enabled for transparency/alpha channel to work,
    // Depth test might not be required. If depth test is enabled, z-values
    // for ghosts must be modified according to name. With depth test disabled,
    // the order of draw calls in render function determines which sprites
    // appears first.

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    windowManagerPtr->run();
}

void GLAPIENTRY MessageCallback(GLenum source,
                                GLenum type,
                                GLuint id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
    spdlog::info("OpenGL::{}\n"
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
