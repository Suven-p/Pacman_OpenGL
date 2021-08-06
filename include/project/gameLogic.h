#pragma once
#include <project/common.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <project/text_renderer.h>
#include <project/windowManager.h>
#include <memory>
#include <project/gameState.hpp>
#include <vector>

class GameLogic : public Sprite {
    GLuint VAO, VBO, blockEBO;
    GameState& gameState;
    TextRenderer text;
    void displayScore();
    void displayLives(const std::string& shaderName);
    void displayLevel(const std::string& shaderName);
    void checkStatus();

   public:
    GameLogic(GameState& gameState);
    ~GameLogic();
    void draw(std::string shaderName) override;
};
