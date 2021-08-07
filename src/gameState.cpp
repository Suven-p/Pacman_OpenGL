#include <project/resourceManager.h>
#include <fstream>
#include <iostream>
#include <project/gameState.hpp>

using json = nlohmann::json;

GameState::GameState() :
    _level(1), _isStarted(false), _isPaused(false), _isReady(false), _lives(3), _baseSpeed(0.01) {
    std::ifstream i(ResourceManager::resolvePath("resources/config.json"));
    i >> _jsonData;
}

json GameState::getLevelData() const {
    auto size = (_jsonData["level"]).size();
    if (_level > size) {
        return _jsonData["level"][size - 1];
    }
    else {
        return _jsonData["level"][_level - 1];
    }
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

bool GameState::isStarted() const {
    return _isStarted;
}

void GameState::setStarted(bool value) {
    _isStarted = value;
}

bool GameState::isReady() const {
    return _isReady;
}

void GameState::setReady(bool value) {
    _isReady = value;
}

bool GameState::isGameOver() const {
    return _isOver;
}

void GameState::setGameOver(bool value) {
    _isOver = value;
}

int GameState::getFrightenedTimer() const {
    return _frightenedTimer;
}

void GameState::reset(bool restart) {
    if (restart) {
        _lives = 3;
        _level = 1;
        _isStarted = false;
    }
    _isPaused = false;
}

double GameState::getBaseSpeed() const {
    return _baseSpeed;
}

double GameState::setBaseSpeed(double newSpeed) {
    _baseSpeed = newSpeed;
    return _baseSpeed;
}

void GameState::setFrightened(bool value) {
    _isFrightened = value;
}

void GameState::setFrightenedTimer(int time) {
    _frightenedTimer = time;
}

bool GameState::getFrightened() {
    return _isFrightened;
}
