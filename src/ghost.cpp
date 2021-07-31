#include <project/common.h>
#include <project/game.h>
#include <project/ghost.h>
#include <project/pacman.h>
#include <project/resourceManager.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <cmath>
#include <random>
#include <utility>

using namespace std;

std::random_device rd;   // Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()

Ghost::Ghost(const std::string& name) : name(name) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);
    glGenBuffers(1, &ebo);

    float vertices[] = {-0.5F, -0.5F, 0.5F, 1.0F,  0.0F, 0.0F, 0.0F,  1.0F, -0.5F, 01.5F, 0.5F,
                        1.0F,  0.0F,  0.0F, 0.0F,  1.0F, 1.5F, 01.5F, 0.5F, 1.0F,  0.0F,  0.0F,
                        0.0F,  1.0F,  1.5F, -0.5F, 0.5F, 1.0F, 0.0F,  0.0F, 0.0F,  1.0F};

    float texCoord[] = {0.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F, 1.0F, 1.0F};

    GLuint indices[] = {0, 1, 2, 0, 2, 3};

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(2);

    currentDirection = DIRECTION::right;
    nextDirection = DIRECTION::right;
    currentMode = GhostMode::chase;

    std::string loggerName = std::string("ghostLogger::") + name;
    logger = spdlog::basic_logger_mt(loggerName, "logs/ghostLog.txt");
    logger->set_level(spdlog::level::trace);
}

Ghost::~Ghost() = default;

void Ghost::draw(std::string shader) {
    getNewPosition();
    ResourceManager::GetShader(shader).Use();
    glBindVertexArray(vao);
    glm::mat4 model = glm::mat4(1.0F);
    model = glm::translate(model, glm::vec3(0.0F, 3.0F, 0.0F));
    glm::mat4 view(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, -1.0F, 1.0F);
    ResourceManager::GetShader(shader).SetMatrix4("view", view, true);
    ResourceManager::GetShader(shader).SetMatrix4("projection", projection, true);

    // This separation of temp_model from model was done so all ghosts could be
    // rendered in same call. This is not required as each ghost has its own
    // object.
    glm::mat4 temp_model = glm::mat4(1.0F);
    temp_model = glm::translate(model, glm::vec3(position.first, position.second, 0.0F));

    ResourceManager::GetShader(shader).SetMatrix4("model", temp_model, true);

    auto texture = ResourceManager::GetTexture(name);
    texture.Bind(0);
    ResourceManager::GetShader(shader).SetInteger("texture1", 0, true);
    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 0.0F);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);

    drawEyes(shader);

    glBindVertexArray(0);
}

void Ghost::drawEyes(const std::string& shader) {
    Texture2D texture;
    switch (currentDirection) {
        case DIRECTION::up:
            texture = ResourceManager::GetTexture("eyesUp");
            break;
        case DIRECTION::down:
            texture = ResourceManager::GetTexture("eyesDown");
            break;
        case DIRECTION::right:
            texture = ResourceManager::GetTexture("eyesRight");
            break;
        case DIRECTION::left:
            texture = ResourceManager::GetTexture("eyesLeft");
            break;
    }

    texture.Bind(0);
    ResourceManager::GetShader(shader).SetInteger("texture1", 0, true);
    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 0.0F);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);
}

