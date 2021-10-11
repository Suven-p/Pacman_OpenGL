#pragma once
#include <project/common.h>
#include <project/ghost.h>
#include <project/pacman.h>
#include <project/mapData.h>
#include <project/sprite.h>
#include <project/timer.hpp>
#include <project/text_renderer.h>
#include <memory>
#include <project/gameState.hpp>
#include <vector>
#include <nlohhmann/json.hpp>

class GameLogic : public Sprite {
    GLuint VAO, VBO, EBO;
    GameState& gameState;
    TextRenderer text;
    Timer frightTimer, modeTimer;
    std::shared_ptr<Ghost> blinkyPtr, pinkyPtr, inkyPtr, clydePtr;
    std::shared_ptr<Pacman> pacmanPtr;
    nlohmann::json levelData;
    int scatterCount, chaseCount;
    int scoreMultiplier;
    GhostMode currentMode;
    void checkStatus();
    void checkPellet();
    void handleCollision(std::shared_ptr<Ghost> ghostPtr);
    void handleFright();
    void handleEnd();
    void changeGhostMode();

   public:
    GameLogic(GameState& gameState);
    ~GameLogic() override;
    void draw(std::string shaderName) override;
    void reset() override;
    void setFright();
    GhostMode getMode();
};
