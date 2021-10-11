#include <project/common.h>
#include <project/game.h>
#include <project/gameLogic.h>
#include <project/ghost.h>
#include <project/pacman.h>
#include <project/pellet.h>
#include <project/resourceManager.h>
#include <project/text_renderer.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

GameLogic::GameLogic(GameState& gameState) : gameState(gameState), text(0, 0) {
    blinkyPtr = ResourceManager::GetSprite<Ghost>("blinky");
    pinkyPtr = ResourceManager::GetSprite<Ghost>("pinky");
    inkyPtr = ResourceManager::GetSprite<Ghost>("inky");
    clydePtr = ResourceManager::GetSprite<Ghost>("clyde");
    pacmanPtr = ResourceManager::GetSprite<Pacman>("pacman");
    scatterCount = 1;
    chaseCount = 1;
    scoreMultiplier = 1;
    currentMode = GhostMode::scatter;
    auto windowSize = ResourceManager::getMainWindow()->getWindowSize();
    text = std::move(TextRenderer(windowSize.first, windowSize.second));
    text.Load(ResourceManager::resolvePath("resources/fonts/ARIAL.TTF"), 24);
}

void GameLogic::draw(std::string shaderName) {
    if (Game::getState().isPaused() || !Game::getState().isStarted() || Game::getState().isReady()) {
        modeTimer.pause();
        return;
    }
    else {
        modeTimer.resume();
    }
    levelData = Game::getState().getLevelData();
    checkStatus();
    checkPellet();
    if(Game::getState().getFrightened()) {
        handleFright();
    }
    else {
        pacmanPtr->setMultiplier(levelData["pacmanSpeed"].get<float>());
        modeTimer.start();
        changeGhostMode();
    }
}

GameLogic::~GameLogic() = default;

void GameLogic::checkStatus() {
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
        pelletPtr->addToScore(200 * scoreMultiplier++);
        ghostPtr->setMode(GhostMode::dead);
    } else if (ghostPtr->getMode() == GhostMode::chase ||
               ghostPtr->getMode() == GhostMode::scatter) {
        Game::getState().setLives(Game::getState().getLives() - 1);
        Game::getState().setReady(true);
        handleEnd();
        ResourceManager::resetSprites({"pellet"});
    }
}

void GameLogic::setFright() {
    frightTimer = Timer();
    // Ghost when dead and returning to Ghost Pen won't change to frightened mode when another power pellet is eaten.
    if (blinkyPtr->getMode() != GhostMode::dead) {
        blinkyPtr->setMode(GhostMode::frightened);
    }
    if (pinkyPtr->getMode() != GhostMode::dead) {
        pinkyPtr->setMode(GhostMode::frightened);
    }
    if (inkyPtr->getMode() != GhostMode::dead) {
        inkyPtr->setMode(GhostMode::frightened);
    }
    if (clydePtr->getMode() != GhostMode::dead) {
        clydePtr->setMode(GhostMode::frightened);
    }
    pacmanPtr->setMultiplier(levelData["pacmanFrightSpeed"].get<float>());
    Game::getState().setFrightened(true);
    Game::getState().setFrightenedTimer(levelData["frightTime"].get<int>());
    frightTimer.start();
    modeTimer.pause();
}

void GameLogic::handleFright() {
    if (frightTimer.timeElapsed() >= Game::getState().getFrightenedTimer()) {
        frightTimer = Timer();
        if (blinkyPtr->getMode() != GhostMode::dead) {
            blinkyPtr->setMode(currentMode);
        }
        if (pinkyPtr->getMode() != GhostMode::dead) {
            pinkyPtr->setMode(currentMode);
        }
        if (inkyPtr->getMode() != GhostMode::dead) {
            inkyPtr->setMode(currentMode);
        }
        if (clydePtr->getMode() != GhostMode::dead) {
            clydePtr->setMode(currentMode);
        }
        pacmanPtr->setMultiplier(levelData["pacmanSpeed"].get<float>());
        modeTimer.resume();
        scoreMultiplier = 1;
        Game::getState().setFrightened(false);
    }
}

void GameLogic::changeGhostMode() {
    if (currentMode == GhostMode::chase) {
        if (chaseCount > 3) {
            return;
        }
        if (modeTimer.timeElapsed() > levelData["chase"][chaseCount-1].get<int>()) {
            currentMode = GhostMode::scatter;
            blinkyPtr->setMode(GhostMode::scatter);
            pinkyPtr->setMode(GhostMode::scatter);
            inkyPtr->setMode(GhostMode::scatter);
            clydePtr->setMode(GhostMode::scatter);
            chaseCount++;
            modeTimer = Timer("", true);
        }
    }
    else if (currentMode == GhostMode::scatter) {
        if (modeTimer.timeElapsed() > levelData["scatter"][scatterCount-1].get<int>()) {
            currentMode = GhostMode::chase;
            blinkyPtr->setMode(GhostMode::chase);
            pinkyPtr->setMode(GhostMode::chase);
            inkyPtr->setMode(GhostMode::chase);
            clydePtr->setMode(GhostMode::chase);
            scatterCount++;
            modeTimer = Timer("", true);
        }
    }
}

void GameLogic::checkPellet() {
    auto pelletPtr = ResourceManager::GetSprite<Pellet>("pellet");
    if (pelletPtr->getPelletsEaten() >= 244) {
        Game::reset();
        int level = Game::getState().getLevel();
        int lives = Game::getState().getLives();
        int score = pelletPtr->getScore();
        Game::getState().reset(true);
        Game::getState().setLevel(level + 1);
        Game::getState().setLives(lives);
        ResourceManager::resetSprites();
        pelletPtr->addToScore(score);
    }
}

void GameLogic::reset() {
    frightTimer = Timer();
    scatterCount = 1;
    chaseCount = 1;
    scoreMultiplier = 1;
    modeTimer = Timer();
    currentMode = GhostMode::scatter;
}

void GameLogic::handleEnd() {
    if (Game::getState().getLives()  == 0) {
        Game::reset();
        Game::getState().setGameOver(true);
        Game::getState().setReady(false);
        Game::getState().reset();
        ResourceManager::resetSprites();
    }
}

GhostMode GameLogic::getMode() {
    return currentMode;
}
