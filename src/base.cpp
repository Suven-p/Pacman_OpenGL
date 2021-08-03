#include <project/common.h>
#include <project/game.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/base.h>
#include <project/resourceManager.h>
#include <vector>

Base::Base() {
    lives = 3;
    position = {
        24,     // Position of cherry in bottom layer
        2,      // Position of first pacman displayed if remaining life >= 2
        4       // Position of second pacman displayed if remaining life == 3
    };

    glGenVertexArrays(1, &blockVAO);
    glGenBuffers(1, &blockVBO);
    glGenBuffers(1, &blockEBO);
    float obstacleVertices[] = {
        // positions            // colors               // texture coords
        1.8F, 0.2F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 1.0F, 1.0F,  // top right
        1.8F, 1.8F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 1.0F, 0.0F,  // bottom right
        0.2F, 1.8F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F,  // bottom left
        0.2F, 0.2F, 0.05F, 1.0F, 1.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F   // top left
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

void Base::draw(std::string shaderName) {
    auto shader = ResourceManager::GetShader(shaderName);
    shader.Use();
    glBindVertexArray(blockVAO);

    glm::mat4 view = glm::mat4(1.0F);
    glm::mat4 projection = glm::ortho(0.0F, 28.0F, 36.0F, 0.0F, 1.0F, -1.0F);
    shader.SetMatrix4("view", view);
    shader.SetMatrix4("projection", projection);
    shader.SetFloat("textureColorMix", 0.0F);
    auto texture = ResourceManager::GetTexture("pacman");
    texture.Bind(0);
    auto texture2 = ResourceManager::GetTexture("cherry");
    texture2.Bind(1);

    shader.SetInteger("texture1", 0, true);

    glm::mat4 model;

    for (int i = 0; i < lives; i++) {
        model = glm::mat4(1.0F);
        model = glm::translate(model, glm::vec3(0.0F, 3.0F, 0.0F));
        model = glm::translate(model, glm::vec3(float(position[i]), float(31), 0.0F));
        shader.SetMatrix4("model", model);

        if(i == 0)
            shader.SetInteger("texture1", 1, true);
        else
            shader.SetInteger("texture1", 0, true);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
    glBindVertexArray(0);
}

void Base::decreaselives() {
    lives = lives - 1;
}

std::shared_ptr<Base> getBasePtr() {
    return std::dynamic_pointer_cast<Base>(ResourceManager::GetSprite("base"));
}

Base::~Base() = default;