#ifndef HELPERS_HPP_GUARD
#define HELPERS_HPP_GUARD

#include <project/common.h>

void draw_gridlines();

enum struct DIRECTION : int
{
    right = 0,
    down = 1,
    left = 2,
    up = 3
};

struct freeglutData
{
    int *argc;
    char **argv;
    int height;
    int width;
    int refreshInterval;
};

#endif
