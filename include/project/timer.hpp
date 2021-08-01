#pragma once

#include <chrono>

class Timer {
   public:
    decltype(std::chrono::high_resolution_clock::now()) _startTimePoint;
    decltype(std::chrono::high_resolution_clock::now()) _pauseTimePoint;
    decltype(std::chrono::high_resolution_clock::now()) _stopTimePoint;
    int64_t _totalPausedTime;
    bool _isStarted;
    bool _isPaused;
    bool _isStopped;

   public:
    Timer(bool start = true);
    ~Timer() = default;
    void start();
    void pause();
    void resume();
    int64_t stop();
    int64_t timeElapsed() const;
    bool isStarted() const;
    bool isPaused() const;
    bool isStopped() const;
    static auto getCurrentTimePoint() -> decltype(std::chrono::high_resolution_clock::now());
};