#pragma once
#include<project/sprite.h>
#include<project/common.h>
class Pacman: public Sprite
{
    DIRECTION currentDirection, nextDirection; 
    void setDirection(DIRECTION newDirection);
    GLuint vao,vbo[2],ebo;
public:

    Pacman();
    void draw(std::string shaderName);
    DIRECTION getDirection();
};