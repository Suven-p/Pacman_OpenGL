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
                                const GLchar* message,
                                const void* userParam);

int main(int argc, char** argv) {
    windowData windowData;
    windowData.argc = &argc;
    windowData.argv = argv;
    windowData.width = 448;
    windowData.height = 576;
    windowData.refreshInterval = 1000.0f / 60.0f;
    auto windowManagerPtr = WindowManager::getInstance();
    windowManagerPtr->createNewWindow(PROJECT_NAME, windowData);
    spdlog::set_level(spdlog::level::debug);

    if (!gladLoadGL()) {
        spdlog::error("Failed to initialize GLAD");
    }

    auto game_obj = Game::getInstance();

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
    // Blending should be enabled for transparency/alpha channel to work,
    // Depth test might not be required. If depth test is enabled, z-values
    // for ghosts must be modified according to name. With depth test disabled,
    // the order of draw calls in render function determines which sprites
    // appears first.

    // glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    spdlog::set_level(spdlog::level::debug);
    int r, g, b, a, depth, stencil;
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &r);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &g);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &b);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &a);
    spdlog::info("Size of rgba channels: {} {} {} {}", r, g, b, a);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth);
    glGetFramebufferAttachmentParameteriv(
        GL_DRAW_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil);
    spdlog::info("Size of depth, stencil channels: {} {}", depth, stencil);
    spdlog::info("Refresh Rate: {}", glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);

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
