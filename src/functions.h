/**
 @file functions.h The function header file.
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Windows.h>
#include <stdbool.h>

#include "main.h"
#include "data.h"

#define XDIM		5		/* define x-dimension */
#define YDIM		5		/* define y-dimension */
#define ARR_LEN(a)	(sizeof(a) / sizeof(a[0]))		/* used to determine the length of arrays */

extern node field[XDIM][YDIM];

coord return_coord(int x, int y);
void init_field();
void link_nodes();

//! Print the field.
/*!
 * Print all the checkpoints and nodes of the fields.
 */
void print_field();

/**
 * Find the neighbours....
 *
 * @param n Coordinate to find neighbours of.
 * @deprecated
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
 * @see route_marks()
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
void iterate_route(HANDLE hSerial, char *byteBuffer, int init_dir, coord to);

/**
 * Mark all nodes according to Lee algorithm.
 *
 * @param from The starting point of the route.
 * @param to The destination of the route.
 * @see find_shortest_route()
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

/**
 * Sort all destination checkpoints for the shortest order i.e. shortest total route length.
 *
 * @param check Array of numbers of all destination checkpoints in random order.
 * @param num Number of destination checkpoints i.e. length of the int array 'check'.
 */
void short_sort(int *check, int num);

/**
 * Find the route from checkpoint i to checkpoint (i + 1) in the checkpoint array, in the given order. Go to all given checkpoints this way.
 *
 * @param checks Array of destination checkpoints in order.
 * @param checks_num Number of checkpoints i.e. the length of the 'checks' array.
 */
void route_sequence(HANDLE hSerial, char *byteBuffer, int *checks, int checks_num);

/**
 * Reads mines from the mines file and calls add_mine_to_field().
 *
 * @return 0 if successfull, 1 if mines file not found.
 * @see add_mine_to_field()
 */
int read_mines();

/**
 * Saves a found mine to the mines file.
 *
 * @param a, b Coords between which the mine is present.
 */
void save_mine_to_file(coord a, coord b);

/**
 * Add a mine to the field i.e. disconnect nodes between
 * which the mine is present.
 *
 * @param a, b Coord between which the mine is present.
 */
void add_mine_to_field(coord a, coord b);

/**
 * Give a drive (relative) direction (right, forward etc.).
 *
 * @param init_dir Initial compass direction before driving in the direction this fucntion will return.
 * @param now Current location's coordinate.
 * @param to Coordinate of the next node to go to.
 * @return Direction to drive to.
 * @see drive_to_cp_direction()
 */
char drive_direction(int init_dir, coord now, coord to);

/**
 * Give a drive (relative) direction (right, forward etc.).
 *
 * @param prev_dir Initial  compass direction.
 * @param new_dir Future compass direction.
 * @return Direction to drive to.
 * @see compass_direction()
 */
char drive_to_cp_direction(int prev_dir, int new_dir);

/**
 * Gives the compass direction (north, west etc.) from one coord to another.
 *
 * @param from Starting coordinate.
 * @param to Destination coordinate.
 * @return int of the compass direction N = 0, E = 1, etc.).
 */
int compass_direction(coord from, coord to);

/**
 * Convert the int of a compass direction to an understandable char.
 *
 * @param comp The number of the direction.
 * @return The first character of the name of the direction.
 */
char compass_int(int comp);

/**
 * Gives the compass direction number in which a certain checkpoint is left.
 *
 * @param cp The specific checkpoint.
 * @return The compass direction when leaving this checkpoint.
 */
int cp_direction(int cp);

char zigbee_write(HANDLE hSerial, char *byteBuffer, char command, bool checkpoint);

#endif /* FUNCTIONS_H */
