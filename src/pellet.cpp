#include <project/pellet.h>
#include <project/game.h>
#include <project/resourceManager.h>
#include <project/common.h>
#include <project/pacman.h>
#include <iostream>
#include <set>
#include <iterator>
#include <math.h>

Pellet::Pellet()
{
    mapData = mapDataColRow;
    // X in mapData denotes uneaten Power Pellet
    mapData[1][3] = 'X', mapData[26][3]= 'X';
    mapData[1][23] = 'X', mapData[26][23] = 'X';
    score = 0;

    glGenVertexArrays(1, &blockVAO);
    glGenBuffers(1, &blockVBO);
    glGenBuffers(1, &blockEBO);
    float obstacleVertices[] = {
        // positions            // colors               // texture coords
        1.0f, 0.0f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f, 1.0f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom right
        0.0f, 1.0f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        0.0f, 0.0f, 0.05f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    glBindVertexArray(blockVAO);
    glBindBuffer(GL_ARRAY_BUFFER, blockVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, blockEBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(obstacleVertices), obstacleVertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)(4 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void *)(8 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void Pellet::draw(std::string shaderName)
{
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(blockVAO);

    auto pacmanPosition = getPacmanPtr()->getPosition();
    changePelletStatus(pacmanPosition);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f, 28.0f, 36.0f, 0.0f, 1.0f, -1.0f);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 0.0f);
    auto texture = ResourceManager::GetTexture("pellet");
    texture.Bind(0);
    auto texture2 = ResourceManager::GetTexture("power_pellet");
    texture2.Bind(1);

    shader.SetInteger("texture1", 0, true);

    glm::mat4 model;
    
    for (int i = 0; i < mapData.size(); i++)
    {
        for (int j = 0; j < mapData[i].size(); j++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f));
            model = glm::translate(model, glm::vec3(float(i), float(j), 0.0f));
            shader.SetMatrix4("model", model);
            switch (mapData[i][j])
            {
            case 'G':
            case 'F':
                break;
            case 'X':
                shader.SetInteger("texture1", 1, true);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                break;
            case 'n':
            case 'P':
            case 'W':
                break;
            case 'o':
            default:
                shader.SetInteger("texture1", 0, true);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
    }
    glBindVertexArray(0);
}

void Pellet::drawCherry(std::string shaderName) 
{
    return;
}

std::shared_ptr<Pellet> getPelletPtr()
{
    return std::dynamic_pointer_cast<Pellet>(ResourceManager::GetSprite("pellet"));
}

void Pellet::changePelletStatus(std::pair<float, float> pacmanPosition) 
{
    int xCoordinate = int(pacmanPosition.first), yCoordinate = int(pacmanPosition.second);
    if(mapData[xCoordinate][yCoordinate] == 'o')
    {
        mapData[xCoordinate][yCoordinate] = 'F'; 
        score++;
    }
    else if(mapData[xCoordinate][yCoordinate] == 'X')
    {
        mapData[xCoordinate][yCoordinate] = 'F'; 
        score++;
    }

}

int Pellet::getScore() 
{
    return score;
}


Pellet::~Pellet () {
    
}