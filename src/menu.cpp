#include <project/resourceManager.h>
#include <project/menu.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

Menu::Menu() {
    glGenVertexArrays(3, vao);
    glGenBuffers(4, vbo);
    glGenBuffers(1, &ebo);

    // clang-format off
    float colorDark[4] = {0.0, 0.0, 0.0, 0.65f};
    float verticesPos[] = {
        -1.0F, -1.0F, 0.8F, 1.0F,
        -1.0F, 01.0F, 0.8F, 1.0F,
        01.0F, 01.0F, 0.8F, 1.0F,
        01.0F, -1.0F, 0.8F, 1.0F
    };
    float verticesColorDarken[] = {
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3],
        colorDark[0], colorDark[1], colorDark[2], colorDark[3]
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPos), verticesPos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorDarken), verticesColorDarken, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(vao[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    // clang-format off
    float colorWhite[] = {1.0F, 1.0F, 1.0F, 1.0F};
    float verticesColorWhite[] = {
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3],
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3],
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3],
        colorWhite[0], colorWhite[1], colorWhite[2], colorWhite[3]
    };
    // clang-format off
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesColorWhite), verticesColorWhite, GL_STATIC_DRAW);
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

    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 1.0F);
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
