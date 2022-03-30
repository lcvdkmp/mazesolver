/*
 * mazesolver: mazesolver, reads a maze from a file and tries solving it
 * shows the progress real-time
 *
 * Author: Luca van der Kamp
 * Date: 2014-02-28
 */

#include <stdio.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "renderer.h"
#include "point.h"
#include "walkerdef.h"
#include "maze.h"
#include "solvers.h"

#define DEFAULT_STEPS 1000000
#define DEFAULT_WIDTH  80
#define DEFAULT_HEIGHT  24
#define DEFAULT_ALGO "wallfollower"
#define DEFAULT_DELAY 10

/* getopt variables */
extern char *optarg;
extern int optind;

extern int mazewarn;

/* the delay between render frames in nanoseconds */
unsigned int delay = DEFAULT_DELAY;
/* the maximum number of steps */
long steps = DEFAULT_STEPS;
/* 0 = no-render, 1 = render */
int render = 1;

/*
 * prints usage
 * err is the exit code that is parsed to exit()
 */
void usage(int err);

/*
 * prompts the user for an answer (y\Y\n\N)
 * keeps prompting until a valid character is given
 * returns 1 if y or Y is given 
 * returns 0 otherwise
 */
int prompt(const char *msg);

/* the algorithm that is used */
algorithm_t *algo;

int main (int argc, char **argv) {
    maze_t* maze;

    int sw = DEFAULT_WIDTH;
    int sh = DEFAULT_HEIGHT;

    char opt;
    while ((opt = getopt(argc, argv, "ha:cd:s:x:y:n")) != (char) -1) {
        switch (opt) {
            case 'a':
                algo = get_algo(optarg);
                if (!algo) {
                    fprintf(stderr, "algorithm '%s' not found, available" 
                            " algorithms:\n", optarg);
                    print_algos();
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
                usage(EXIT_SUCCESS);
                break;

            case 'd':
                delay = atoi(optarg);
                if (delay < 0) {
                    fprintf(stderr, "-d expects a positive integer or 0\n");
                    return EXIT_FAILURE;
                }
                break;

            case 'c':
                rs_set_coloured(1);
                break;

            case 's':
                steps = atol(optarg);
                if (steps <= 0L) {
                    fprintf(stderr, "-s expects a positive integer\n");
                    return EXIT_FAILURE;
                }
                break;

            case 'x':
                sw = atoi(optarg);
                if (sw <= 0) {
                    fprintf(stderr, "-x expects a positive integer\n");
                    return EXIT_FAILURE;
                }
                break;

            case 'y':
                sh = atoi(optarg);
                if (sh <= 0) {
                    fprintf(stderr, "-y expects a positive integer\n");
                    return EXIT_FAILURE;
                }
                break;

            case 'n':
                render = 0;
                break;

            case '?':
                return EXIT_FAILURE;
        }
    }

    /* no solver algorithm specified by user, use the default algorithm */
    if (!algo)
        algo = get_algo(DEFAULT_ALGO);

    if (argv[optind] == NULL)
        usage(EXIT_FAILURE);

    /* read and parse the maze */
    maze = read_maze(argv[optind]);

    if (!maze) {
        fprintf(stderr, "Error while reading maze file\n");
        return 1;
    }
    if (mazewarn) {
        int cont = prompt("There were some errors in the maze file, are you sure you want to continue?");
        if (!cont)
            return EXIT_SUCCESS;
    }
    
    walker_t *walker = init_walker(maze, algo->funct);

    /* change renderer settings and clear terminal */
    if (render) {
        rs_detect_dim();
        rs_set_focus(&(walker->pos));
        clear_term();
    }

    if(algo->init)
        algo->init(walker);

    long count = 0L;
    int err;
    while (count < steps) {
        count++;
        /* logic */
        walker_step(maze, walker);
        if (at_exit(maze, walker))
            break;
        /* render */
        if(render && (err = render_maze(maze, walker) != MRSUCC))
            mrerror("Error while rendering", err);
        /* sleep */
        /* FIXME: usleep is depricated, use nanosleep instead */
        if (delay)
            usleep(delay * 1000);
    }

    if (count < steps)
        printf("Found exit after %ld steps\n", count);

    /* free walker->state through algo, since algo is tasked with
     * (initialization and) cleanup of walker->state */
    if(algo->free)
        algo->free(walker->state);

    cleanup_walker(walker);
    return EXIT_SUCCESS;
}

void usage(int err) {
    printf(
        "mazesolver, reads a maze from a file and tries solving it\n"
        "            shows the progress real-time\n"
        "\n"
        "usage: mazesolver MAZE_FILE [-h|-v|-a ALGORITHM|-c|-d DELAY|-s STEPS"
        "|-x WIDTH|-y HEIGHT|-n]\n\n"
        "    -h             print the help page\n"
        "    -a ALGORITHM   set the algorithm to use\n"
        "    -c             use coloured output\n"
        "    -d DELAY       sets the delay between frames in milliseconds\n"
        "    -s MAX_STEPS   sets the maximum steps the algorithm takes\n"
        "    -x WIDTH       sets the width of the screen\n"
        "    -y HEIGHT      sets the width of the screen\n"
        "    -n             enables no render mode\n"
        );

    printf("\nThe following algorithms are available:\n");
    solver_usage("   - ");
    printf("\nSpecify a solver with the '-a' flag\n");
    printf("By default '%s' is used\n", DEFAULT_ALGO);
    exit(err);
}

int prompt(const char *msg) {
    printf("%s y/Y/n/N:\n", msg);
    char r = 'n';
    scanf("%c", &r);
    while (r != 'n' && r != 'N' && r != 'y' && r != 'Y')
        scanf("%c", &r);
    return ( r == 'y' || r == 'Y');
}
