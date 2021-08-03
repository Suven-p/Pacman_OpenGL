#pragma once

#include <project/common.h>
#include <memory>
#include <nlohhmann/json.hpp>

class GameState {
    GameState();
    static std::shared_ptr<GameState> instance;
    nlohmann::json jsonData;
    int level;

public:
    static std::shared_ptr<GameState> getInstance();
    nlohmann::json getLevelData();
};
