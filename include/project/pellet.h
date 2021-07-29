#pragma once
#include<project/sprite.h>
#include<project/mapData.h>
#include<project/common.h>
#include<memory>
class Pellet: public Sprite
{
    GLuint blockVAO, blockVBO, blockEBO;
    int score;
public:

    std::vector<std::vector<char>> mapData;

    Pellet();
    void draw(std::string shaderName);
    void drawCherry(std::string shaderName);
    void changePelletStatus(std::pair<float, float> pacmanPosition);
    int getScore();

    ~Pellet();
};
std::shared_ptr<Pellet> getPelletPtr();

