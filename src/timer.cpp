#include <project/timer.hpp>

using namespace std::chrono;

Timer::Timer(bool autostart) :
    _totalPausedTime(0), _isStarted(false), _isPaused(false), _isStopped(false) {
    if (autostart) {
        start();
    }
}

void Timer::start() {
    if (_isStarted) {
        return;
    }
    _startTimePoint = Timer::getCurrentTimePoint();
    _isStarted = true;
}

void Timer::pause() {
    if (!_isStarted || _isPaused)
        return;
    _pauseTimePoint = Timer::getCurrentTimePoint();
    _isPaused = true;
}

void Timer::resume() {
    if (!_isStarted || !_isPaused)
        return;
    auto currentTime = Timer::getCurrentTimePoint();
    auto pausedTime = duration_cast<milliseconds>(currentTime - _pauseTimePoint).count();
    _totalPausedTime += pausedTime;
    _isPaused = false;
}

int64_t Timer::stop() {
    if (!_isStarted) {
        return 0;
    }
    _isStopped = true;
    return timeElapsed();
}

int64_t Timer::timeElapsed() const {
    auto currentTime = Timer::getCurrentTimePoint();
    auto pausedTime = _totalPausedTime;
    if (_isPaused) {
        auto temp = duration_cast<milliseconds>(currentTime - _pauseTimePoint);
        pausedTime += temp.count();
    }
    auto ans = duration_cast<milliseconds>(currentTime - _startTimePoint).count();
    ans -= pausedTime;
    return ans;
}

bool Timer::isStarted() const {
    return _isStarted;
}

bool Timer::isPaused() const {
    return _isPaused;
}

bool Timer::isStopped() const {
    return _isStopped;
}

auto Timer::getCurrentTimePoint() -> decltype(std::chrono::high_resolution_clock::now()) {
    using namespace std::chrono;
    auto ms = high_resolution_clock::now();
    return ms;
}
