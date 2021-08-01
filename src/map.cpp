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
        28.0F, 03.0F, -0.9F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F,  // top right
        28.0F, 34.0F, -0.9F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F,  // bottom right
        00.0F, 34.0F, -0.9F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F,  // bottom left
        00.0F, 03.0F, -0.9F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F   // top left
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
        1.0F, 0.0F, -0.8F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 1.0F,  // top right
        1.0F, 1.0F, -0.8F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 1.0F, 0.0F,  // bottom right
        0.0F, 1.0F, -0.8F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F,  // bottom left
        0.0F, 0.0F, -0.8F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F   // top left
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

    glm::mat4 model = glm::mat4(1.0F);
    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, -1.0F, 1.0F);
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

void Map::draw(const std::string& shaderName, bool drawGrid) {
    draw(shaderName);
    if (drawGrid) {
        drawGridLines(shaderName);
    }
}

void Map::drawGridLines(const std::string& shaderName) const {
    auto shader = ResourceManager::GetShader(shaderName);
    glBindVertexArray(gridVAO);
    shader.SetFloat("textureColorMix", 1.0F);
    glDrawArrays(GL_LINES, 0, (gridSize.first + gridSize.second) * 2);
}

void Map::initializeGrid() {
    glGenVertexArrays(1, &gridVAO);
    glGenBuffers(1, &gridVBO);
    int count = 0;
    auto* vertices = new float[((gridSize.first + gridSize.second) * 16)]();
    for (int i = 1; i <= gridSize.first; i++) {
        vertices[count++] = i;
        vertices[count++] = 0.0F;
        vertices[count++] = 0.99F;
        vertices[count++] = 1.0F;

        vertices[count++] = 0.7F;
        vertices[count++] = 0.3F;
        vertices[count++] = 0.7F;
        vertices[count++] = 1.0F;

        vertices[count++] = i;
        vertices[count++] = 36.0F;
        vertices[count++] = 0.99F;
        vertices[count++] = 1.0F;

        vertices[count++] = 0.7F;
        vertices[count++] = 0.3F;
        vertices[count++] = 0.7F;
        vertices[count++] = 1.0F;
    }
    for (int i = 1; i <= gridSize.second; i++) {
        vertices[count++] = 0.0F;
        vertices[count++] = float(i);
        vertices[count++] = 0.99F;
        vertices[count++] = 1.0F;

        vertices[count++] = 0.7F;
        vertices[count++] = 0.3F;
        vertices[count++] = 0.7F;
        vertices[count++] = 1.0F;

        vertices[count++] = 28.0F;
        vertices[count++] = float(i);
        vertices[count++] = 0.99F;
        vertices[count++] = 1.0F;

        vertices[count++] = 0.7F;
        vertices[count++] = 0.3F;
        vertices[count++] = 0.7F;
        vertices[count++] = 1.0F;
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

    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, -1.0F, 1.0F);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 0.0F);
    auto texture = ResourceManager::GetTexture("blinky");
    texture.Bind(0);
    shader.SetInteger("texture1", 0, true);

    glm::mat4 model;
    auto mapData = mapDataColRow;
    for (int i = 0; i < mapData.size(); i++) {
        for (int j = 0; j < mapData[i].size(); j++) {
            model = glm::mat4(1.0F);
            model = glm::translate(model, glm::vec3(0.0F, 3.0F, 0.0F));
            model = glm::translate(model, glm::vec3(float(i), float(j), 0.0F));
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

bool Map::checkObstacle(const std::pair<float, float>& toCheck,
                        const std::set<char>& obstacles) const {
    std::pair<int, int> position({toCheck.first, toCheck.second});
    if (position.first < 0 || position.first >= mapDataColRow.size()) {
        return true;
    }
    if (position.second < 0 || position.second >= mapDataColRow[0].size()) {
        return true;
    }
    auto block = mapDataColRow[position.first][position.second];
    return (obstacles.count(block) > 0);
}

char Map::getBlockType(const std::pair<int, int>& toCheck) const {
    if (toCheck.first < 0 || toCheck.first >= mapDataColRow.size()) {
        return MAP_OUTOFBOUNDS;
    }
    if (toCheck.second < 0 || toCheck.second >= mapDataColRow[0].size()) {
        return MAP_OUTOFBOUNDS;
    }
    auto block = mapDataColRow[toCheck.first][toCheck.second];
    return block;
}

std::set<DIRECTION> Map::possibleDirections(const std::pair<float, float>& toCheck,
                                            const std::set<char>& obstacles) const {
    std::set<DIRECTION> possible = {
        DIRECTION::up, DIRECTION::down, DIRECTION::left, DIRECTION::right};
    bool validCoord = true;
    if (toCheck.second < 0 || toCheck.second > 30) {
        validCoord = false;
    }
    if (toCheck.first < 0) {
        if (abs(toCheck.second - 14) < 0.3) {
            return {DIRECTION::left};
        } else {
            validCoord == false;
        }
    }
    if (toCheck.first > 27) {
        if (abs(toCheck.second - 14) < 0.3) {
            return {DIRECTION::right};
        } else {
            validCoord == false;
        }
    }
    if (!validCoord) {
        spdlog::error("Map::Something went wrong."
                      "Position change requested for invalid coordinates: {}, {}",
                      toCheck.first,
                      toCheck.second);
        return {};
    }
    for (auto it = possible.begin(); it != possible.end();) {
        auto c = *it;
        bool deleted = false;
        switch (c) {
            case DIRECTION::up: {
                std::pair<float, float> newPosition = {toCheck.first, toCheck.second - 1};
                if (checkObstacle(newPosition, obstacles)) {
                    it = possible.erase(it);
                    deleted = true;
                }
                break;
            }
            case DIRECTION::down: {
                std::pair<float, float> newPosition = {toCheck.first, toCheck.second + 1};
                if (checkObstacle(newPosition, obstacles)) {
                    it = possible.erase(it);
                    deleted = true;
                }
                break;
            }
            case DIRECTION::left: {
                std::pair<float, float> newPosition = {toCheck.first - 1, toCheck.second};
                if (checkObstacle(newPosition, obstacles)) {
                    it = possible.erase(it);
                    deleted = true;
                }
                break;
            }
            case DIRECTION::right: {
                std::pair<float, float> newPosition = {toCheck.first + 1, toCheck.second};
                if (checkObstacle(newPosition, obstacles)) {
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
