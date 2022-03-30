/*
 * typedef for point and direction
 * functions that work with points and directions
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#include "point.h"

void trans_point_dir(point_t *p, direction_t dir) {
    if(!p)
        return;

    switch (dir) {
        case NORTH:
            p->y--;
            break;

        case EAST:
            p->x++;
            break;

        case SOUTH:
            p->y++;
            break;

        case WEST:
            p->x--;
            break;
    }
}

int point_equals(point_t *p1, point_t *p2) {
    if (!p1 || !p2)
        return 0;
    return (p1->x == p2->x && p1->y == p2->y);
}

direction_t rotate_dir(direction_t dir, rotation_t rot, int n) {
    if (rot == LEFT)
        return (dir - n) % 4;
    else
        return (dir + n) % 4;
}

