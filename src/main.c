/* 	Projectgroep D2
	Luc, Wesley en Toine
*/

#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "functions.h"

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
	print_field();

	clear_marks();
	
	init_mines();
	
	get_route(c1, c2);

	printf("\n");

	return 0;
}
