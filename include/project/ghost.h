#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <set>

class Ghost : public Sprite
{
    GLuint vao, vbo[2], ebo;
    std::string name;
    glm::vec3 targetTile;
    DIRECTION currentDirection;
    DIRECTION nextDirection;
    DIRECTION setNextDirection();
    void getNewPosition();
    std::set<DIRECTION> possibleDirections();

public:
    Ghost(std::string name);
    void draw(std::string shader);
    void draw(std::string shader, std::string name);
    ~Ghost();
};