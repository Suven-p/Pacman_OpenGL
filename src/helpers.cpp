#include <project/common.h>
#include <project/helpers.h>
#include <project/resourceManager.h>

std::string toString(DIRECTION d) {
    switch (d) {
        case DIRECTION::left:
            return "left";
        case DIRECTION::right:
            return "right";
        case DIRECTION::up:
            return "up";
        case DIRECTION::down:
            return "down";
        default:  // Not necessarily required but gives warning for void return
            return "";
    }
}

DIRECTION getOppositeDirection(DIRECTION current) {
    switch (current) {
        case DIRECTION::up:
            return DIRECTION::down;
        case DIRECTION::down:
            return DIRECTION::up;
        case DIRECTION::left:
            return DIRECTION::right;
        case DIRECTION::right:
            return DIRECTION::left;
        default:
            spdlog::error("Invalid direction provided to getOppositeDirection().");
            return DIRECTION::invalid;
    }
}
