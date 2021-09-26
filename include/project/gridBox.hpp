#pragma once

#include <project/common.h>
#include <glm/glm.hpp>
#include <utility>
#include <array>
#include <string>

class GridBox {
    private:
    std::pair<int, int> gridSize;
    GLuint VAO, VBO[3], EBO;
    glm::mat4 model, view, projection;

    public:
    GridBox(const std::pair<float, float> &size = {1.0F, 1.0F});
    void setColor(const std::array<float, 4> &color) const;
    void setColor(const std::array<std::array<float, 4>, 4> &color) const;
    void setTexCoord(const std::array<std::array<float, 2>, 4> &textureCoord) const;
    void draw(const std::array<float, 3> &position, const std::string &shaderName) const;

};
