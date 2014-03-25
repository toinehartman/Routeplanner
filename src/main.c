/* 	Projectgroep D2
	Luc, Wesley en Toine
*/

#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "functions.h"

int main(int argc, char* argv[]) {
	int i, cp1, cp2;
	coord one, two;

	if (argc - 1 >= 2) { /* als checkpoints in argument zijn meegegeven, deze gebruiken */
		cp1 = atoi(argv[1]);
		one = checkpoint_to_coord(cp1);

		cp2 = atoi(argv[2]);
		two = checkpoint_to_coord(cp2);
	} 
	else { /* anders ERROR */
		printf("2 arguments required!\n");
		return 1;
	}

	printf("\n");

	init_field();
	link_nodes();
	print_field();
	get_route(one, two);

	printf("\n");
	return 0;
}
