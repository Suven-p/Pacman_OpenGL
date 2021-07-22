#include <project/common.h>
#include <project/ghost.h>
#include <project/game.h>
#include <project/resourceManager.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <cmath>
#include <utility>
#include <random>

using namespace std;

std::random_device rd;  //Will be used to obtain a seed for the random number engine
std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()

Ghost::Ghost(std::string name) : name(name)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbo);
    glGenBuffers(1, &ebo);

    float vertices[] = {
        -0.5f, -0.5f, 0.1f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 1.5f, 0.1f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        1.5f, 1.5f, 0.1f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        1.5f, -0.5f, 0.1f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f};

    float texCoord[] = {
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f};

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3};

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(4 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(0));
    glEnableVertexAttribArray(2);

    currentDirection = DIRECTION::right;
    nextDirection = DIRECTION::right;
    currentMode = GhostMode::scatter;

    std::string loggerName = std::string("ghostLogger::") + name;
    logger = spdlog::basic_logger_mt(loggerName, "logs/ghostLog.txt");
    logger->set_level(spdlog::level::trace);
}

Ghost::~Ghost()
{
}

void Ghost::draw(std::string shader)
{
    getNewPosition();
    ResourceManager::GetShader(shader).Use();
    glBindVertexArray(vao);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
    glm::mat4 view(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 36.0f, 0.0f, 1.0f, -1.0f);
    ResourceManager::GetShader(shader).SetMatrix4("view", view, true);
    ResourceManager::GetShader(shader).SetMatrix4("projection", projection, true);

    glm::mat4 temp_model = glm::mat4(1.0f);
    temp_model = glm::translate(model, glm::vec3(position.first, position.second, 0.0f));

    ResourceManager::GetShader(shader).SetMatrix4("model", temp_model, true);

    auto texture = ResourceManager::GetTexture(name);
    texture.Bind(0);
    ResourceManager::GetShader(shader).SetInteger("texture1", 0, true);
    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

    glBindVertexArray(0);
}

DIRECTION Ghost::setNextDirection()
{
    // TODO: get target tile for chase mode
    std::pair<int, int> nextTile;
    DIRECTION oppositeDirection;
    switch (currentDirection)
    {
    case DIRECTION::up:
    {
        nextTile = {position.first, std::ceil(position.second) - 1};
        oppositeDirection = DIRECTION::down;
        break;
    }
    case DIRECTION::down:
    {
        nextTile = {position.first, position.second + 1};
        oppositeDirection = DIRECTION::up;
        break;
    }
    case DIRECTION::left:
    {
        nextTile = {std::ceil(position.first) - 1, position.second};
        oppositeDirection = DIRECTION::right;
        break;
    }
    case DIRECTION::right:
    {
        nextTile = {position.first + 1, position.second};
        oppositeDirection = DIRECTION::left;
        break;
    }
    }
    auto baseMapPtr = std::dynamic_pointer_cast<Map>(ResourceManager::GetSprite("baseMap"));
    auto possible = baseMapPtr->possibleDirections(nextTile);
    possible.erase(oppositeDirection);
    logger->trace("Next tile is: {}, {}", nextTile.first, nextTile.second);

    if (possible.size())
    {
        if (currentMode == GhostMode::frightened)
        {
            std::uniform_int_distribution<> distrib(0, possible.size() - 1);
            auto it = possible.begin();
            auto index = distrib(gen);
            std::advance(it, index);
            nextDirection = *it;
        }
        else if (currentMode == GhostMode::scatter)
        {
            if (name == "blinky")
            {
                targetTile = {25, -3};
            }
            else if (name == "inky")
            {
                targetTile = {27, 32};
            }
            else if (name == "pinky")
            {
                targetTile = {2, -3};
            }
            else if (name == "clyde")
            {
                targetTile = {0, 32};
            }
            logger->trace("Target Tile is: {} {}", targetTile.first, targetTile.second);
            float minValue = 1e9;
            DIRECTION bestDirection;
            std::map<DIRECTION, int> priorityOrder = {{DIRECTION::up, 4}, {DIRECTION::left, 3}, {DIRECTION::down, 2}, {DIRECTION::right, 1}};
            for (auto it = possible.begin(); it != possible.end(); it++)
            {
                auto c = *it;
                std::pair<float, float> newPosition;
                switch (c)
                {
                case DIRECTION::up:
                {
                    newPosition = {nextTile.first, nextTile.second - 1};
                    break;
                }
                case DIRECTION::down:
                {
                    newPosition = {nextTile.first, nextTile.second + 1};
                    break;
                }
                case DIRECTION::left:
                {
                    newPosition = {nextTile.first - 1, nextTile.second};
                    break;
                }
                case DIRECTION::right:
                {
                    newPosition = {nextTile.first + 1, nextTile.second};
                    break;
                }
                }
                float distance = sqrt(pow((targetTile.first - newPosition.first), 2) + pow((targetTile.second - newPosition.second), 2));
                logger->trace("New position is {} {}", newPosition.first, newPosition.second);
                logger->trace("Distance in direction {} is {}", toString(c), distance);
                if (distance < minValue)
                {
                    logger->trace("Updating direction to {} based on value.", toString(c));
                    minValue = distance;
                    bestDirection = c;
                }
                else if (distance == minValue)
                {
                    logger->trace("Updating direction to {} based on priority.", toString(c));
                    bestDirection = priorityOrder[c] > priorityOrder[bestDirection] ? c : bestDirection;
                }
            }
            nextDirection = bestDirection;
            logger->trace("Best direction is set to be: {}", toString(bestDirection));
        }
        else if (currentMode == GhostMode::chase)
        {
            if (name == "blinky")
            {
            }
            else if (name == "inky")
            {
            }
            else if (name == "pinky")
            {
            }
            else if (name == "clyde")
            {
            }
        }
    }
    return nextDirection;
}

