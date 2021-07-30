#include <math.h>
#include <project/common.h>
#include <project/game.h>
#include <project/pacman.h>
#include <project/resourceManager.h>
#include <iostream>
#include <iterator>
#include <set>
Pacman::Pacman() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);
    glGenBuffers(1, &ebo);

    float vertices[] = {-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        -0.5f, 01.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        01.5f, 01.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                        01.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};

    float texCoord[] = {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(
        0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          8 * sizeof(float),
                          (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(
        2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(2);

    currentDirection = DIRECTION::right;
    nextDirection = DIRECTION::right;
}

void Pacman::draw(std::string shader) {
    getNewPosition();
    ResourceManager::GetShader(shader).Use();
    glBindVertexArray(vao);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
    glm::mat4 view(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 36.0f, 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader(shader).SetMatrix4("view", view, true);
    ResourceManager::GetShader(shader).SetMatrix4(
        "projection", projection, true);

    glm::mat4 temp_model = glm::mat4(1.0f);
    temp_model =
        glm::translate(model, glm::vec3(position.first, position.second, 0.0f));
    // translate to center of square, rotate and untranslate
    temp_model = glm::translate(temp_model, glm::vec3(0.5f, 0.5f, 0.0f));
    temp_model = glm::rotate(temp_model,
                             glm::radians(int(currentDirection) * 90.0f),
                             glm::vec3(0.0, 0.0, 1.0));
    temp_model = glm::translate(temp_model, glm::vec3(-0.5f, -0.5f, 0.0f));

    ResourceManager::GetShader(shader).SetMatrix4("model", temp_model, true);

    auto texture = ResourceManager::GetTexture("pacman");
    texture.Bind(0);
    ResourceManager::GetShader(shader).SetInteger("texture1", 0, true);
    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    glBindVertexArray(0);
}
DIRECTION Pacman::getDirection() {
    return currentDirection;
}
void Pacman::setDirection(DIRECTION newDirection) {
    // dont change direction if collides in the new direction
    if (isColliding(newDirection)) {
        return;
    } else {
        // prevent clipping
        if (newDirection == DIRECTION::right ||
            newDirection == DIRECTION::left) {
            // for smoother direction change
            if (abs(position.second - int(position.second) > 0.15)) {
                return;
            }
            position.second = int(position.second);
            // fixes pacman stuck in edges
            if (oldPosition.second < position.second) {
                return;
            }
        } else {
            // for smoother direction change
            if (abs(position.first - int(position.first) > 0.15)) {
                return;
            }
            position.first = int(position.first);
            if (oldPosition.first < position.first) {
                return;
            }
        }
        currentDirection = newDirection;
    }
}
void Pacman::setNextDirection(DIRECTION newDirection) {
    nextDirection = newDirection;
}
bool Pacman::isColliding(DIRECTION aDirection) {
    auto baseMapPtr =
        std::dynamic_pointer_cast<Map>(ResourceManager::GetSprite("baseMap"));
    auto possible =
        baseMapPtr->possibleDirections(std::pair<int, int>(oldPosition));
    bool collision = true;

    for (auto itr = possible.begin(); itr != possible.end(); ++itr) {
        if (*itr == aDirection) {
            collision = false;
        }
    }
    return collision;
}
std::shared_ptr<Pacman> getPacmanPtr() {
    return std::dynamic_pointer_cast<Pacman>(
        ResourceManager::GetSprite("pacman"));
}

void Pacman::getNewPosition() {
    float diffPixels =
        Game::getInstance()->getSpeed() * Game::getInstance()->getTime() * 0.8;
    // float diffPixels = Game::getInstance()->getSpeed() *
    // Game::getInstance()->getTime() * getMultiplier();
    // float diffPixels = Game::getInstance()->getSpeed() * 16 * 0.75;
    oldPosition = position;
    bool reachedNewTile = false;
    switch (currentDirection) {
        case DIRECTION::left: {
            position.first -= diffPixels;
            if (oldPosition.first > std::ceil(position.first) and
                position.first <= std::ceil(position.first)) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::up ||
                    nextDirection == DIRECTION::down) {
                    position.first = std::ceil(position.first);
                }
            }
            break;
        }
        case DIRECTION::up: {
            position.second -= diffPixels;
            if (oldPosition.second > std::ceil(position.second) and
                position.second <= std::ceil(position.second)) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::left ||
                    nextDirection == DIRECTION::right) {
                    position.second = std::ceil(position.second);
                }
            }
            break;
        }
        case DIRECTION::right: {
            position.first += diffPixels;
            if ((int(position.first) - int(oldPosition.first)) == 1) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::up ||
                    nextDirection == DIRECTION::down) {
                    position.first = int(position.first);
                }
            }
            break;
        }
        case DIRECTION::down: {
            position.second += diffPixels;
            if ((int(position.second) - int(oldPosition.second)) == 1) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::left ||
                    nextDirection == DIRECTION::right) {
                    position.second = int(position.second);
                }
            }
            break;
        }
    }
    // std::cout<<position.first<<","<<position.second<<"\t"<<oldPosition.first<<","<<oldPosition.second<<std::endl;
    if (int(position.second) == 14) {
        if (int(position.first) <= 0 && currentDirection == DIRECTION::left) {
            position.first = 28;
        } else if (position.first > 27 &&
                   currentDirection == DIRECTION::right) {
            position.first = -1;
        }
    }
    bool collision = isColliding(currentDirection);
    if (collision) {
        // Prevent sudden tile change in left and up direction movement
        if (currentDirection == DIRECTION::left) {
            if (position.first < int(oldPosition.first)) {
                position = std::pair<int, int>(oldPosition);
            }
        } else if (currentDirection == DIRECTION::up) {
            if (position.second < int(oldPosition.second)) {
                position = std::pair<int, int>(oldPosition);
            }
        } else {
            position = std::pair<int, int>(oldPosition);
        }
    }
    setDirection(nextDirection);
}

void Pacman::setMultiplier(float mul = 0.8) {
    multiplier = mul;
}
float Pacman::getMultiplier() {
    return multiplier;
}