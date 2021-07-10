#include <project/helpers.h>
#include <project/common.h>
#include <project/resourceManager.h>

void draw_gridlines()
{
    std::pair<int, int> gridSize(28, 36);
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    int count = 0;
    float *vertices = new float[(gridSize.first) * 16]();
    for (int i = 1; i <= gridSize.first; i++)
    {
        vertices[count++] = i;
        vertices[count++] = 0.0f;
        vertices[count++] = 0.2f;
        vertices[count++] = 1.0f;

        vertices[count++] = 1.0f;
        vertices[count++] = 1.0f;
        vertices[count++] = 0.0f;
        vertices[count++] = 1.0f;

        vertices[count++] = i;
        vertices[count++] = 36.0f;
        vertices[count++] = 0.2f;
        vertices[count++] = 1.0f;

        vertices[count++] = 1.0f;
        vertices[count++] = 1.0f;
        vertices[count++] = 0.0f;
        vertices[count++] = 1.0f;
    }
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (gridSize.first) * 16 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)4);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    ResourceManager::GetShader("mainShader").Use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 36.0f, 0.0f, 1.0f, -1.0f);
    ResourceManager::GetShader("mainShader").SetMatrix4("model", model, true);
    ResourceManager::GetShader("mainShader").SetMatrix4("view", view, true);
    ResourceManager::GetShader("mainShader").SetMatrix4("projection", projection, true);
    ResourceManager::GetShader("mainShader").SetFloat("textureColorMix", 1.0f);
    glDrawArrays(GL_LINES, 0, (gridSize.first) * 2);
    delete[] vertices;

    vertices = new float[(gridSize.second) * 16]();
    count = 0;
    for (int i = 1; i <= gridSize.second; i++)
    {
        vertices[count++] = 0.0f;
        vertices[count++] = float(i);
        vertices[count++] = 0.2f;
        vertices[count++] = 1.0f;

        vertices[count++] = 1.0f;
        vertices[count++] = 1.0f;
        vertices[count++] = 0.0f;
        vertices[count++] = 1.0f;

        vertices[count++] = 28.0f;
        vertices[count++] = float(i);
        vertices[count++] = 0.2f;
        vertices[count++] = 1.0f;

        vertices[count++] = 1.0f;
        vertices[count++] = 1.0f;
        vertices[count++] = 0.0f;
        vertices[count++] = 1.0f;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (gridSize.second) * 16 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)4);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_LINES, 0, (gridSize.second) * 2);
    delete[] vertices;

    glBindVertexArray(0);
}