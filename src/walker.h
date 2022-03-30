/*
 * Functions that create and work on the walker
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#ifndef WALKER_H
#define WALKER_H

#include "point.h"

typedef struct walker_t walker_t;
typedef struct maze_t maze_t;

/*
 * Initializes a walker.
 *
 * maze is the maze the walker is in
 * algo is the algorithm the walker should use to generate the moves
 *
 * Note: is is expected that the walker created is freed with
 * the cleanup_walker() function
 *
 * returns an initialized walker on success
 * returns NULL on failure
 */
walker_t* init_walker(maze_t *maze, direction_t (*algo)(maze_t *, walker_t *));

/*
 * Move a walker in a direction.
 *
 * m is the maze in which the walker should move  
 * w is the walker that should be moved
 * dir is the direction that the walker should be moved in.
 *
 * returns 1 on success
 * returns 0 on failure
 */
int move_walker(maze_t *m, walker_t *w, direction_t dir);

/*
 * cHecks if a walker is at the exit.
 * 
 * w is the walker
 * m is the maze the walker is in
 *
 * returns 1 if at exit
 * returns 0 if not at exit
 */
int at_exit(maze_t *m, walker_t *w);

/*
 * Checks if a given move (a single motion in a direction) is valid.
 *
 * w is the walker that executes the move
 * m is the maze the walker is in
 * dir is the direction that is moved in
 *
 * returns 1 if move is valid
 * returns 0 if move is not valid
 */
int check_move(maze_t *m, walker_t *w, direction_t dir);

/*
 * Executes a single step fo a walker,
 * calculates a move and moves the walker one step
 *
 * w is the walker
 * m is the maze the walker is in
 *
 * returns 1 on success
 * returns 0 on failure
 */
int walker_step(maze_t *m, walker_t *w);

/*
 * Cleans up a walker, freeing the memory.
 *
 * w is the walker to free
 */
void cleanup_walker(walker_t *w);
#endif
