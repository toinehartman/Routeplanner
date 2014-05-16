#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "main.h"
#include "data.h"

#define XDIM		5		/* define x-dimension */
#define YDIM		5		/* define y-dimension */
#define ARR_LEN(a)	(sizeof(a) / sizeof(a[0]))		/* used to determine the length of arrays */

extern node field[XDIM][YDIM];

coord return_coord(int x, int y);
void init_field();
void link_nodes();

/**
 * Print the field.
 */
void print_field();

/**
 * Find the neighbours....
 *
 * @param n Coordinate to find neighbours of.
 */
void find_neighbours(coord n);

/**
 * Find the coordinates of a specific checkpoint.
 *
 * @param checkpoint Checkpoint to find coordinates of.
 * @return The coordinate of the given checkpoint if the checkpoint exists, (coord){-1, -1} otherwise.
 */
coord checkpoint_to_coord(int checkpoint);

/**
 * Find to which checkpoint a node is connected.
 *
 * @param n Node to find the connected checkpoint of.
 * @return The number of the checkpoint.
 */
int node_to_checkpoint(node n);

/**
 * Clear all the marks used in Lee algorithm; clearing means setting to zero.
 */
void clear_marks();

/**
 * Applies Lee algorithm (http://en.wikipedia.org/wiki/Lee_algorithm) to find
 * the shortest route between two points.
 *
 * @param from The starting point of the route.
 * @param to The destination point of the route.
 */
void find_shortest_route(coord from, coord to);

/**
 * Iterate over the array created by clear_marks() and give directions.
 *
 * @param init_dir Initial direction. The direction in which the vehicle is before following the routes directions.
 * @param to The destination coordinate.
 */
void iterate_route(int init_dir, coord to);

/**
 * Mark all nodes according to Lee algorithm.
 * 
 * @param from The starting point of the route.
 * @param to The destination of the route.
 */
void route_marks(coord from, coord to);

/**
 * Print all route marks
 */
void print_route_marks();

/**
 * Give the length of the shortest route from coordinate a to coordinate b.
 * 
 * @param a Starttig coordinate of the route.
 * @param b Destination.
 * @return The length of the route.
 */
int route_len(coord a, coord b);
void short_sort(int *check, int num);
void route_sequence(int *checks, int checks_num);
int read_mines();
void save_mine_to_file(coord a, coord b);
void add_mine_to_field(coord a, coord b);
char drive_direction(int prev_dir, coord now, coord to);
char drive_to_cp_direction(int prev_dir, int new_dir);
int compass_direction(coord from, coord to);
char compass_int(int comp);
int cp_direction(int cp);

#endif /* FUNCTIONS_H */
