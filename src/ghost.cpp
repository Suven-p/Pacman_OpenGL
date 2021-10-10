#include <project/common.h>
#include <project/game.h>
#include <project/ghost.h>
#include <project/gameLogic.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <cmath>
#include <random>
#include <utility>

using namespace std;

const std::map<std::string, std::pair<float, float>> Ghost::positionInPen = {{"blinky", {13, 14}},
                                                                             {"inky", {12, 14}},
                                                                             {"pinky", {13, 14}},
                                                                             {"clyde", {15, 14}}};
const std::map<std::string, std::pair<float, float>> Ghost::initialPosition = {
    {"blinky", {13.5, 11}},
    {"inky", {11.5, 14}},
    {"pinky", {13.5, 14}},
    {"clyde", {15.5, 14}}};
const std::map<std::string, int> Ghost::timeToLeave = {{"blinky", 0},
                                                       {"pinky", 2000},
                                                       {"inky", 7000},
                                                       {"clyde", 12000}};

std::random_device rd;   // Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with rd()

Ghost::Ghost(const std::string& name) : name(name), box(GridBox({2, 2})) {
    constexpr std::array<std::array<float, 2>, 4> texCoord= {{{0.0F, 0.0F}, {1.0F, 0.0F}, {1.0F, 1.0F}, {0.0F, 1.0F}}};
    box.setTexCoord(texCoord);

    position = initialPosition.at(name);
    currentDirection = (name == "blinky") ? DIRECTION::right : DIRECTION::up;
    nextDirection = DIRECTION::right;
    currentMode = GhostMode::scatter;

    setMultiplier(0.75);

    std::string loggerName = std::string("ghostLogger::") + name;
    logger = spdlog::basic_logger_mt(loggerName, "logs/ghostLog.txt");
    logger->set_level(spdlog::level::debug);

    outOfPen = false;
}

void Ghost::draw(std::string shader) {
    ghostTimer.start();
    recalculatePosition();

    if (currentMode != GhostMode::dead) {
        std::string textureToUse = (currentMode == GhostMode::frightened) ? "frightened" : name;
        auto texture = ResourceManager::GetTexture(textureToUse);
        texture.Bind(0);
        ResourceManager::GetShader(shader).SetInteger("texture1", 0, true);
        ResourceManager::GetShader(shader).SetFloat("textureColorMix", 0.0F);
        box.draw({position.first, position.second, 0.5F}, shader);
    }
    if (currentMode != GhostMode::frightened) {
        drawEyes(shader);
    }
    glBindVertexArray(0);
}

void Ghost::drawEyes(const std::string& shader) const {
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
    box.draw({position.first, position.second, 0.5F}, shader);
}

void Ghost::recalculatePosition() {
    if (Game::getState().isPaused() || !Game::getState().isStarted() || Game::getState().isReady()) {
        ghostTimer.pause();
        return;
    }
    ghostTimer.resume();
    bool ghostInPen = (position.first >= 11 && position.first <= 16 && position.second > 11 &&
                       position.second <= 15);
    calculateMultiplier();
    if (!outOfPen) {
        initialMovement();
        return;
    }
    if (ghostInPen || currentMode != GhostMode::dead) {
        deadModeMovement(ghostInPen);
    } else {
        basicMovement();
    }
}

void Ghost::initialMovement() {
    if (ghostTimer.timeElapsed() < timeToLeave.at(name)) {
        if (position.second >= 14.5) {
            currentDirection = DIRECTION::up;
        } else if (position.second <= 13.5) {
            currentDirection = DIRECTION::down;
        }
        basicMovement();
    } else {
        outOfPen = true;
    }
}

