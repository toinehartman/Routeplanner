#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define XDIM		5		/* define x-dimension */
#define YDIM		5		/* define y-dimension */
#define ARR_LEN(a)	(sizeof(a) / sizeof(a[0]))		/* used to determine the length of arrays */

#include "data.h"

coord return_coord(int x, int y);
void init_field();
void link_nodes();
void print_field();
void find_neighbours(int x, int y);
coord checkpoint_to_coord(int checkpoint);
int node_to_checkpoint(node n);
void print_route(int x1, int y1, int x2, int y2);
void clear_marks(void);
void get_route(int start, int end);

#endif
