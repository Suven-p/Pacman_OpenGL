#include <project/common.h>
#include <project/game.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <cmath>
#include <iostream>
#include <iterator>
#include <set>

Pellet::Pellet() {
    mapData = mapDataColRow;
    // X in mapData denotes uneaten Power Pellet
    mapData[1][3] = 'X', mapData[26][3] = 'X';
    mapData[1][23] = 'X', mapData[26][23] = 'X';
    score = 0;
    pelletsEaten = 0;
    timeTillCherryDisappears = 0;
    timeTillFrightenedModeStops = 0;
    timeTillScatterMode = glfwGetTime() + 7;
    timeTillChaseMode = timeTillScatterMode + 20;
    chaseIteration = 0;
    mode = 1;

    glGenVertexArrays(1, &blockVAO);
    glGenBuffers(1, &blockVBO);
    glGenBuffers(1, &blockEBO);
    float obstacleVertices[] = {
        // positions            // colors               // texture coords
        1.5F, -0.5F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1.0F,  // top right
        1.5F, 1.5F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F,  // bottom right
        -0.5F, 1.5F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F,  // bottom left
        -0.5F, -0.5F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F   // top left
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

void Pellet::draw(std::string shaderName) {
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(blockVAO);

    auto pacmanPosition = getPacmanPtr()->getPosition();
    changePelletStatus(pacmanPosition);
    setMode();

    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, 1.0F, -1.0F);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 0.0F);
    auto texture = ResourceManager::GetTexture("pellet");
    texture.Bind(0);
    auto texture2 = ResourceManager::GetTexture("power_pellet");
    texture2.Bind(1);
    auto texture3 = ResourceManager::GetTexture("cherry");
    texture3.Bind(2);

    shader.SetInteger("texture1", 0, true);

    glm::mat4 model;

    for (int i = 0; i < mapData.size(); i++) {
        for (int j = 0; j < mapData[i].size(); j++) {
            model = glm::mat4(1.0F);
            model = glm::translate(model, glm::vec3(0.0F, 3.0F, 0.0F));
            model = glm::translate(model, glm::vec3(float(i), float(j), 0.0F));
            shader.SetMatrix4("model", model);
            switch (mapData[i][j]) {
                case 'G':
                case 'F':
                    break;
                case 'X':
                    shader.SetInteger("texture1", 1, true);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                    break;
                case 'C':
                    if (toDrawCherry()) {
                        shader.SetInteger("texture1", 2, true);
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
                    }
                    break;
                case 'n':
                case 'P':
                case 'W':
                    break;
                case 'o':
                default:
                    shader.SetInteger("texture1", 0, true);
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            }
        }
    }
    glBindVertexArray(0);
}

void Pellet::drawCherry(const std::string& shaderName) {
    return;
}

std::shared_ptr<Pellet> getPelletPtr() {
    return std::dynamic_pointer_cast<Pellet>(ResourceManager::GetSprite("pellet"));
}

void Pellet::changePelletStatus(std::pair<float, float> pacmanPosition) {
    int xCoordinate = int(pacmanPosition.first), yCoordinate = int(pacmanPosition.second);
    if (mapData[xCoordinate][yCoordinate] == 'o') {
        mapData[xCoordinate][yCoordinate] = 'F';
        score += 10;
        pelletsEaten++;
    } 
    else if (mapData[xCoordinate][yCoordinate] == 'X') {
        mapData[xCoordinate][yCoordinate] = 'F';
        score += 50;
        pelletsEaten++;
        // TODO : replace glfwGetTime()
        timeTillFrightenedModeStops = glfwGetTime() + 6;

        timeTillScatterMode += timeTillFrightenedModeStops;
        timeTillChaseMode += timeTillFrightenedModeStops;
    } 
    else if (mapData[xCoordinate][yCoordinate] == 'C') {
        mapData[xCoordinate][yCoordinate] = 'F';
        score += 100;
    }
    if (pelletsEaten == 70 or pelletsEaten == 170) {
        // TODO : replace glfwGetTime
        timeTillCherryDisappears = glfwGetTime() + 10;
        mapData[14][17] = 'C';
    }
}

bool Pellet::toDrawCherry() {
    // TODO : replace glfwGetTime
    if (glfwGetTime() > timeTillCherryDisappears) {
        mapData[14][17] = 'n';
        return false;
    } 
    else
        return true;
}

int Pellet::getScore() {
    return score;
}

int Pellet::getPelletsEaten() {
    return pelletsEaten;
}

void Pellet::setMode()
{
    // Check for Frightened mode
    if(glfwGetTime() < timeTillFrightenedModeStops) {
        mode = 3;
        return;
    }

    // If there have been 4 or more chases then there is no more future scatter mode 
    if(chaseIteration >= 4) {
        mode = 2;
        return;
    }
    
    if(timeTillScatterMode < timeTillChaseMode) {

        // If scatter mode has just passed
        if(glfwGetTime() > timeTillScatterMode) {
            timeTillScatterMode = timeTillChaseMode + 7;
            mode = 2;
            chaseIteration++;
        }
        else {
            mode = 1;
        }

    }
    
    if(timeTillChaseMode < timeTillScatterMode) {

        // If chase mode has just just passed
        if(glfwGetTime() > timeTillChaseMode and chaseIteration < 4) {
            timeTillChaseMode = timeTillScatterMode + 20;
            mode = 1;
        }
        else {
            mode = 2;
        }
        
    }
}

bool Pellet::allPelletsEaten(){
    if(pelletsEaten == 244)
        return true;
    else
        return false;
}

Pellet::~Pellet() = default;