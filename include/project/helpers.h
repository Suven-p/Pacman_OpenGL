#ifndef HELPERS_HPP_GUARD
#define HELPERS_HPP_GUARD

#include <utility>

enum struct DIRECTION : int
{
    right = 0,
    down = 1,
    left = 2,
    up = 3
};
typedef std::pair<double, double> COORD;

struct freeglutData
{
    int *argc;
    char **argv;
    int height;
    int width;
    int refreshInterval;
};

#endif
