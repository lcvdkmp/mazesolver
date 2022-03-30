/*
 * MR - the Maze renderer
 * Renders a maze
 *
 * This renderer only works on terminals with ANSI escape code 
 * support and UTF-8 support
 *
 * Tested successfully in:
 * - linux tty
 * - urxvt
 * - xterm
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "maze.h"
#include "walker.h"
#include "point.h"

/* the maze draw characters */
#define MR_WALL_HOR "─"
#define MR_WALL_VER "│"
#define MR_WALL_RIGHT_BOTTOM "┌"
#define MR_WALL_BOTTOM_LEFT "┐"
#define MR_WALL_TOP_RIGHT "└"
#define MR_WALL_TOP_LEFT "┘"
#define MR_WALL_TOP_RIGHT_BOTTOM "├"
#define MR_WALL_TOP_BOTTOM_LEFT "┤"
#define MR_WALL_RIGHT_BOTTOM_LEFT "┬"
#define MR_WALL_TOP_LEFT_RIGHT "┴"
#define MR_WALL_FULL "┼"
#define MR_WALL_EMPTY "."

/* colours */
#define CNRM  "\x1B[0m"
#define CRED  "\x1B[31m"
#define CGRN  "\x1B[32m"
#define CYEL  "\x1B[33m"
#define CBLU  "\x1B[34m"
#define CMAG  "\x1B[35m"
#define CCYN  "\x1B[36m"
#define CWHT  "\x1B[37m"

/* console sequences */
#define SQ_CLEAR "\e[1;1H\e[2J"
/* #define SQ_CLEAR "\033[2J" */
#define SQ_LINE_UP "\033[A"
#define SQ_GOTO_1_1 "\033[1;1H"

/* the special characters, walkers, start end ect */
#define MR_WALKER "@"
#define MR_START "S"
#define MR_EXIT "E"

/* maze renderer error codes */
enum {
    MRSUCC = 0,
    MRERR_INVARG = -1,
    MRERR_NOFOCUS = -2,
};

/* 
 * The state of the renderer.
 * The state is changed by the rs_* functions 
 */
typedef struct ren_state_t {
    /* if coloured output is used */
    int coloured;
    /* width and height of the screen */
    int w, h;
    /* the focus point of the view port */
    point_t *focus;

} ren_state_t;

/* 
 * a viewport.
 * The viewport is defined as two points, top left and bottom right.
 * The viewport defines what tiles are rendered by the renderer.
 * e.g. a 8 * 8 field is rendered, and a viewport of tl = (2, 1) br = (6, 7) 
 * is used:
 *
 * 0 0 0 0 0 0 0 0
 * 0 0 +-------+ 0
 * 0 0 | 1 1 1 | 0
 * 0 0 | 1 1 1 | 0
 * 0 0 | 1 1 1 | 0
 * 0 0 | 1 1 1 | 0
 * 0 0 | 1 1 1 | 0
 * 0 0 +-------+ 0
 *
 * Every tile denoted by 0 is not rendered.
 * Thus every tile denoted by 1 AND the tiles under the border are rendered
 */
typedef struct viewport_t {
    point_t tl, br;
} viewport_t;


/*
 * Render maze and walker
 */
int render_maze(maze_t *maze, walker_t *walker);

/*
 * Renders a wall on point p
 * Note: this function assumes that p is in fact a wall
 */
void render_wall(maze_t *m, point_t p);

/*
 * Renders the tile on point p
 */
void render_tile(maze_t *m, walker_t *w, point_t p);


/*
 * Calculates a viewport and stores it in vp
 */
int calc_viewport(maze_t *m, viewport_t *vp);

/*
 * Clear the terminal
 */
void clear_term(void);

/*
 * Clear all printed lines printed with mr_printf and mr_puts
 */
void clear_printed(void);

/*
 * Set the center/focus point of a viewport
 *
 * Warning: the point pointed to by p should stay alive once this function 
 * is called. When the point is deleted, call this function again with 
 * another focus point
 */
void rs_set_focus(point_t *p);

/*
 * Set the dimensions used to calculate the viewport.
 * w is the width of the viewport
 * h is the height of the viewport
 */
void rs_set_dimensions(int w, int h);

/*
 * If c = 1, use coloured output,
 * else use plain text
 */
void rs_set_coloured(int c);

/*
 * Detect dimensions of terminal and set the viewport accordingly.
 */
void rs_detect_dim(void);

/*
 * Print an error message given an error code err.
 */
void mrerror(const char *msg, int err);
#endif /* RENDERER_H */
