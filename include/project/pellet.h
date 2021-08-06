#pragma once
#include <project/common.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <memory>
class Pellet : public Sprite {
    GLuint blockVAO, blockVBO, blockEBO;
    int score, pelletsEaten;
    float timeTillCherryDisappears;
    float timeTillFrightenedModeStops;
    float timeTillScatterMode;
    float timeTillChaseMode;
    int chaseIteration;

    // Mode 1 - Scatter; Mode 2 - Chase; Mode 3 - Frightened
    int mode;
   public:
    std::vector<std::vector<char>> mapData;

    Pellet();
    void draw(std::string shaderName);
    void drawCherry(const std::string& shaderName);
    void changePelletStatus(std::pair<float, float> pacmanPosition);
    bool toDrawCherry();
    int getScore();
    int getPelletsEaten();
    void setMode();
    int getMode();

    // Returns true if all(244) pellets are eaten  
    bool allPelletsEaten();

    // Adds incoming value to score
    void addToScore(int value);

    // Initializes timers for mode selection.
    // To be called when ghost eats pacman
    void initializeModeTimer();

    ~Pellet();
};
std::shared_ptr<Pellet> getPelletPtr();
