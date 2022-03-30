/*
 * File: maze.h
 * 
 * Functions that work with a maze
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#ifndef MAZE_H
#define MAZE_H
#include "point.h"

/* size of blocks the maze file is read in */
#define BLOCK_SIZE 1024

/* tile characters as found in maze */
#define WALL    '#'
#define START   'S'
#define EXIT    'E'
#define OPEN	' '

typedef struct maze_t maze_t;
typedef struct walker_t walker_t;

/*
 * Initialized a maze with r rows and c columns
 *
 * Note: it is expected that the user frees the maze with cleanup_maze()
 */
maze_t* init_maze(int r, int c);

/*
 * Reads a maze from a file
 * fname is the path of the file that contains the maze
 *
 * When a fatal error is found in the file, the function prints a message 
 * and returns NULL. When an error occurs, mazewarn is set to one.
 *
 * Fatal errors occur when:
 *     - The borders of the maze are not all walls
 *     - an invalid characer is found
 * 
 * A normal error occurs when:
 *     - A second entrance is found
 */
maze_t* read_maze(const char *fname);

/*
 * Returns 1 if c is a valid maze char,
 * returns 0 if it is not
 */
int val_maze_char(char c);

/*
 * Cleans up the maze pointed to by m
 * This functions should be called whenever a maze created with init_maze() is
 * not needed anymore
 */
void cleanup_maze(maze_t *m);

/*
 * Returns the tile found if one step in direction dir is taken from point p
 */
char tile_dir(maze_t *m, point_t p, direction_t dir);

#endif /* MAZE_H */
