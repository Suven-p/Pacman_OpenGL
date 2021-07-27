#pragma once
#include<project/sprite.h>
#include<project/common.h>
#include<memory>
class Pacman: public Sprite
{
    DIRECTION currentDirection, nextDirection;
    GLuint vao,vbo[2],ebo;
    float multiplier;
public:

    Pacman();
    void draw(std::string shaderName);
    DIRECTION getDirection();
    void setDirection(DIRECTION newDirection);
    void getNewPosition();
    DIRECTION setNextDirection();
    float getMultiplier();
    void setMultiplier(float);
    void setNextDirection(DIRECTION);
};
std::shared_ptr<Pacman> getPacmanPtr();