#include <project/gameState.hpp>
#include <project/resourceManager.h>
#include <iostream>
#include <fstream>

using json = nlohmann::json;

std::shared_ptr<GameState> GameState::instance;

std::shared_ptr<GameState> GameState::getInstance() {
    if (!instance) {
        instance = std::shared_ptr<GameState>(new GameState());
    }
    return instance;
}

GameState::GameState(): level(1) {
    std::ifstream i(ResourceManager::resolvePath("resources/config.json"));
    i >> jsonData;
}

json GameState::getLevelData() {
    return jsonData["level"][level-1];
}
