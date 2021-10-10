#include <ft2build.h>
#include <project/common.h>
#include <project/game.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <project/text_renderer.h>
#include <cmath>
#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(unsigned int width, unsigned int height) : VAO(0), VBO(0) {
    TextShader = ResourceManager::GetShader("textShader");

    TextShader.SetMatrix4(
        "projection",
        glm::ortho(0.0F, static_cast<float>(width), static_cast<float>(height), 0.0F),
        true);
    TextShader.SetInteger("text", 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Load no data but reserve space
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::Load(const std::string& font, unsigned int fontSize) {
    Characters.clear();
    FT_Library ft{};
    if (FT_Init_FreeType(&ft) != 0) {
        spdlog::error("ERROR::FREETYPE: Could not init FreeType Library");
    }

    FT_Face face{};
    if (FT_New_Face(ft, font.c_str(), 0, &face) != 0) {
        spdlog::error("ERROR::FREETYPE: Failed to load font");
    }

    // set width according to height ie fontSize
    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0) {
            spdlog::info("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // Generate texture
        unsigned int texture = 0;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Now store character for later use
        Character character = {texture,
                               glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                               glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                               face->glyph->advance.x};
        Characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

std::pair<float, float> TextRenderer::calculateDimension(const std::string& text, float scale) {
    float X = 0, Y = 0;
    for(auto c: text) {
        auto ch = Characters[c];
        X += (ch.Advance >> 6) * scale;
        // (offset to start of character + size of character) * scale
        float ch_Y = ((Characters['H'].Bearing.y - ch.Bearing.y) + ch.Size.y) * scale;
        Y = std::max(Y, ch_Y);
    }
    return {X, Y};
}

std::pair<float, float> TextRenderer::RenderText(const std::string& text,
                              float x,
                              float y,
                              float scale,
                              glm::vec3 color) {
    // Activate corresponding render state
    TextShader.Use();
    TextShader.SetVector3f("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    float initialX = x;
    float finalX = 0.0F;
    float initialY = y;
    float finalY = y;
    // Iterate through all characters
    std::string::const_iterator char_it;
    for (char_it = text.begin(); char_it != text.end(); char_it++) {
        Character ch = Characters[*char_it];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y + (Characters['H'].Bearing.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // Different characters have different finalY so take maximum
        finalY = std::max(finalY, ypos +h);
        // Update VBO for each character
        float vertices[6][4] = {{xpos, ypos + h, 0.0F, 1.0F},
                                {xpos + w, ypos, 1.0F, 0.0F},
                                {xpos, ypos, 0.0F, 0.0F},

                                {xpos, ypos + h, 0.0F, 1.0F},
                                {xpos + w, ypos + h, 1.0F, 1.0F},
                                {xpos + w, ypos, 1.0F, 0.0F}};
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER,
                        0,
                        sizeof(vertices),
                        vertices);  // Be sure to use glBufferSubData and not glBufferData
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph
        x += (ch.Advance >> 6) *
             scale;  // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    finalX = x;
    return {finalX - initialX, finalY - initialY};
}
