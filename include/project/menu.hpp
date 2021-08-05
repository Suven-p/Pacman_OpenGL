#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <string>

class Menu : public Sprite {
    // vao for: darkening, white outline, actual menu
    GLuint vao[3]{}, vbo[4]{}, ebo{};

   public:
    Menu();
    void draw(std::string shader) override;
};
