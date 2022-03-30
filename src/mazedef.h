#ifndef MAZEDEF_H
#define MAZEDEF_H
#include "point.h"
#include "maze.h"
struct maze_t {
    int r, c;
    char **maze;
    point_t start;
    point_t exit;
};

#endif /* MAZEDEF_H */
