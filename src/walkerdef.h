#ifndef WALKERDEF_H
#define WALKERDEF_H
#include "point.h"
#include "walker.h"
#include "maze.h"

struct walker_t {
    /* the position of the walker */
    point_t pos;

    /* the algorithm that is used to generate a new step */
    direction_t (*algo)(maze_t *, walker_t *);

    /*
     * The state of the walker.
     * No context is given as to what this is, 
     * the solver function saves and interprets this state.
     * 
     * Note: this is not freed by the solver function
     *       when deleting this struct, free this
     */
    void *state;
};
#endif /* WALKERDEF_H */
