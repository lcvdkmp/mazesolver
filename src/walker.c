/*
 * Functions that create and work on the walker
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#include <stdlib.h>
#include <stdio.h>
#include "walkerdef.h"
#include "mazedef.h"
#include "renderer.h"
int check_move(maze_t *m, walker_t *w, direction_t dir) {
    if (!m || !w)
        return 0;
    if (tile_dir(m, w->pos, dir) == WALL)
        return 0;
    return 1;
}

int at_exit(maze_t *m, walker_t *w) {
    return point_equals(&(m->exit), &(w->pos));
}

walker_t* init_walker(maze_t *maze, direction_t (*algo)(maze_t *, walker_t *)) {
    /* static int nwalkers = 0; */
    walker_t *w = malloc(sizeof(walker_t));
    if (!maze || !maze->maze || !w || !algo)
        return NULL;
    
    w->pos.x = maze->start.x;
    w->pos.y = maze->start.y;
    w->state = NULL;
    w->algo = algo;
    return w;
}

int move_walker(maze_t *m, walker_t *w, direction_t dir) {
    if (!w || !m)
        return 0;
    
    if (check_move(m, w, dir))
        trans_point_dir(&(w->pos), dir);
    return 1;
}

int walker_step(maze_t *m, walker_t *w) {
    if (!m || !w)
        return 0;
    direction_t dir = w->algo(m, w);
    return move_walker(m, w, dir);
}

void cleanup_walker(walker_t *w) {
    if (!w)
        return;
    /* don't cleanup walker->state here. This should be done by the solver. */
    free(w);
}
