#pragma once

#include <project/common.h>
#include <memory>
#include <nlohhmann/json.hpp>

class GameState {
    GameState();
    nlohmann::json jsonData;
    int level;
    bool isPaused;

   public:
    ~GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    GameState(const GameState&&) = delete;
    GameState& operator=(const GameState&&) = delete;

    static GameState& getInstance();
    nlohmann::json getLevelData();
};
