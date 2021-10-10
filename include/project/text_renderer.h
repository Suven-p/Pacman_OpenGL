#pragma once

#include <glad/glad.h>
#include <project/common.h>
#include <project/shader.h>
#include <project/texture.h>
#include <glm/glm.hpp>
#include <map>

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    long Advance;
};

class TextRenderer {
   private:
    // Render state
    unsigned int VAO, VBO;

   public:
    TextRenderer(unsigned int width, unsigned int height);
    std::map<char, Character> Characters;
    Shader TextShader;

    void Load(const std::string& font, unsigned int fontSize);
    std::pair<float, float> calculateDimension(const std::string& text, float scale);
    std::pair<float, float> RenderText(const std::string& text,
                    float x,
                    float y,
                    float scale,
                    glm::vec3 color = glm::vec3(1.0F));
};
