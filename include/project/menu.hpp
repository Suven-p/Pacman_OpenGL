#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <string>

class Menu : public Sprite {
    GLuint vao[2]{}, vbo[3]{}, ebo{};

   public:
    Menu();
    void draw(std::string shader) override;
};
