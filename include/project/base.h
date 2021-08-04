#pragma once
#include <project/common.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <project/text_renderer.h>
#include <vector>
#include <memory>

class Base : public Sprite {
    GLuint blockVAO, blockVBO, blockEBO;
    int lives;
    std::vector<int> position;
   public:

    Base();
    void draw(std::string shaderName);

    // Decrease lives by 1. To be called when ghost eats pacman 
    void decreaselives();

    TextRenderer* Text;

    ~Base();
};
std::shared_ptr<Base> getBasePtr();
