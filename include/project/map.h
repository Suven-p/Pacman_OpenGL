// TODO: Create base sprite class and move setup to new class

#ifndef MAP_HPP_GUARD
#define MAP_HPP_GUARD

#include <vector>
#include <project/common.h>
#include <project/sprite.h>
#include <project/mapData.h>

/**
 * @brief Class to handle basic transparent map without any characters.
 *
 */
class Map : public Sprite
{
    std::pair<int, int> gridSize;
    std::vector<std::vector<int>> gridData;
    GLuint VAO, VBO, EBO;
    GLuint gridVAO, gridVBO;
    GLuint blockVAO, blockVBO, blockEBO;
    void initializeGrid();

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
    void draw(std::string shader, bool drawGrid);
    void draw(std::string shader);
    /**
     * @brief Draw grid lines on top of map.
     * This is used only for debuggin purposes.
     *
     * @param shader Index of shader to use. Shader must be stored in ResourceManager.
     */
    void drawGridLines(std::string shader);
    /**
     * @brief Show obstacles as colored blocks.
     * This is used to demonstrate and verify position of obstacles according to stored map data.
     *
     */
    void drawObstacles(std::string shader);
};

#endif