DIRECTION Ghost::setNextDirection() {
    // TODO: get target tile for chase mode
    std::pair<int, int> nextTile;
    DIRECTION oppositeDirection;
    switch (currentDirection) {
        case DIRECTION::up: {
            nextTile = {position.first, std::ceil(position.second) - 1};
            oppositeDirection = DIRECTION::down;
            break;
        }
        case DIRECTION::down: {
            nextTile = {position.first, position.second + 1};
            oppositeDirection = DIRECTION::up;
            break;
        }
        case DIRECTION::left: {
            nextTile = {std::ceil(position.first) - 1, position.second};
            oppositeDirection = DIRECTION::right;
            break;
        }
        case DIRECTION::right: {
            nextTile = {position.first + 1, position.second};
            oppositeDirection = DIRECTION::left;
            break;
        }
    }
    auto baseMapPtr = std::dynamic_pointer_cast<Map>(ResourceManager::GetSprite("baseMap"));
    auto possible = baseMapPtr->possibleDirections(nextTile);
    possible.erase(oppositeDirection);
    logger->trace("Next tile is: {}, {}", nextTile.first, nextTile.second);

    if (possible.empty()) {
        return nextDirection;
    }
    if (currentMode == GhostMode::frightened) {
        std::uniform_int_distribution<> distrib(0, possible.size() - 1);
        auto it = possible.begin();
        auto index = distrib(gen);
        std::advance(it, index);
        nextDirection = *it;
    } else {
        if (currentMode == GhostMode::scatter) {
            if (name == "blinky") {
                targetTile = {25, -3};
            } else if (name == "inky") {
                targetTile = {27, 32};
            } else if (name == "pinky") {
                targetTile = {2, -3};
            } else if (name == "clyde") {
                targetTile = {0, 32};
            }
        } else if (currentMode == GhostMode::chase) {
            auto pacmanPtr = getPacmanPtr();
            DIRECTION pacmanDirection = pacmanPtr->getDirection();
            auto pacmanPosition = pacmanPtr->getPosition();
            targetTile = pacmanPosition;
            if (name == "blinky") {
                // target already set to pacman postion
            } else if (name == "inky") {
                auto blinkyPtr = ResourceManager::GetSprite("blinky");
                auto blinkyPosition = blinkyPtr->getPosition();
                switch (pacmanDirection) {
                    case DIRECTION::up:
                        pacmanPosition.second -= 2;
                        break;
                    case DIRECTION::down:
                        pacmanPosition.second += 2;
                        break;
                    case DIRECTION::left:
                        pacmanPosition.first -= 2;
                        break;
                    case DIRECTION::right:
                        pacmanPosition.first += 2;
                        break;
                }
                float xdist = pacmanPosition.first - blinkyPosition.first;
                float ydist = pacmanPosition.second - blinkyPosition.second;
                targetTile = {blinkyPosition.first + 2 * xdist, blinkyPosition.second + 2 * ydist};
            } else if (name == "pinky") {
                switch (pacmanDirection) {
                    case DIRECTION::up:
                        targetTile.second -= 3;
                        break;
                    case DIRECTION::down:
                        targetTile.second += 3;
                        break;
                    case DIRECTION::left:
                        targetTile.first -= 3;
                        break;
                    case DIRECTION::right:
                        targetTile.first += 3;
                        break;
                }
            } else if (name == "clyde") {
                float dist2 = sqrt(pow((position.first - pacmanPosition.first), 2) +
                                   pow((position.second - pacmanPosition.second), 2));
                if (dist2 <= 8) {
                    targetTile = {0, 32};
                }
                // else target already set to pacman position
            }
        }
        logger->trace("Target Tile is: {} {}", targetTile.first, targetTile.second);
        float minValue = 1e9;
        // Note: The current pacman map is guaranteed to have at least one possible direction
        DIRECTION bestDirection = *possible.begin();
        std::map<DIRECTION, int> priorityOrder = {
            {DIRECTION::up, 4}, {DIRECTION::left, 3}, {DIRECTION::down, 2}, {DIRECTION::right, 1}};
        for (auto c : possible) {
            std::pair<float, float> newPosition;
            switch (c) {
                case DIRECTION::up: {
                    newPosition = {nextTile.first, nextTile.second - 1};
                    break;
                }
                case DIRECTION::down: {
                    newPosition = {nextTile.first, nextTile.second + 1};
                    break;
                }
                case DIRECTION::left: {
                    newPosition = {nextTile.first - 1, nextTile.second};
                    break;
                }
                case DIRECTION::right: {
                    newPosition = {nextTile.first + 1, nextTile.second};
                    break;
                }
            }
            float distance = sqrt(pow((targetTile.first - newPosition.first), 2) +
                                  pow((targetTile.second - newPosition.second), 2));
            logger->trace("New position is {} {}", newPosition.first, newPosition.second);
            logger->trace("Distance in direction {} is {}", toString(c), distance);
            if (distance < minValue) {
                logger->trace("Updating direction to {} based on value.", toString(c));
                minValue = distance;
                bestDirection = c;
            } else if (distance == minValue) {
                logger->trace("Updating direction to {} based on priority.", toString(c));
                bestDirection = priorityOrder[c] > priorityOrder[bestDirection] ? c : bestDirection;
            }
        }
        nextDirection = bestDirection;
        logger->trace("Best direction is set to be: {}", toString(bestDirection));
    }
    return nextDirection;
}

void Ghost::getNewPosition() {
    float diffPixels = Game::getInstance()->getSpeed() * Game::getInstance()->getTime() * 0.75;
    auto oldPosition = position;
    bool reachedNewTile = false;
    switch (currentDirection) {
        case DIRECTION::left: {
            position.first -= diffPixels;
            if (oldPosition.first > std::ceil(position.first) and
                position.first <= std::ceil(position.first)) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::up || nextDirection == DIRECTION::down) {
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
                if (nextDirection == DIRECTION::left || nextDirection == DIRECTION::right) {
                    position.second = std::ceil(position.second);
                }
            }
            break;
        }
        case DIRECTION::right: {
            position.first += diffPixels;
            if ((int(position.first) - int(oldPosition.first)) == 1) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::up || nextDirection == DIRECTION::down) {
                    position.first = int(position.first);
                }
            }
            break;
        }
        case DIRECTION::down: {
            position.second += diffPixels;
            if ((int(position.second) - int(oldPosition.second)) == 1) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::left || nextDirection == DIRECTION::right) {
                    position.second = int(position.second);
                }
            }
            break;
        }
    }
    if (reachedNewTile) {
        logger->trace("Current position is: {} {}", position.first, position.second);
        logger->trace("Current direction is: {}", toString(currentDirection));
        logger->trace("Switching direction to {}", toString(nextDirection));
        if (int(position.second) == 14) {
            if (int(position.first) <= -2 && currentDirection == DIRECTION::left) {
                position.first = 30;
            } else if (position.first > 30 && currentDirection == DIRECTION::right) {
                position.first = -2;
            } else {
                currentDirection = nextDirection;
                setNextDirection();
            }
        } else {
            currentDirection = nextDirection;
            setNextDirection();
        }
    };
}

GhostMode Ghost::getMode() {
    return currentMode;
}

void Ghost::setMode(GhostMode newMode) {
    currentMode = newMode;
}
