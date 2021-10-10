// TODO: Create base sprite class and move setup to new class

#ifndef MAP_HPP_GUARD
#define MAP_HPP_GUARD

#include <project/common.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <project/text_renderer.h>
#include <project/gridBox.hpp>
#include <set>
#include <vector>

/**
 * @brief Class to handle basic transparent map without any characters.
 *
 */
class Map : public Sprite {
    std::pair<int, int> gridSize;
    std::vector<std::vector<int>> gridData;
    const GridBox box, tileBox;
    GLuint gridVAO, gridVBO;
    TextRenderer text;
    void initializeGrid();
    void displayScore();
    void displayLives(const std::string& shaderName);
    void displayLevel(const std::string& shaderName);

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
    void draw(const std::string& shader, bool drawGrid);
    void draw(std::string shader) override;
    /**
     * @brief Draw grid lines on top of map.
     * This is used only for debuggin purposes.
     *
     * @param shader Index of shader to use. Shader must be stored in ResourceManager.
     */
    void drawGridLines(const std::string& shader) const;

    [[nodiscard]] bool checkObstacle(const std::pair<float, float>& toCheck,
                                     const std::set<char>& obstacles = {MAP_WALL, MAP_GATE}) const;

    [[nodiscard]] static char getBlockType(const std::pair<int, int>& toCheck);

    [[nodiscard]] std::set<DIRECTION> possibleDirections(const std::pair<float, float>& toCheck,
                                                         const std::set<char>& obstacles = {
                                                             MAP_WALL,
                                                             MAP_GATE}) const;
};

#endif
