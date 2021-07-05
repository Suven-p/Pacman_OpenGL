// TODO: Create base sprite class and move setup to new class

#ifndef MAP_HPP_GUARD
#define MAP_HPP_GUARD

#include <project/common.h>
#include <project/sprite.h>

class Map : Sprite
{
    GLuint VAO, VBO, EBO;

public:
    Map();
    void draw(std::string shader);
};

#endif
