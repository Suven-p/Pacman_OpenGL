#pragma once

#include <string>
#include <project/common.h>
#include <project/texture.h>
#include <glm/glm.hpp>

/**
 * @brief A abstract class for game objects
 * An abstract base class so that all game objects can be stored in ResourceMap
 * without std::variant. All derived class must use public inheritance.
 */
class Sprite
{
protected:
    std::string texture;

public:
    void setTexture(std::string newTexture);
    std::string getTexture() const;
    virtual void draw(std::string shader) = 0;
};