void Ghost::calculateMultiplier() {
    auto levelData = Game::getState().getLevelData();
    auto pelletPtr = ResourceManager::GetSprite<Pellet>("pellet");
    if ((position.first <= 4 || position.first >= 23) && position.second == 14) {
        setMultiplier(levelData["ghostTunnelSpeed"].get<float>());
        return;
    }
    if (name == "blinky" &&
        (currentMode != GhostMode::frightened && currentMode != GhostMode::dead)) {
        if ((244 - pelletPtr->getPelletsEaten()) <= levelData["elroy2Dots"].get<int>()) {
            setMultiplier(levelData["elroy2Speed"].get<float>());
            return;
        }
        if ((244 - pelletPtr->getPelletsEaten()) <= levelData["elroy1Dots"].get<int>()) {
            setMultiplier(levelData["elroy1Speed"].get<float>());
            return;
        }
    }
    bool ghostInPen = (position.first >= 11 && position.first <= 16 && position.second >= 12 &&
                       position.second <= 15);
    if (ghostInPen) {
        setMultiplier(levelData["ghostPenSpeed"].get<float>());
        return;
    }
    switch (currentMode) {
        case GhostMode::frightened:
            setMultiplier(levelData["ghostFrightSpeed"].get<float>());
            return;
        case GhostMode::scatter:
        case GhostMode::chase:
            setMultiplier(levelData["ghostSpeed"].get<float>());
            return;
        case GhostMode::dead:
            setMultiplier(1.2);
    }
}

void Ghost::basicMovement() {
    float diffPixels = Game::getSpeed() * Game::getTime() * speedMultiplier;
    auto oldPosition = position;
    position = addPosition(position, diffPixels, currentDirection);
    // reachedNewTile is true iff ghost completely overlaps with new tile
    bool reachedNewTile = false;
    switch (currentDirection) {
        case DIRECTION::left: {
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
            if ((int(position.first) - int(oldPosition.first)) == 1) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::up || nextDirection == DIRECTION::down) {
                    position.first = int(position.first);
                }
            }
            break;
        }
        case DIRECTION::down: {
            if ((int(position.second) - int(oldPosition.second)) == 1) {
                reachedNewTile = true;
                if (nextDirection == DIRECTION::left || nextDirection == DIRECTION::right) {
                    position.second = int(position.second);
                }
            }
            break;
        }
    }
    if (!reachedNewTile || !outOfPen) {
        return;
    }

    logger->trace("Current position is: {} {}", position.first, position.second);
    logger->trace("Current direction is: {}", toString(currentDirection));
    logger->trace("Switching direction to {}", toString(nextDirection));

    if (currentMode == GhostMode::dead && position == targetTile) {
        setMode(ResourceManager::GetSprite<GameLogic>("gameLogic")->getMode());
    }
    // Adjustment for tunnel passthrough
    handleSpecialZone();
    currentDirection = nextDirection;
    setNextDirection();
}

void Ghost::deadModeMovement(bool ghostInPen) {
    // TODO: Maybe move into monster pen?
    if (ghostInPen && currentMode != GhostMode::dead) {
        MoveOutofPen();
    } else {
        basicMovement();
    }
}

void Ghost::handleSpecialZone() {
    if (position.second == 14) {
        if (position.first <= -2) {
            position.first = 30;
        } else if (position.first > 30) {
            position.first = -2;
        }
    }
}

DIRECTION Ghost::setNextDirection() {
    std::pair<int, int> nextTile;
    switch (currentDirection) {
        case DIRECTION::up: {
            nextTile = {position.first, std::ceil(position.second) - 1};
            break;
        }
        case DIRECTION::down: {
            nextTile = {position.first, position.second + 1};
            break;
        }
        case DIRECTION::left: {
            nextTile = {std::ceil(position.first) - 1, position.second};
            break;
        }
        case DIRECTION::right: {
            nextTile = {position.first + 1, position.second};
            break;
        }
    }
    logger->trace("Next tile is: {}, {}", nextTile.first, nextTile.second);

    if (currentMode == GhostMode::frightened) {
        auto baseMapPtr = ResourceManager::GetSprite<Map>("baseMap");
        auto possible = baseMapPtr->possibleDirections(nextTile);
        possible.erase(getOppositeDirection(currentDirection));
        std::uniform_int_distribution<int> distrib(0, possible.size() - 1);
        auto it = possible.begin();
        auto index = distrib(gen);
        std::advance(it, index);
        nextDirection = *it;
    } else {
        selectTargetTile();
        nextDirection = selectBestDirection(nextTile, targetTile);
    }
    logger->info("Target Tile is: {} {}", targetTile.first, targetTile.second);
    logger->trace("Best direction is set to be: {}", toString(nextDirection));
    return nextDirection;
}

