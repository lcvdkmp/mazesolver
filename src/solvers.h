/*
 * The solvers
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#ifndef SOLVERS_H
#define SOLVERS_H
#include "maze.h"


/*
 * A maze solver algorithms.
 * init will be called before a move is executed.
 * init should return 1 on success, 0 on failure
 *
 * funct should generate a direction to move in and return it.
 *
 * name is the name used to denote the algorithm
 * description should describe the algorithm in a couple of lines.
 */
typedef struct algorithm_t {
	const char *name;
	direction_t (*funct)(maze_t *, walker_t *);

	/*
	 * called on initialisation with a pointer to the walker
	 * associated with the algorithm.
	 */
	int (*init)(walker_t *);

	/*
	 * called when state in walker must be freed.
	 * See walkerdef.h
	 */
	void (*free)(void *state);
	const char *description;
} algorithm_t;

/*
 * Print available algorithms
 */
void print_algos();

/* 
 * Get an algorithms from name.
 * Has O(n) complexity, where n is the number of
 * algorithms defined
 * 
 * name is the name of the algorithm
 *
 * returns a pointer to an algorithms struct if name is found
 * returns NULL if no name is found
 */
algorithm_t *get_algo(const char *name);


/*
 * Print the algorithm names with their description,
 * pre is printed before each algorithms.
 *
 * Print format:
 *     PRE NAME DESCRIPTION
 */
void solver_usage(const char *pre);
#endif
