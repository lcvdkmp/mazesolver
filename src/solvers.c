/*
 * The solvers
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "walkerdef.h"
#include "solvers.h"

/* init function of defined solver algorithms */
int init_rand_walker(walker_t *w);
int init_randi_walker(walker_t *w);
int init_wall_follower(walker_t *w);

/* solve function of defined solver algorithms */
direction_t rand_walker(maze_t *m, walker_t *w);
direction_t randi_walker(maze_t *m, walker_t *w);
direction_t wall_follower(maze_t *m, walker_t *w);

void free_walker_state(void *state) {
    if(state)
        free(state);
}

/*
 * all algorithms that are available.
 * Add a algorithms to the list to make it available to the program
 *
 * always keep the NULL struct at the end of the array, this is used to 
 * denote the end of the array when searching throught the array
 */
 algorithm_t algorithms[] = {
    {"random", rand_walker, init_rand_walker, free_walker_state, "Walkes in a random direction."},
    {"randomi", randi_walker, init_randi_walker, free_walker_state,
        "The same as random except that it favours a different direction "
        "than the one it came from."},
    {"wallfollower", wall_follower, init_wall_follower, free_walker_state, "Always keeps a wall on its right hand."},
    {NULL, NULL, NULL, NULL, NULL}
};

void print_algos() {
    algorithm_t *algo = algorithms;
    while (algo->name) {
        printf("- %s\n", algo->name);
        algo++;
    }
}

algorithm_t *get_algo(const char *name) {
    if (!name)
        return NULL;
    algorithm_t *algo = algorithms;
    while (algo->name) {
        if (strcmp(algo->name, name) == 0)
            return algo;
        algo++;
    }
    return NULL;
}

int init_rand_walker(walker_t *w) {
    srand(time(NULL));
    return 1;
}

direction_t rand_walker(maze_t *m, walker_t *w) {
    /* get all valid directions the walker can go */
    direction_t dirs[4];
    int valdirs = 0;
    for (int dir = 0; dir < 4 ; dir++) {
        if (check_move(m, w, dir)) {
            /* fprintf(stderr, "valid move found: %i\n", dir); */
            dirs[valdirs] = dir;
            valdirs++;
        }
    }

    /* if valid directions are found, return a random direction */
    if (valdirs > 0)
        return dirs[rand() % valdirs];
    else
        return -1;
}

int init_randi_walker(walker_t *w) {
    srand(time(NULL));
    if (w->state != NULL)
        return 0;

    w->state = malloc(sizeof(direction_t));
    return 1;
}

direction_t randi_walker(maze_t *m, walker_t *w) {

    direction_t od = *((direction_t *) w->state);
    od = rotate_dir(od, LEFT, 2);

    direction_t dirs[4];
    int valdirs = 0;
    for (unsigned int dir = 0; dir < 4; dir++) {
        if (check_move(m, w, dir) && dir != od) {
            /* fprintf(stderr, "valid move found: %i\n", dir); */
            dirs[valdirs] = dir;
            valdirs++;
        }
    }

    /* if valid directions are found, return a random direction */
    direction_t nd;
    if (valdirs > 0)
        nd = dirs[rand() % valdirs];
    else
        nd = od;

    *((direction_t *)w->state) = nd;
    return nd;
}

int init_wall_follower(walker_t *w) {
    if (w->state != NULL)
        return 0;
    w->state = malloc(sizeof(direction_t));
    return 1;
}

direction_t wall_follower(maze_t *m, walker_t *w) {
    if (!w->state)
        return -1;

    direction_t cd = *((direction_t *) w->state);

    /* go right by default */
    cd = rotate_dir(cd, RIGHT, 1);

    /* keep rotating left until correct path is found */
    while (!check_move(m, w, cd)) {
        cd = rotate_dir(cd, LEFT, 1);
    }

    *((direction_t *)w->state) = cd;
    return cd;
}

void solver_usage(const char *pre) {
    /* get max string length of name */
    int mstrlen = 0;
    algorithm_t *a = algorithms;

    while (a->name) {
        int nl = strlen(a->name);
        if (nl > mstrlen)
            mstrlen = nl;
        a++;
    }

    a = algorithms;
    while (a->name) {
        printf("%s%*.64s    %s\n", pre, mstrlen, a->name, a->description);
        a++;
    }
}


