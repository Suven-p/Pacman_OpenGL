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
    bool _isOver;
    bool _isFrightened;
    bool _isReady;
    //BaseSpeed is tiles per millisecond
    double _baseSpeed;

   public:
    GameState();
    ~GameState() = default;
    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;
    GameState(const GameState&&) = delete;
    GameState& operator=(const GameState&&) = delete;

    void reset(bool restart = false);
    [[nodiscard]] nlohmann::json getLevelData() const;
    [[nodiscard]] bool isPaused() const;
    [[nodiscard]] int getLives() const;
    void setLives(int livesRemaining);
    bool invertPaused();
    [[nodiscard]] int getLevel() const;
    void setLevel(int newLevel);
    [[nodiscard]] bool isStarted() const;
    void setStarted(bool value = true);
    [[nodiscard]] bool isReady() const;
    void setReady(bool value);
    void setFrightened(bool value);
    void setFrightenedTimer(int time);
    bool getFrightened();
    [[nodiscard]] int getFrightenedTimer() const;
    [[nodiscard]] double getBaseSpeed() const;
    double setBaseSpeed(double newSpeed);
    [[nodiscard]] bool isGameOver() const;
    void setGameOver(bool value);
};
