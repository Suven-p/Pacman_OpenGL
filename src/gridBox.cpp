#include <project/gridBox.hpp>
#include <project/resourceManager.h>

GridBox::GridBox(const std::pair<float, float> &size):VAO(0), VBO{0, 0, 0}, EBO(0) {
    model = glm::mat4(1.0F);
    model = glm::translate(model, glm::vec3(0.5F, 3.5F, 0.0F));
    model = glm::scale(model, glm::vec3(size.first, size.second, 1.0F));
    view = glm::mat4(1.0F);
    projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, -1.0F, 1.0F);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(3, VBO);
    glGenBuffers(1, &EBO);

    float vertices[] = {
        -0.5F, 00.5F, 0.5F, 1.0F,
        00.5F, 00.5F, 0.5F, 1.0F,
        00.5F, -0.5F, 0.5F, 1.0F,
        -0.5F, -0.5F, 0.5F, 1.0F,
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void GridBox::setColor(const std::array<float, 4> &color) const {
    setColor({color, color, color, color});
}

void GridBox::setColor(const std::array<std::array<float, 4>, 4> &newColor) const {
    float color[] = {
        newColor[0][0], newColor[0][1], newColor[0][2], newColor[0][3],
        newColor[1][0], newColor[1][1], newColor[1][2], newColor[1][3],
        newColor[2][0], newColor[2][1], newColor[2][2], newColor[2][3],
        newColor[3][0], newColor[3][1], newColor[3][2], newColor[3][3],
    };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void GridBox::setTexCoord(const std::array<std::array<float, 2>, 4> &newTextureCoord) const {
    float texCoord[] = {
        newTextureCoord[0][0], newTextureCoord[0][1],
        newTextureCoord[1][0], newTextureCoord[1][1],
        newTextureCoord[2][0], newTextureCoord[2][1],
        newTextureCoord[3][0], newTextureCoord[3][1],
    };
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void GridBox::draw(const std::array<float, 3> &position, const std::string &shaderName) const {
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(VAO);

    auto temp_model = glm::mat4(1.0F);
    temp_model = glm::translate(temp_model, glm::vec3(position[0], position[1], position[2]));
    temp_model = temp_model * model;
    shader.SetMatrix4("model", temp_model);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
