#include <project/game.h>
#include <project/gameLogic.h>
#include <project/resourceManager.h>
#include <project/text_renderer.h>
#include <spdlog/spdlog.h>
#include <project/menu.hpp>
#include <utility>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "project/helpers.h"

BorderedMenu::BorderedMenu(std::string title,
                           std::vector<std::string> names,
                           std::map<std::string, std::function<void(void)>> callbacks) :
    title(title),
    optionNames(names),
    optionCallbacks(callbacks),
    selectedOption(0),
    isActive(false) {
    glGenVertexArrays(3, vao);
    glGenBuffers(4, vbo);
    glGenBuffers(1, &ebo);

    // clang-format off
    float colorDark[4] = {0.0, 0.0, 0.0, 0.65F};
    float verticesPos[] = {
        -1.0F, -1.0F, 0.8F, 1.0F,
        -1.0F, 01.0F, 0.8F, 1.0F,
        01.0F, 01.0F, 0.8F, 1.0F,
        01.0F, -1.0F, 0.8F, 1.0F
    };
    float verticesColorDarken[] = {
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3]
    };
    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    // clang-format on

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPos), verticesPos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorDarken), verticesColorDarken, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // clang-format off
    float colorWhite[] = {1.0F, 1.0F, 1.0F, 1.0F};
    float verticesColorWhite[] = {
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3],
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3],
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3],
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3]
    };
    // clang-format off
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorWhite), verticesColorWhite, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    auto callback = [&](auto key) {
        if (isActive) {
            handleKeyboardInput(key);
        }
    };
    keyboardCallbackID = Game::registerKeyboardCallback(callback);
}

BorderedMenu::~BorderedMenu() {
    Game::unregisterKeyboardCallback(keyboardCallbackID);
}

void BorderedMenu::draw(std::string shader) {
    ResourceManager::GetShader(shader).Use();
    // Draw dark overlay
    glBindVertexArray(vao[0]);
    glm::mat4 model = glm::mat4(1.0F);
    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::mat4(1.0F);
    ResourceManager::GetShader(shader).SetMatrix4("model", model, true);
    ResourceManager::GetShader(shader).SetMatrix4("view", view, true);
    ResourceManager::GetShader(shader).SetMatrix4("projection", projection, true);
    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 1.0F);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);

    // Show white rectangle
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5);
    glBindVertexArray(vao[1]);
    model = glm::scale(model, glm::vec3(0.75F, 0.75F, 1.0F));
    ResourceManager::GetShader(shader).SetMatrix4("model", model, true);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1);

    show_options();

    glBindVertexArray(0);
}

void BorderedMenu::show_options() {
    auto sz = ResourceManager::getMainWindow()->getWindowSize();
    static auto text = TextRenderer(sz.first, sz.second);
    static bool initText = false;
    if (!initText) {
        initText = true;
        text.Load(ResourceManager::resolvePath("resources/fonts/ARIAL.TTF"), 24);
    }

    int xOffset = (sz.first/2) - int(title.size() * 12 / 2);
    constexpr int yOffset = 50;
    text.RenderText(title, xOffset, 150.0F, 1.0F);
    int count = 0;
    for (const auto name: optionNames) {
        constexpr glm::vec3 colorSelected = {1.0F, 1.0F, 1.0F};
        constexpr glm::vec3 colorUnselected = {0.5, 0.5, 0.5};
        auto optionColor = (count == selectedOption)?colorSelected:colorUnselected;
        xOffset = (sz.first/2) - int(name.size() * 12 / 2);
        text.RenderText(name, xOffset, 150.0F + yOffset * (count + 1), 1.0F, optionColor);
        count++;
    }
}

void BorderedMenu::handleKeyboardInput(int key) {
    switch (key) {
        case int('s'): {
            selectedOption = (selectedOption == (optionNames.size() - 1)) ? 0 : selectedOption + 1;
            break;
        }
        case int('w'): {
            selectedOption = (selectedOption == 0) ? (optionNames.size() - 1) : selectedOption - 1;
            break;
        }
        case int('\n'): {
            executeFunction();
        }
    }
}

void BorderedMenu::executeFunction() {
    auto option = optionNames[selectedOption];
    (optionCallbacks.at(option))();
}

void BorderedMenu::activate(bool activate) {
    isActive = activate;
}

MainMenu::MainMenu() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);
    glGenBuffers(1, &ebo);

    // clang-format off
    float colorDark[4] = {0.0, 0.0, 0.0, 0.65F};
    float verticesPos[] = {
        -1.0F, -1.0F, 0.8F, 1.0F,
        -1.0F, 01.0F, 0.8F, 1.0F,
        01.0F, 01.0F, 0.8F, 1.0F,
        01.0F, -1.0F, 0.8F, 1.0F
    };
    float verticesColorDarken[] = {
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3]
    };
    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    // clang-format on

    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPos), verticesPos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorDarken), verticesColorDarken, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    auto callback = [](int key) {
        if (key == int('\n')) {
            Game::getState().setReady(true);
        }
    };
    Game::registerKeyboardCallback(callback);
}

void MainMenu::draw(std::string shaderName) {
    // Draw dark overlay
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(vao);
    glm::mat4 model = glm::mat4(1.0F);
    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::mat4(1.0F);
    shader.SetMatrix4("model", model, true);
    shader.SetMatrix4("view", view, true);
    shader.SetMatrix4("projection", projection, true);
    shader.SetFloat("textureColorMix", 1.0F);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);

    auto sz = ResourceManager::getMainWindow()->getWindowSize();
    static auto text = TextRenderer(sz.first, sz.second);
    static bool initText = false;
    if (!initText) {
        initText = true;
        text.Load(ResourceManager::resolvePath("resources/fonts/ARIAL.TTF"), 24);
    }
    std::string toRender = "PRESS ENTER TO CONTINUE";
    int xOffset = (sz.first / 2) - (toRender.size() / 2 * 15);
    int yOffset = (sz.second / 2) - (18);
    text.RenderText(toRender, xOffset, yOffset, 1.0F);
}


void ReadyScreen::draw(std::string shaderName) {
    auto sz = ResourceManager::getMainWindow()->getWindowSize();
    static auto text = TextRenderer(sz.first, sz.second);
    static bool initText = false;
    if (!initText) {
        initText = true;
        text.Load(ResourceManager::resolvePath("resources/fonts/ARIAL.TTF"), 24);
    }
    std::string toRender = "Ready!";
    int xOffset = (sz.first / 2) - (toRender.size() / 2 * 12);
    int yOffset = (sz.second / 2) + (30);
    text.RenderText(toRender, xOffset, yOffset, 1.0F);
    static Timer readyTimer;
    readyTimer.start();
    if (readyTimer.timeElapsed() > 3000) {
        Game::getState().setReady(false);
        Game::getState().setStarted(true);
        readyTimer = Timer();
    }
}
