#include <project/common.h>
#include <project/map.h>
#include <project/resourceManager.h>
#include <utility>

Map::Map() : gridSize({28, 36}) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    float vertices[] = {
        // positions              // colors               // texture coords
        28.0f, 03.0f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        28.0f, 34.0f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        00.0f, 34.0f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom left
        00.0f, 03.0f, -0.9f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0,
        1,
        3,  // first Triangle
        1,
        2,
        3  // second Triangle
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    initializeGrid();

    glGenVertexArrays(1, &blockVAO);
    glGenBuffers(1, &blockVBO);
    glGenBuffers(1, &blockEBO);
    float obstacleVertices[] = {
        // positions            // colors               // texture coords
        1.0f, 0.0f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top right
        1.0f, 1.0f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom right
        0.0f, 1.0f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        0.0f, 0.0f, -0.8f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f   // top left
    };
    glBindVertexArray(blockVAO);
    glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleVertices), obstacleVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Map::draw(std::string shaderName) {
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(VAO);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 36.0f, 0.0f, -1.0f, 1.0f);
    shader.SetMatrix4("model", model);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);

    glActiveTexture(GL_TEXTURE0);
    auto texture = ResourceManager::GetTexture("baseMap");
    texture.Bind(0);
    shader.SetInteger("texture1", 0, true);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Map::draw(std::string shaderName, bool drawGrid) {
    draw(shaderName);
    if (drawGrid) {
        drawGridLines(shaderName);
    }
}

void Map::drawGridLines(const std::string& shaderName) const {
    auto shader = ResourceManager::GetShader(shaderName);
    glBindVertexArray(gridVAO);
    shader.SetFloat("textureColorMix", 1.0f);
    glDrawArrays(GL_LINES, 0, (gridSize.first + gridSize.second) * 2);
}

void Map::initializeGrid() {
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    int count = 0;
    auto* vertices = new float[((gridSize.first + gridSize.second) * 16)]();
    for (int i = 1; i <= gridSize.first; i++) {
        vertices[count++] = i;
        vertices[count++] = 0.0f;
        vertices[count++] = 0.99f;
        vertices[count++] = 1.0f;

        vertices[count++] = 0.7f;
        vertices[count++] = 0.3f;
        vertices[count++] = 0.7f;
        vertices[count++] = 1.0f;

        vertices[count++] = i;
        vertices[count++] = 36.0f;
        vertices[count++] = 0.99f;
        vertices[count++] = 1.0f;

        vertices[count++] = 0.7f;
        vertices[count++] = 0.3f;
        vertices[count++] = 0.7f;
        vertices[count++] = 1.0f;
    }
    for (int i = 1; i <= gridSize.second; i++) {
        vertices[count++] = 0.0f;
        vertices[count++] = float(i);
        vertices[count++] = 0.99f;
        vertices[count++] = 1.0f;

        vertices[count++] = 0.7f;
        vertices[count++] = 0.3f;
        vertices[count++] = 0.7f;
        vertices[count++] = 1.0f;

        vertices[count++] = 28.0f;
        vertices[count++] = float(i);
        vertices[count++] = 0.99f;
        vertices[count++] = 1.0f;

        vertices[count++] = 0.7f;
        vertices[count++] = 0.3f;
        vertices[count++] = 0.7f;
        vertices[count++] = 1.0f;
    }
    glBindVertexArray(gridVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gridVBO);
    glBufferData(GL_ARRAY_BUFFER,
                 (gridSize.first + gridSize.second) * 16 * sizeof(float),
                 vertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)4);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    delete[] vertices;
    glBindVertexArray(0);
}

void Map::drawObstacles(const std::string& shaderName) const {
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(blockVAO);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 36.0f, 0.0f, -1.0f, 1.0f);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 0.0f);
    auto texture = ResourceManager::GetTexture("blinky");
    texture.Bind(0);
    shader.SetInteger("texture1", 0, true);

    glm::mat4 model;
    auto mapData = mapDataColRow;
    for (int i = 0; i < mapData.size(); i++) {
        for (int j = 0; j < mapData[i].size(); j++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
            model = glm::translate(model, glm::vec3(float(i), float(j), 0.0f));
            shader.SetMatrix4("model", model);
            switch (mapData[i][j]) {
                case 'G':
                case 'o':
                case 'n':
                case 'P':
                case 'W':
                    break;
                default:
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            }
        }
    }
    glBindVertexArray(0);
}

bool Map::checkObstacle(const std::pair<float, float>& toCheck) const {
    std::pair<int, int> position({toCheck.first, toCheck.second});
    if (position.first < 0 || position.first >= mapDataColRow.size()) {
        return true;
    }
    if (position.second < 0 || position.second >= mapDataColRow[0].size()) {
        return true;
    }
    auto block = mapDataColRow[position.first][position.second];
    return (block == MAP_WALL || block == MAP_GATE);
}

std::set<DIRECTION> Map::possibleDirections(const std::pair<float, float>& toCheck) const {
    std::set<DIRECTION> possible = {
        DIRECTION::up, DIRECTION::down, DIRECTION::left, DIRECTION::right};

    for (auto it = possible.begin(); it != possible.end();) {
        auto c = *it;
        bool deleted = false;
        switch (c) {
            case DIRECTION::up: {
                std::pair<float, float> newPosition = {toCheck.first, toCheck.second - 1};
                if (checkObstacle(newPosition)) {
                    it = possible.erase(it);
                    deleted = true;
                }
                break;
            }
            case DIRECTION::down: {
                std::pair<float, float> newPosition = {toCheck.first, toCheck.second + 1};
                if (checkObstacle(newPosition)) {
                    it = possible.erase(it);
                    deleted = true;
                }
                break;
            }
            case DIRECTION::left: {
                std::pair<float, float> newPosition = {toCheck.first - 1, toCheck.second};
                if (checkObstacle(newPosition)) {
                    it = possible.erase(it);
                    deleted = true;
                }
                break;
            }
            case DIRECTION::right: {
                std::pair<float, float> newPosition = {toCheck.first + 1, toCheck.second};
                if (checkObstacle(newPosition)) {
                    it = possible.erase(it);
                    deleted = true;
                }
                break;
            }
        }
        // std::set<>.erase increments the pointer so increment iff no elements
        // was deleted
        if (!deleted)
            it++;
    }
    return possible;
}
