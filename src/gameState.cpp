#include <project/resourceManager.h>
#include <fstream>
#include <iostream>
#include <project/gameState.hpp>

using json = nlohmann::json;

GameState& GameState::getInstance() {
    static GameState instance;
    return instance;
}

GameState::GameState() : level(1), isPaused(false) {
    std::ifstream i(ResourceManager::resolvePath("resources/config.json"));
    i >> jsonData;
}

json GameState::getLevelData() {
    return jsonData["level"][level - 1];
}
