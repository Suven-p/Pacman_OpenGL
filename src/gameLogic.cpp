#include <project/common.h>
#include <project/game.h>
#include <project/gameLogic.h>
#include <project/ghost.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <project/text_renderer.h>
#include <string>
#include <vector>
#include "project/windowManager.h"

GameLogic::GameLogic(GameState& gameState) : gameState(gameState), text(0, 0) {
    auto windowSize = WindowManager::getInstance()->getWindowSize();
    text = std::move(TextRenderer(windowSize.first, windowSize.second));
    text.Load(ResourceManager::resolvePath("resources/fonts/ARIAL.TTF"), 24);
}

void GameLogic::draw(std::string shaderName) {
    checkStatus();
}

GameLogic::~GameLogic() = default;

void GameLogic::checkStatus() {
    auto pacmanPtr = ResourceManager::GetSprite<Pacman>("pacman");
    auto blinkyPtr = ResourceManager::GetSprite<Ghost>("blinky");
    auto pinkyPtr = ResourceManager::GetSprite<Ghost>("pinky");
    auto inkyPtr = ResourceManager::GetSprite<Ghost>("inky");
    auto clydePtr = ResourceManager::GetSprite<Ghost>("clyde");
    constexpr auto toInt = [](std::pair<float, float> pos) {
        std::pair<int, int> ans = pos;
        if ((pos.first - int(pos.first)) >= 0.5) {
            ans.first++;
        }
        if ((pos.second - int(pos.second)) >= 0.5) {
            ans.second++;
        }
        return ans;
    };
    if (toInt(pacmanPtr->getPosition()) == toInt(blinkyPtr->getPosition())) {
        handleCollision(blinkyPtr);
    }
    if (toInt(pacmanPtr->getPosition()) == toInt(pinkyPtr->getPosition())) {
        handleCollision(pinkyPtr);
    }
    if (toInt(pacmanPtr->getPosition()) == toInt(inkyPtr->getPosition())) {
        handleCollision(inkyPtr);
    }
    if (toInt(pacmanPtr->getPosition()) == toInt(clydePtr->getPosition())) {
        handleCollision(clydePtr);
    }
}

void GameLogic::handleCollision(std::shared_ptr<Ghost> ghostPtr) {
    if (ghostPtr->getMode() == GhostMode::frightened) {
        auto pelletPtr = ResourceManager::GetSprite<Pellet>("pellet");
        pelletPtr->addToScore(200);
        ghostPtr->setMode(GhostMode::dead);
    } else if (ghostPtr->getMode() == GhostMode::chase ||
               ghostPtr->getMode() == GhostMode::scatter) {
        Game::getState().setLives(Game::getState().getLives() - 1);
        ResourceManager::resetSprites({"pellet"});
    }
}

void GameLogic::reset() {}
