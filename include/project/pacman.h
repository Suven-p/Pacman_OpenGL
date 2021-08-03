#pragma once
#include <project/common.h>
#include <project/sprite.h>
#include <memory>
class Pacman : public Sprite {
    DIRECTION currentDirection, nextDirection;
    std::pair<float, float> oldPosition;
    GLuint vao, vbo[2], ebo;
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
    bool isColliding(DIRECTION aDirection);
};
std::shared_ptr<Pacman> getPacmanPtr();
