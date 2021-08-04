#pragma once

#include <project/common.h>
#include <memory>
#include <nlohhmann/json.hpp>

class GameState {
    nlohmann::json jsonData;
    int level;
    bool _isPaused;

   public:
    GameState();
    // ~GameState() = default;
    // GameState(const GameState&) = delete;
    // GameState& operator=(const GameState&) = delete;
    // GameState(const GameState&&) = delete;
    // GameState& operator=(const GameState&&) = delete;

    [[nodiscard]] nlohmann::json getLevelData() const;
    [[nodiscard]] bool isPaused() const;
    bool invertPaused();
};
