#include <project/resourceManager.h>
#include <project/menu.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

Menu::Menu() {
    glGenVertexArrays(2, vao);
    glGenBuffers(3, vbo);
    glGenBuffers(1, &ebo);

    // clang-format off
    float color[4] = {0.2, 0.2, 0.2, 0.75};
    float vertices_pos[] = {
        -1.0F, -1.0F, 0.8F, 1.0F,
        -1.0F, 01.0F, 0.8F, 1.0F,
        01.0F, 01.0F, 0.8F, 1.0F,
        01.0F, -1.0F, 0.8F, 1.0F
    };

    float vertices_color[] = {
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3]
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    // clang-format on

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_pos), vertices_pos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    float color_white[] = {1.0F, 1.0F, 1.0F, 1.0F};
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_color), vertices_color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Menu::draw(std::string shader) {
    ResourceManager::GetShader(shader).Use();
    glBindVertexArray(vao[0]);

    glm::mat4 model = glm::mat4(1.0F);
    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::mat4(1.0F);
    ResourceManager::GetShader(shader).SetMatrix4("model", model, true);
    ResourceManager::GetShader(shader).SetMatrix4("view", view, true);
    ResourceManager::GetShader(shader).SetMatrix4("projection", projection, true);

    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 0.9F);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(5);
    glBindVertexArray(vao[1]);
    model = glm::scale(model, glm::vec3(0.75F, 0.75F, 1.0F));
    ResourceManager::GetShader(shader).SetMatrix4("model", model, true);
    glDrawArrays(GL_LINE_LOOP, 0, 4);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glLineWidth(1);

    glBindVertexArray(0);
}
