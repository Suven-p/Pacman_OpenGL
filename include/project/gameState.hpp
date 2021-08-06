#pragma once

#include <project/common.h>
#include <memory>
#include <nlohhmann/json.hpp>

class GameState {
    nlohmann::json _jsonData;
    int _level;
    int _lives;
    int _frightenedTimer;
    bool _isPaused;
    bool _isStarted;
    bool _isFrightened;

   public:
    GameState();
    ~GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    GameState(const GameState&&) = delete;
    GameState& operator=(const GameState&&) = delete;

    void reset();
    [[nodiscard]] nlohmann::json getLevelData() const;
    [[nodiscard]] bool isPaused() const;
    [[nodiscard]] int getLives() const;
    void setLives(int livesRemaining);
    bool invertPaused();
    [[nodiscard]] int getLevel() const;
    void setLevel(int newLevel);
    bool isStarted();
    void setStarted(bool value);
    int getFrightenedTimer();
};