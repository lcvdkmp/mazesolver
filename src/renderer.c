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

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <string.h>
#include "renderer.h"
#include "mazedef.h"
#include "walkerdef.h"

/* the state of the renderer */
ren_state_t rs;

/* the viewport that is used when rendering */
viewport_t vp;

void clear_term() {
    printf(SQ_CLEAR);
    fflush(stdout);
}

void clear_printed() {
    printf("%s", SQ_GOTO_1_1);
    fflush(stdout);
}

int render_maze(maze_t *m, walker_t *w) {

    /* clear lines */
    clear_printed();

    if (!m || !w)
        return MRERR_INVARG;

    int err;
    /* calculate viewport */
    if ((err = calc_viewport(m, &vp)) != MRSUCC)
        return err;

    /* render tiles using viewport */
    for (int r = vp.tl.y; r < vp.br.y; r++) {
        for (int c = vp.tl.x; c < vp.br.x; c++) {
            point_t p = {c, r};
            render_tile(m, w, p);
        }
        putchar('\n');
    }

    fflush(stdout);
    return MRSUCC;
}

int calc_viewport(maze_t *m, viewport_t *vp) {
    if (!m || !vp)
        return MRERR_INVARG;

    if (!rs.focus)
        return MRERR_NOFOCUS;
    
    /* generate data */
    vp->tl.x = rs.focus->x - rs.w/2;
    vp->tl.y = rs.focus->y - rs.h/2;

    vp->br.x = rs.focus->x + rs.w/2;
    vp->br.y = rs.focus->y + rs.h/2;

    /* clamp generated data */
    if (vp->tl.x < 0) {
        /* keep aspect ration by shoving viewport to the right */
        vp->br.x += -1 * vp->tl.x;
        vp->tl.x = 0;
    }
    if (vp->tl.y < 0) {
        /* keep aspect ration by shoving viewport down */
        vp->br.y += -1 * vp->tl.y;
        vp->tl.y = 0;
    }

    if (vp->br.x > m->c) {
        vp->tl.x -= vp->br.x - m->c;
        vp->br.x = m->c;

        /* the viewport can be out of bounds again, make sure that is isn't */
        if (vp->tl.x < 0)
            vp->tl.x = 0;
    }
    if (vp->br.y > m->r) {
        vp->tl.y -= vp->br.y - m->r;
        vp->br.y = m->r;

        /* the viewport can be out of bounds again, make sure that is isn't */
        if (vp->tl.y < 0)
            vp->tl.y = 0;
    }

    return MRSUCC;
}

void render_tile(maze_t *m, walker_t *w, point_t p) {
    if(point_equals(&p, &(w->pos))) {
        if (rs.coloured)
            printf(CBLU "%s" CNRM, MR_WALKER);
        else
            printf(MR_WALKER);

        return;
    }

    switch (m->maze[p.y][p.x]) {
        case WALL:
            render_wall(m, p);
            break;
        case START:
            if (rs.coloured)
                printf(CGRN "%s" CNRM, MR_START);
            else
                printf(MR_START);
            break;
        case EXIT:
            if (rs.coloured)
                printf(CRED "%s" CNRM, MR_EXIT);
            else
                printf(MR_EXIT);
            break;
        default:
            putchar(m->maze[p.y][p.x]);
    }
}

void render_wall(maze_t *m, point_t p) {
    int walls[4] = {0};

    for (int i = 0; i < 4; i++)
        walls[i] = (tile_dir(m, p, i) == WALL);

    if(walls[WEST] && walls[EAST] && walls[NORTH] && walls[SOUTH])
        printf("%s", MR_WALL_FULL);
    else if(walls[WEST] && walls[EAST] && walls[NORTH])
        printf("%s", MR_WALL_TOP_LEFT_RIGHT);
    else if(walls[WEST] && walls[EAST] && walls[SOUTH])
        printf("%s", MR_WALL_RIGHT_BOTTOM_LEFT);
    else if(walls[WEST] && walls[NORTH] && walls[SOUTH])
        printf("%s", MR_WALL_TOP_BOTTOM_LEFT);
    else if(walls[SOUTH] && walls[NORTH] && walls[EAST])
        printf("%s", MR_WALL_TOP_RIGHT_BOTTOM);
    else if(walls[WEST] && walls[NORTH])
        printf("%s", MR_WALL_TOP_LEFT);
    else if(walls[NORTH] && walls[EAST])
        printf("%s", MR_WALL_TOP_RIGHT);
    else if(walls[SOUTH] && walls[WEST])
        printf("%s", MR_WALL_BOTTOM_LEFT);
    else if(walls[SOUTH] && walls[EAST])
        printf("%s", MR_WALL_RIGHT_BOTTOM);
    else if(walls[NORTH] || walls[SOUTH])
        printf("%s", MR_WALL_VER);
    else if(walls[WEST] || walls[EAST])
        printf("%s", MR_WALL_HOR);
    else
        printf("%s", MR_WALL_EMPTY);
}


/* 
 * functions to change the renderer settings 
 */

void rs_set_coloured(int c) {
    rs.coloured = c;
}

void rs_detect_dim(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    rs_set_dimensions(w.ws_col, w.ws_row);
}

void rs_set_focus(point_t *p) {
    rs.focus = p;
}

void rs_set_dimensions(int w, int h) {
    rs.w = w;
    rs.h = h;
}

void mrerror(const char *msg, int err) {
    if (err == MRSUCC)
        return;

    const char *e;
    switch (err) {
        case MRERR_INVARG:
            e = "One or more arguments are invalid";
            break;
        case MRERR_NOFOCUS:
            e = "Viewport focus is not defined";
            break;
        default:
            e = "Unknown error";
    }

    if (rs.coloured)
        fprintf(stderr, CRED "%s: %s\n" CNRM, msg, e);

    else
        fprintf(stderr, "%s: %s\n", msg, e);

}
