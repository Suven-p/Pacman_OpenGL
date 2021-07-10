#include <project/common.h>
#include <project/map.h>
#include <project/resourceManager.h>

Map::Map() : gridSize({28, 36})
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    float vertices[] = {
        // positions              // colors               // texture coords
        28.0f, 03.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        28.0f, 34.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        00.0f, 34.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        00.0f, 03.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)(4 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    initializeGrid();
}

void Map::draw(std::string shaderName)
{
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 36.0f, 0.0f, 1.0f, -1.0f);
    shader.SetMatrix4("model", model);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    auto texture = ResourceManager::GetTexture("baseMap");
    texture.Bind(0);
    shader.SetInteger("texture1", 0, true);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Map::draw(std::string shaderName, bool drawGrid)
{
    draw(shaderName);
    if (drawGrid)
    {
        drawGridLines(shaderName);
    }
}

void Map::drawGridLines(std::string shaderName)
{
    auto shader = ResourceManager::GetShader(shaderName);
    glBindVertexArray(gridVAO);
    shader.SetFloat("textureColorMix", 1.0f);
    glDrawArrays(GL_LINES, 0, (gridSize.first + gridSize.second) * 2);
}

void Map::initializeGrid()
{
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    int count = 0;
    float *vertices = new float[((gridSize.first + gridSize.second) * 16)]();
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
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER, (gridSize.first + gridSize.second) * 16 * sizeof(float), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)4);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    delete[] vertices;
    glBindVertexArray(0);
}
