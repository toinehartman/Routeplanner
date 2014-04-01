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
void print_field();
void find_neighbours(coord n);
coord checkpoint_to_coord(int checkpoint);
int node_to_checkpoint(node n);
void clear_marks();
void get_route(coord from, coord to);
void route_marks(coord from, coord to);
void print_route_marks();
int route_len(coord a, coord b);
void short_sort(int *check, int num);
void route_sequence(int *checks, int checks_num);
int read_mines();
void place_mine(coord a, coord b);
char drive_direction(coord past, coord now, coord to);
int compass_direction(coord from, coord to);

#endif /* FUNCTIONS_H */
