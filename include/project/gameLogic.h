#pragma once
#include <project/common.h>
#include <project/ghost.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <project/text_renderer.h>
#include <project/windowManager.h>
#include <memory>
#include <project/gameState.hpp>
#include <vector>

class GameLogic : public Sprite {
    GLuint VAO, VBO, EBO;
    GameState& gameState;
    TextRenderer text;
    void checkStatus();
    void handleCollision(std::shared_ptr<Ghost> ghostPtr);

   public:
    GameLogic(GameState& gameState);
    ~GameLogic() override;
    void draw(std::string shaderName) override;
    void reset() override;
};
