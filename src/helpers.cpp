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