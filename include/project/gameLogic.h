#pragma once
#include <project/common.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <project/text_renderer.h>
#include <memory>
#include <vector>

class GameLogic : public Sprite {
    GLuint blockVAO, blockVBO, blockEBO;
    int lives;
    std::vector<int> position;

   public:
    GameLogic();
    void draw(std::string shaderName);

    // Decrease lives by 1. To be called when ghost eats pacman
    void decreaselives();

    TextRenderer* Text;

    ~GameLogic();
};
std::shared_ptr<GameLogic> getBasePtr();
