#include <project/common.h>
#include <project/ghost.h>
#include <project/game.h>
#include <project/resourceManager.h>

using namespace std;

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
}

Ghost::~Ghost()
{
}

void Ghost::draw(std::string shader)
{
    getNewPosition();
    //cout << "X = " << position.first << " Y = " << position.second << endl << Game::getInstance()->getTime() << endl;
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

DIRECTION Ghost::setDirection() 
{
    //TODO: is junction?, get target tile, get possible directions, set direction towards target tile through possible direction.
    currentDirection = DIRECTION::down;
    return currentDirection;
}

void Ghost::getNewPosition() 
{
    // TODO: set ghost speed as variable  
    setDirection();
    float diffPixels = Game::getInstance()->getSpeed() * Game::getInstance()->getTime() * 0.75;
    switch (currentDirection) 
    {
        case DIRECTION::left:
            position.first -= diffPixels;
            break;
        case DIRECTION::right:
            position.first += diffPixels;
            break;
        case DIRECTION::up:
            position.second -= diffPixels;
            break;
        case DIRECTION::down:
            position.second += diffPixels;
            break;
    }
}