void Ghost::selectTargetTile() {
    switch (currentMode) {
        case GhostMode::chase: {
            auto pacmanPtr = ResourceManager::GetSprite<Pacman>("pacman");
            DIRECTION pacmanDirection = pacmanPtr->getDirection();
            auto pacmanPosition = pacmanPtr->getPosition();
            targetTile = pacmanPosition;
            if (name == "blinky") {
                targetTile = pacmanPosition;
            } else if (name == "inky") {
                auto blinkyPtr = ResourceManager::GetSprite("blinky");
                auto blinkyPosition = blinkyPtr->getPosition();
                pacmanPosition = addPosition(pacmanPosition, 2, pacmanDirection);
                auto xdist = pacmanPosition.first - blinkyPosition.first;
                auto ydist = pacmanPosition.second - blinkyPosition.second;
                targetTile = {blinkyPosition.first + 2 * xdist, blinkyPosition.second + 2 * ydist};
            } else if (name == "pinky") {
                targetTile = addPosition(pacmanPosition, 3, pacmanDirection);
            } else if (name == "clyde") {
                auto distance = sqrt(pow((position.first - pacmanPosition.first), 2) +
                                     pow((position.second - pacmanPosition.second), 2));
                if (distance <= 8) {
                    targetTile = {0, 32};
                } else {
                    targetTile = pacmanPosition;
                }
            }
            break;
        }
        case GhostMode::dead: {
            targetTile = positionInPen.at(name);
            break;
        }
        case GhostMode::frightened: {
            break;
        }
        case GhostMode::scatter: {
            if (name == "blinky") {
                targetTile = {25, -3};
            } else if (name == "inky") {
                targetTile = {27, 32};
            } else if (name == "pinky") {
                targetTile = {2, -3};
            } else if (name == "clyde") {
                targetTile = {0, 32};
            }
            break;
        }
    }
}

DIRECTION Ghost::selectBestDirection(pair<float, float> fromPosition,
                                     pair<float, float> toPosition) {
    auto baseMapPtr = ResourceManager::GetSprite<Map>("baseMap");
    set<DIRECTION> possible;
    if (currentMode == GhostMode::dead) {
        possible = baseMapPtr->possibleDirections(fromPosition, {MAP_WALL});
    } else {
        possible = baseMapPtr->possibleDirections(fromPosition);
    }
    // Ghost cannot normally reverse direction
    possible.erase(getOppositeDirection(currentDirection));
    // Note: The current pacman map is guaranteed to have at least one possible direction
    DIRECTION bestDirection = *possible.begin();
    std::map<DIRECTION, int> priorityOrder = {
        {DIRECTION::up, 4}, {DIRECTION::left, 3}, {DIRECTION::down, 2}, {DIRECTION::right, 1}};
    float minValue = 1e9;
    for (auto c : possible) {
        std::pair<float, float> newPosition;
        switch (c) {
            case DIRECTION::up: {
                newPosition = {fromPosition.first, fromPosition.second - 1};
                break;
            }
            case DIRECTION::down: {
                newPosition = {fromPosition.first, fromPosition.second + 1};
                break;
            }
            case DIRECTION::left: {
                newPosition = {fromPosition.first - 1, fromPosition.second};
                break;
            }
            case DIRECTION::right: {
                newPosition = {fromPosition.first + 1, fromPosition.second};
                break;
            }
        }
        float distance = sqrt(pow((toPosition.first - newPosition.first), 2) +
                              pow((toPosition.second - newPosition.second), 2));
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
    return bestDirection;
}

GhostMode Ghost::getMode() const {
    return currentMode;
}

void Ghost::setMode(GhostMode newMode) {
    currentMode = newMode;
}

float Ghost::getMultiplier() const {
    return speedMultiplier;
}

void Ghost::setMultiplier(double newSpeed) {
    speedMultiplier = newSpeed;
}

void Ghost::MoveOutofPen() {
    float diffPixels = Game::getSpeed() * Game::getTime() * speedMultiplier;
    auto oldPosition = position;
    if (position.first <= 13.3) {
        position.first += diffPixels;
        currentDirection = DIRECTION::right;
    } else if (position.first >= 13.7) {
        position.first -= diffPixels;
        currentDirection = DIRECTION::left;
    } else {
        position.first = 13.5;
        position.second -= diffPixels;
        currentDirection = DIRECTION::up;
    }
}

void Ghost::reset() {
    position = initialPosition.at(name);
    currentMode = GhostMode::scatter;
    currentDirection = (name == "blinky") ? DIRECTION::right : DIRECTION::up;
    nextDirection = DIRECTION::right;
    outOfPen = false;
    ghostTimer = Timer();
}
