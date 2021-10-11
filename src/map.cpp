#include <project/common.h>
#include <project/game.h>
#include <project/map.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <utility>

Map::Map() : gridSize({28, 36}), box({28, 31}), tileBox({2, 2}), gridVAO(0), gridVBO(0), text({0, 0}) {
    constexpr std::array<std::array<float, 2>, 4> texCoord= {{{0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}, {0.0F, 1.0F}}};
    box.setTexCoord(texCoord);
    constexpr std::array<std::array<float, 2>, 4> texCoordTile= {{{1.0F, 0.0F}, {0.0F, 0.0F}, {0.0F, 1.0F}, {1.0F, 1.0F}}};
    tileBox.setTexCoord(texCoord);

    auto windowSize = ResourceManager::getMainWindow()->getWindowSize();
    text = std::move(TextRenderer(windowSize.first, windowSize.second));
    text.Load(ResourceManager::resolvePath("resources/fonts/ARIAL.TTF"), 24);

    initializeGrid();

    glBindVertexArray(0);
}

void Map::draw(std::string shaderName) {
    auto shader = ResourceManager::GetShader(shaderName);

    glActiveTexture(GL_TEXTURE0);
    auto texture = ResourceManager::GetTexture("baseMap");
    texture.Bind(0);
    shader.SetInteger("texture1", 0, true);
    shader.SetFloat("textureColorMix", 0.0F);
    box.draw({13.5F, 15.0F, -0.9F}, shaderName);

    displayScore();
    displayLives(shaderName);
    displayLevel(shaderName);
}

void Map::draw(const std::string& shaderName, bool drawGrid) {
    draw(shaderName);
    if (drawGrid) {
        drawGridLines(shaderName);
    }
}

void Map::drawGridLines(const std::string& shaderName) const {
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(gridVAO);
    glm::mat4 model = glm::mat4(1.0F);
    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, -1.0F, 1.0F);
    shader.SetMatrix4("model", model);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 1.0F);
    glDrawArrays(GL_LINES, 0, (gridSize.first + gridSize.second) * 2);
}

void Map::displayScore() {
    int currentScore = ResourceManager::GetSprite<Pellet>("pellet")->getScore();
    std::string textToRender = "Score : " + std::to_string(currentScore);
    text.RenderText(textToRender, 15.0F, 15.0F, 1.0F);
}

void Map::displayLives(const std::string& shaderName) {
    auto shader = ResourceManager::GetShader(shaderName);
    auto texture = ResourceManager::GetTexture("pacman_1");
    texture.Bind(0);
    shader.SetInteger("texture1", 0, true);
    shader.SetFloat("textureColorMix", 0.0F);

    for (int i = 0, lives = Game::getState().getLives(); i < lives - 1; i++) {
        tileBox.draw({2.5F + (2.0F * i), 31.5F, 0.05F}, shaderName);
    }
}

void Map::displayLevel(const std::string& shaderName) {
    auto currentLevel = Game::getState().getLevel();

    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    shader.SetFloat("textureColorMix", 0.0F);
    auto texture2 = ResourceManager::GetTexture("cherry");
    texture2.Bind(1);
    shader.SetInteger("texture1", 1, true);
    tileBox.draw({24.5F, 31.5F, 0.05F}, shaderName);

    // tile coordinates multiplied to convert to pixel coordinates
    auto levelText = std::to_string(currentLevel);
    text.RenderText(levelText, 26.0F * 16, 34.5F * 16, 1.0F);
    glBindVertexArray(0);
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

char Map::getBlockType(const std::pair<int, int>& toCheck) {
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
    if (toCheck.first < 1) {
        if (abs(toCheck.second - 14) < 0.3) {
            return {DIRECTION::left, DIRECTION::right};
        } else {
            validCoord == false;
        }
    }
    if (toCheck.first > 26) {
        if (abs(toCheck.second - 14) < 0.3) {
            return {DIRECTION::right, DIRECTION::left};
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
