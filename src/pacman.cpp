#include <project/pacman.h>
#include <project/game.h>
#include <project/resourceManager.h>
#include <project/common.h>
#include <iostream>
#include <set>
#include <iterator>
Pacman::Pacman()
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

}

void Pacman::draw(std::string shader)
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
    temp_model = glm::translate(temp_model, glm::vec3(0.5f, 0.5f, 0.0f));
    temp_model = glm::rotate(temp_model,glm::radians(int(currentDirection)*90.0f),glm::vec3(0.0,0.0,1.0));
    temp_model = glm::translate(temp_model, glm::vec3(-0.5f,-0.5f, 0.0f));


    ResourceManager::GetShader(shader).SetMatrix4("model", temp_model, true);

    auto texture = ResourceManager::GetTexture("pacman");
    texture.Bind(0);
    ResourceManager::GetShader(shader).SetInteger("texture1", 0, true);
    ResourceManager::GetShader(shader).SetFloat("textureColorMix", 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);

    glBindVertexArray(0);
}
DIRECTION Pacman::getDirection()
{
    return currentDirection;
}
void Pacman::setDirection(DIRECTION newDirection)
{
    currentDirection = newDirection;
}
std::shared_ptr<Pacman> getPacmanPtr()
{
    return std::dynamic_pointer_cast<Pacman>(ResourceManager::GetSprite("pacman"));
}

void Pacman::getNewPosition()
{

    float diffPixels = Game::getInstance()->getSpeed() * Game::getInstance()->getTime() * 0.8;
    // float diffPixels = Game::getInstance()->getSpeed() * 16 * 0.75;
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
    
    auto baseMapPtr = std::dynamic_pointer_cast<Map>(ResourceManager::GetSprite("baseMap"));
    auto possible = baseMapPtr->possibleDirections(std::pair<int,int>(oldPosition));
    bool collision = true;

    for(auto itr = possible.begin();itr!=possible.end(); ++itr)
    {
        if(*itr == currentDirection)
        {
            collision = false;
        }
    }
    if(collision)
    {
        position = std::pair<int,int>(oldPosition);
    }
    
}