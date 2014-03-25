/* 	Projectgroep D2
	Luc, Wesley en Toine
*/

#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "functions.h"

int main(int argc, char* argv[]) {
	int i, arg1, arg2;
	coord one, two;
	int n1[] = { 0, 0 }, n2[] = { 0, 0 };
	int printfield = 0;

	if (argc - 1 >= 2) { /* als checkpoints in argument zijn meegegeven, deze gebruiken */
		arg1 = atoi(argv[1]);
		one = checkpoint_to_coord(arg1);

		arg2 = atoi(argv[2]);
		one = checkpoint_to_coord(arg2);
	} 
	else { /* anders ERROR */
		printf("2 arguments required!\n");
		return 1;
	}

	printf("\n");

	init_field();
	link_nodes();
	print_field();
	get_route(arg1, arg2);

	printf("\n");
	return 0;
}