void Ghost::getNewPosition()
{
    float diffPixels = Game::getInstance()->getSpeed() * Game::getInstance()->getTime() * 0.75;
    auto oldPosition = position;
    bool reachedNewTile = false;
    switch (currentDirection)
    {
    case DIRECTION::left:
    {
        position.first -= diffPixels;
        if (oldPosition.first > std::ceil(position.first) and position.first <= std::ceil(position.first))
        {
            reachedNewTile = true;
            if (nextDirection == DIRECTION::up || nextDirection == DIRECTION::down)
            {
                position.first = std::ceil(position.first);
            }
        }
        break;
    }
    case DIRECTION::up:
    {
        position.second -= diffPixels;
        if (oldPosition.second > std::ceil(position.second) and position.second <= std::ceil(position.second))
        {
            reachedNewTile = true;
            if (nextDirection == DIRECTION::left || nextDirection == DIRECTION::right)
            {
                position.second = std::ceil(position.second);
            }
        }
        break;
    }
    case DIRECTION::right:
    {
        position.first += diffPixels;
        if ((int(position.first) - int(oldPosition.first)) == 1)
        {
            reachedNewTile = true;
            if (nextDirection == DIRECTION::up || nextDirection == DIRECTION::down)
            {
                position.first = int(position.first);
            }
        }
        break;
    }
    case DIRECTION::down:
    {
        position.second += diffPixels;
        if ((int(position.second) - int(oldPosition.second)) == 1)
        {
            reachedNewTile = true;
            if (nextDirection == DIRECTION::left || nextDirection == DIRECTION::right)
            {
                position.second = int(position.second);
            }
        }
        break;
    }
    }
    if (reachedNewTile)
    {
        logger->trace("Current position is: {} {}", position.first, position.second);
        logger->trace("Current direction is: {}", toString(currentDirection));
        logger->trace("Switching direction to {}", toString(nextDirection));
        currentDirection = nextDirection;
        setNextDirection();
    };
}

GhostMode Ghost::getMode()
{
    return currentMode;
}

void Ghost::setMode(GhostMode newMode)
{
    currentMode = newMode;
}
