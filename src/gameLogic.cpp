#include <project/common.h>
#include <project/game.h>
#include <project/gameLogic.h>
#include <project/ghost.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <project/text_renderer.h>
#include <string>
#include <vector>
#include "project/windowManager.h"

GameLogic::GameLogic(GameState& gameState) : gameState(gameState), text(0, 0) {
    auto windowSize = WindowManager::getInstance()->getWindowSize();
    text = std::move(TextRenderer(windowSize.first, windowSize.second));
    text.Load(ResourceManager::resolvePath("resources/fonts/ARIAL.TTF"), 24);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &blockEBO);
    // clang-format off
    float obstacleVertices[] = {
        // positions             // colors               // texture coords
        1.8F, 0.2F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1.0F,  // top right
        1.8F, 1.8F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F,  // bottom right
        0.2F, 1.8F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F,  // bottom left
        0.2F, 0.2F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F   // top left
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    // clang-format on

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleVertices), obstacleVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void GameLogic::draw(std::string shaderName) {
    checkStatus();
    displayScore();
    displayLives(shaderName);
    displayLevel(shaderName);
}

void GameLogic::displayScore() {
    int currentScore = ResourceManager::GetSprite<Pellet>("pellet")->getScore();
    std::string textToRender = "Score : " + std::to_string(currentScore);
    text.RenderText(textToRender, 15.0F, 15.0F, 1.0F);
}

void GameLogic::displayLives(const std::string& shaderName) {
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0F);
    model = glm::translate(model, glm::vec3(0.0F, 34.0F, 0.0F));
    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, -1.0F, 1.0F);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 0.0F);
    auto texture = ResourceManager::GetTexture("pacman");
    texture.Bind(0);
    shader.SetInteger("texture1", 0, true);

    for (int i = 1, lives = gameState.getLives(); i < lives; i++) {
        model = glm::translate(model, glm::vec3(2.0F, 0.0F, 0.0F));
        shader.SetMatrix4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void GameLogic::displayLevel(const std::string& shaderName) {
    auto currentLevel = gameState.getLevel();

    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0F);
    model = glm::translate(model, glm::vec3(24.0F, 34.0F, 0.0F));
    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, -1.0F, 1.0F);
    shader.SetMatrix4("model", model);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 0.0F);
    auto texture2 = ResourceManager::GetTexture("cherry");
    texture2.Bind(1);
    shader.SetInteger("texture1", 1, true);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    // tile coordinates multiplied to convert to pixel coordinates
    auto levelText = std::to_string(currentLevel);
    text.RenderText(levelText, 26.0F * 16, 34.5F * 16, 1.0F);
    glBindVertexArray(0);
}

GameLogic::~GameLogic() = default;

void GameLogic::checkStatus() {
    auto pacmanPtr = ResourceManager::GetSprite<Pacman>("pacman");
    auto blinkyPtr = ResourceManager::GetSprite<Ghost>("blinky");
    auto pinkyPtr = ResourceManager::GetSprite<Ghost>("pinky");
    auto inkyPtr = ResourceManager::GetSprite<Ghost>("inky");
    auto clydePtr = ResourceManager::GetSprite<Ghost>("clyde");
    constexpr auto toInt = [](std::pair<float, float> pos) {
        std::pair<int, int> ans = pos;
        if ((pos.first - int(pos.first)) >= 0.5) {
            ans.first++;
        }
        if ((pos.second - int(pos.second)) >= 0.5) {
            ans.second++;
        }
        return ans;
    };
    if (toInt(pacmanPtr->getPosition()) == toInt(blinkyPtr->getPosition())) {
        handleCollision(blinkyPtr);
    }
    if (toInt(pacmanPtr->getPosition()) == toInt(pinkyPtr->getPosition())) {
        handleCollision(pinkyPtr);
    }
    if (toInt(pacmanPtr->getPosition()) == toInt(inkyPtr->getPosition())) {
        handleCollision(inkyPtr);
    }
    if (toInt(pacmanPtr->getPosition()) == toInt(clydePtr->getPosition())) {
        handleCollision(clydePtr);
    }
}

void GameLogic::handleCollision(std::shared_ptr<Ghost> ghostPtr) {
    if (ghostPtr->getMode() == GhostMode::frightened) {
        auto pelletPtr = ResourceManager::GetSprite<Pellet>("pellet");
        pelletPtr->addToScore(200);
        ghostPtr->setMode(GhostMode::dead);
    }
    else if (ghostPtr->getMode() == GhostMode::chase || ghostPtr->getMode() == GhostMode::scatter) {
        Game::getState().setLives(Game::getState().getLives() - 1);
        ResourceManager::resetSprites({"pellet"});
    }
}

void GameLogic::reset() {}
