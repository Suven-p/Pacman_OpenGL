// TODO: Create base sprite class and move setup to new class

#ifndef MAP_HPP_GUARD
#define MAP_HPP_GUARD

#include <project/common.h>
#include <project/sprite.h>

// Inheritance must be public to use Sprite* in resource manager
class Map : public Sprite
{
    GLuint VAO, VBO, EBO;

public:
    Map();
    void draw(std::string shader);
};

#endif
