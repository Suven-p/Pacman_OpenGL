#pragma once

#include <project/common.h>
#include <project/sprite.h>

class Ghost: public Sprite
{
    GLuint vao, vbo[2], ebo;
    std::string name;

    public:
    Ghost(std::string name);
    void draw(std::string shader);
    void draw(std::string shader, std::string name);
    ~Ghost();
};