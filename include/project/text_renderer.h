#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <project/common.h>
#include <map>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <project/texture.h>
#include <project/shader.h>

struct Character {
    unsigned int TextureID; 
    glm::ivec2   Size;      
    glm::ivec2   Bearing;   
    unsigned int Advance; 
};

class TextRenderer
{
private:
    // Render state
    unsigned int VAO, VBO;
public:
    TextRenderer(unsigned int width, unsigned int height);
    std::map<char, Character> Characters; 
    Shader TextShader;

    void Load(std::string font, unsigned int fontSize);
    void RenderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
};

#endif 
