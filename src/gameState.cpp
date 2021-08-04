#include <project/resourceManager.h>
#include <fstream>
#include <iostream>
#include <project/gameState.hpp>

using json = nlohmann::json;

GameState::GameState() : level(1), _isPaused(false) {
    std::ifstream i(ResourceManager::resolvePath("resources/config.json"));
    i >> jsonData;
}

json GameState::getLevelData() const {
    return jsonData["level"][level - 1];
}

bool GameState::isPaused() const {
    return _isPaused;
}

bool GameState::invertPaused() {
    _isPaused = !_isPaused;
    return _isPaused;
}
