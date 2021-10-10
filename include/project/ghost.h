#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <project/gridBox.hpp>
#include <spdlog/sinks/basic_file_sink.h>
#include <map>
#include <project/timer.hpp>
#include <set>
#include <utility>

enum struct GhostMode
{
    chase,
    scatter,
    frightened,
    dead
};

class Ghost : public Sprite {
    const GridBox box;
    std::string name;
    std::pair<float, float> targetTile;
    static const std::map<std::string, std::pair<float, float>> positionInPen;
    static const std::map<std::string, std::pair<float, float>> initialPosition;
    static const std::map<std::string, int> timeToLeave;
    bool outOfPen;
    DIRECTION currentDirection;
    DIRECTION nextDirection;
    DIRECTION setNextDirection();
    void recalculatePosition();
    void basicMovement();
    void deadModeMovement(bool ghostInPen);
    void MoveOutofPen();
    void selectTargetTile();
    void handleSpecialZone();
    void calculateMultiplier();
    void initialMovement();
    DIRECTION selectBestDirection(std::pair<float, float> from, std::pair<float, float> to);
    std::set<DIRECTION> possibleDirections();
    GhostMode currentMode;
    std::shared_ptr<spdlog::logger> logger;
    double speedMultiplier{};
    Timer ghostTimer;
    void setMultiplier(double newSpeed);

   public:
    Ghost() = delete;
    Ghost(const std::string& name);
    Ghost(const Ghost&) = delete;   // Copy might cause problems
    Ghost(Ghost&&) = default;
    Ghost& operator=(const Ghost&) = delete;
    void draw(std::string shader) override;
    void drawEyes(const std::string& shader) const;
    [[nodiscard]] GhostMode getMode() const;
    void setMode(GhostMode newMode);
    [[nodiscard]] float getMultiplier() const;
    void reset() override;
    ~Ghost() override = default;
};
