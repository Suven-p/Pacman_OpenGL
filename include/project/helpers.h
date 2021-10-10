#ifndef HELPERS_HPP_GUARD
#define HELPERS_HPP_GUARD

#include <project/common.h>
#include <spdlog/spdlog.h>
#include <string>

void draw_gridlines();

enum struct DIRECTION : int
{
    right = 0,
    down = 1,
    left = 2,
    up = 3,
    invalid
};

std::string toString(DIRECTION d);
DIRECTION getOppositeDirection(DIRECTION current);

constexpr auto addPosition =
    [](const auto& position, const auto& diff, const DIRECTION& direction) {
        auto newPosition = position;
        switch (direction) {
            case DIRECTION::up: {
                newPosition.second -= diff;
                break;
            }
            case DIRECTION::down: {
                newPosition.second += diff;
                break;
            }
            case DIRECTION::left: {
                newPosition.first -= diff;
                break;
            }
            case DIRECTION::right: {
                newPosition.first += diff;
                break;
            }
            case DIRECTION::invalid: {
                spdlog::error("Invalid Direction provided to function addPosition().");
                break;
            }
        }
        return newPosition;
    };

struct windowData {
    int* argc{};
    char** argv{};
    int height{};
    int width{};
    int refreshInterval{60};
    bool isResizable;
    bool initialVisible;
    bool initialFocus;
    bool isDecorated;
};

#endif
