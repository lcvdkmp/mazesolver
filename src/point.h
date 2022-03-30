/*
 * typedef for point and direction
 * functions that work with points and directions
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#ifndef POINT_H
#define POINT_H

typedef struct {
    int x, y;
} point_t;

typedef enum {
    NORTH = 0,
    EAST = 1,
    SOUTH,
    WEST,
} direction_t;

typedef enum {
    LEFT = 0,
    RIGHT = 1,
} rotation_t;

/*
 * translates a point as if a single step is done in a direction
 *
 * dir is the direction to move in
 * p is the point to translate
 */
void trans_point_dir(point_t *p, direction_t dir);

/*
 * checks if p1 equals p2
 *
 * returns 1 if p1 equals p2
 * returns 0 if p1 doesn't equal p2
 */
int point_equals(point_t *p1, point_t *p2);

/*
 * rotates the direction
 * 
 * dir is the direction to rotate
 * rot is the rotation direction
 * dir is rotated n times
 *
 * returns the rotated direction
 */
direction_t rotate_dir(direction_t dir, rotation_t rot, int n);
#endif /* POINT_H */
