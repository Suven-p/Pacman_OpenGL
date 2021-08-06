#pragma once

#include <chrono>
#include <map>
#include <memory>
#include <string>

class Timer {
   private:
    static std::map<std::string, Timer*> _allTimers;
    decltype(std::chrono::high_resolution_clock::now()) _startTimePoint;
    decltype(std::chrono::high_resolution_clock::now()) _pauseTimePoint;
    decltype(std::chrono::high_resolution_clock::now()) _stopTimePoint;
    int64_t _totalPausedTime;
    bool _isStarted;
    bool _isPaused;
    bool _isStopped;

   public:
    Timer(std::string name = "", bool start = false);
    ~Timer() = default;
    static Timer* getTimer(std::string name);
    void start();
    void pause();
    void resume();
    int64_t stop();
    [[nodiscard]] int64_t timeElapsed() const;
    [[nodiscard]] bool isStarted() const;
    [[nodiscard]] bool isPaused() const;
    [[nodiscard]] bool isStopped() const;
    static auto getCurrentTimePoint() -> decltype(std::chrono::high_resolution_clock::now());
};
