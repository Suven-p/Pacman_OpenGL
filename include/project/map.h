// TODO: Create base sprite class and move setup to new class

#ifndef MAP_HPP_GUARD
#define MAP_HPP_GUARD

#include <project/common.h>
#include <project/sprite.h>

/**
 * @brief Class to handle basic transparent map without any characters.
 *
 */
class Map : public Sprite
{
    GLuint VAO, VBO, EBO;

public:
    /**
     * @brief Construct a new Map object.
     * Generates and configures the vao required for render.
     */
    Map();
    /**
     * @brief Draw the actual map.
     *
     * @param shader Index of shader to use. Shader must be stored in ResourceManager.
     */
    void draw(std::string shader);
};

#endif
