// TODO: Create base sprite class and move setup to new class

#ifndef MAP_HPP_GUARD
#define MAP_HPP_GUARD

#include <project/common.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <set>
#include <vector>

/**
 * @brief Class to handle basic transparent map without any characters.
 *
 */
class Map : public Sprite {
    std::pair<int, int> gridSize;
    std::vector<std::vector<int>> gridData;
    GLuint VAO, VBO, EBO;
    GLuint gridVAO, gridVBO;
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

    [[nodiscard]] char getBlockType(const std::pair<int, int>& toCheck) const;

    [[nodiscard]] std::set<DIRECTION> possibleDirections(const std::pair<float, float>& toCheck,
                                                         const std::set<char>& obstacles = {
                                                             MAP_WALL,
                                                             MAP_GATE}) const;
};

#endif
