#include <project/resourceManager.h>
#include <fstream>
#include <iostream>
#include <project/gameState.hpp>

using json = nlohmann::json;

GameState::GameState() : _level(1), _isPaused(false), _lives(3) {
    std::ifstream i(ResourceManager::resolvePath("resources/config.json"));
    i >> _jsonData;
}

json GameState::getLevelData() const {
    return _jsonData["level"][_level - 1];
}

bool GameState::isPaused() const {
    return _isPaused;
}

int GameState::getLives() const {
    return _lives;
}

void GameState::setLives(int livesRemaining) {
    _lives = livesRemaining;
}

bool GameState::invertPaused() {
    _isPaused = !_isPaused;
    return _isPaused;
}

int GameState::getLevel() const {
    return _level;
}

void GameState::setLevel(int newLevel) {
    _level = newLevel;
}

bool GameState::isStarted() {
    return _isStarted;
}

void GameState::setStarted(bool value) {
    _isStarted = value;
}

int GameState::getFrightenedTimer() {
    return _frightenedTimer;
}
void GameState::reset() {
    _lives = 3;
    _level = 1;
    _isStarted = false;
    _isPaused = false;
}
