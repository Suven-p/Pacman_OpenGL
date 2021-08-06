#pragma once

#include <project/common.h>
#include <project/texture.h>
#include <glm/glm.hpp>
#include <string>
#include <utility>

/**
 * @brief A abstract class for game objects
 * An abstract base class so that all game objects can be stored in ResourceMap
 * without std::variant. All derived class must use public inheritance.
 */
class Sprite {
   protected:
    std::pair<float, float> position;
    std::string texture;
    void setTexture(std::string newTexture);
    std::string getTexture() const;

   public:
    Sprite();
    void setPosition(std::pair<float, float> newPosition);
    std::pair<float, float> getPosition() const;
    virtual void draw(std::string shader) = 0;
    virtual ~Sprite();
    virtual void reset();
};
