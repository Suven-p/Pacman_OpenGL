#pragma once

#include <string>
#include <project/common.h>
#include <project/texture.h>
#include <glm/glm.hpp>

class Sprite
{
protected:
    std::string texture;

public:
    void setTexture(std::string newTexture);
    std::string getTexture() const;
    virtual void draw(std::string shader) = 0;
};