#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <set>

enum struct GhostMode{chase, scatter, frightened};

class Ghost : public Sprite
{
    GLuint vao, vbo[2], ebo;
    std::string name;
    std::pair<float, float> targetTile;
    DIRECTION currentDirection;
    DIRECTION nextDirection;
    DIRECTION setNextDirection();
    void getNewPosition();
    std::set<DIRECTION> possibleDirections();
    GhostMode currentMode;

public:
    Ghost(std::string name);
    void draw(std::string shader);
    void draw(std::string shader, std::string name);
    GhostMode getMode();
    void setMode(GhostMode newMode);
    ~Ghost();
};