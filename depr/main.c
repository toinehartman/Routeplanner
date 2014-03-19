/*
 * main.c
 *
 *  Created on: Feb 18, 2014
 *      Author: Toine
 */

#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "field.h"

int main(int argc, char* argv[]) {
	int i, c1, c2, x1, x2, y1, y2;
	int n1[] = { 0, 0 }, n2[] = { 0, 0 };
	int printfield = 0;

	if (argc - 1 >= 2) { /* als checkpoints in argument zijn meegegeven, deze gebruiken */
		c1 = atoi(argv[1]);
		checkpoint_to_coord(c1, n1);

		c2 = atoi(argv[2]);
		checkpoint_to_coord(c2, n2);

		x1 = n1[0];
		y1 = n1[1];
		x2 = n2[0];
		y2 = n2[1];

	} else { /* anders ERROR */
		printf("2 arguments required!\n");
		return 1;
	}

	printf("\n");

	init_field();
	link_nodes();
	if (printfield) print_field();

	/*
	find_neighbours(x1, y1);
	find_neighbours(x2, y2);
	printf("Mogelijkheden van checkpoint %d (%d, %d) naar checkpoint %d (%d, %d): %d.\n", c1, x1, y1, c2, x2, y2, num_of_routes(x1, y1, x2, y2));
 printf("\n{%d}: ", c1);
	print_route(x1, y1, x2, y2);
	printf(": {%d}\n\n", c2);
	*/

	clear_marks();
	/*
	init_mines();
	*/
	get_route(c1, c2);

	printf("\n");

	return 0;
}
