#pragma once

#include <project/common.h>
#include <memory>
#include <nlohhmann/json.hpp>
#include <project/timer.hpp>

class GameState {
    nlohmann::json _jsonData;
    int _level;
    int _lives;
    int _frightenedTimer;
    bool _isPaused;
    bool _isStarted;
    bool _isFrightened;

    double _baseSpeed;

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
    [[nodiscard]] bool isStarted() const;
    void setStarted(bool value = true);
    [[nodiscard]] int getFrightenedTimer() const;
    [[nodiscard]] double getBaseSpeed() const;
    double setBaseSpeed(double newSpeed);
};
