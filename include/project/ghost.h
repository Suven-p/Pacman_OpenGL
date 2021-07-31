#pragma once

#include <project/common.h>
#include <project/sprite.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <set>

enum struct GhostMode
{
    chase,
    scatter,
    frightened,
    dead
};

class Ghost : public Sprite {
    GLuint vao, vbo[2], ebo;
    std::string name;
    std::pair<float, float> targetTile;
    DIRECTION currentDirection;
    DIRECTION nextDirection;
    DIRECTION setNextDirection();
    void getNewPosition();
    void MoveOutofBox();
    std::set<DIRECTION> possibleDirections();
    GhostMode currentMode;
    std::shared_ptr<spdlog::logger> logger;
    double speedMultiplier;

   public:
    Ghost(const std::string& name);
    void draw(std::string shader);
    void draw(std::string shader, std::string name);
    void drawEyes(const std::string& shader);
    GhostMode getMode();
    void setMode(GhostMode newMode);
    float getMultiplier();
    void setMultiplier(double newSpeed);
    ~Ghost();
};