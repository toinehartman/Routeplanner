/* 	Projectgroep D2
	Luc, Wesley en Toine
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "data.h"
#include "functions.h"

int verbosity = 0;

int main(int argc, char* argv[]) {
	int fixed_order = 0;
	int i, j, *cp, *new_cp, cp_num, a;
	
	cp_num = argc - 1;

	for (i = 0; i < argc; i++) {
		if (strncmp(argv[i], "--fixed", 7))
			fixed_order = 1;
	}

	cp = (int*) calloc(cp_num, sizeof(int));
	// printf("cp created, %d elements of %ld bytes\n", cp_num, sizeof(int));

	if (cp_num >= 1) { /* als checkpoints in argument zijn meegegeven, deze gebruiken */
		for (i = 0; i < cp_num; i++) {
			a = atoi(argv[i + 1]);

			if (a >= 1 && a <= 12)
				cp[i] = atoi(argv[i + 1]);
			else {
				printf("%d is not a number of an existing checkpoint!!\n", a);
				exit(EXIT_FAILURE);
			}
		}

		if (cp[0] != START_CP) {
			cp_num++;
			if ((new_cp = (int*) realloc(cp, (cp_num) * sizeof(int))) == NULL) {
				free(cp);
				printf("ERROR: Unable to allocate enough memory for all destinations!\n");
				exit(EXIT_FAILURE);
			}
			cp = new_cp;
			free (new_cp);
			
			for (i = cp_num - 1; i > 0; i--)
				cp[i] = cp[i - 1];
			cp[0] = START_CP;
		}

		printf("-----------------------------------\n");
		printf("Number of target checkpoints: %d\n", cp_num - 1);

		/* print checkpoints to go to */
		printf("Start at %d, go to: ", cp[0]);
		for (i = 1; i < cp_num; i++)
			printf("%d ", cp[i]);
		printf("\n-----------------------------------\n\n");
	} 
	else { /* anders ERROR */
		printf("At least 1 argument required!\n");
		exit(EXIT_FAILURE);
	}

	printf("\n");

	init_field();
	link_nodes();
	print_field();

	if (fixed_order) {
		/* sort cp on shortest route */
		short_sort(cp, cp_num);

		printf("-------\nSorted!\n");
		for (j = 0; j < cp_num; j++)
			printf("%d ", cp[j]);
		printf("\n-------\n\n");
	}
	else {
		for (i = 0; i < cp_num; i++)
			printf("%d ", cp[i]);
		printf("\n\n");
	}

	if (read_mines(1) == 1) printf("No mine file!\n");

	/* go through all checkpoints */
	route_sequence(cp, cp_num);

	return 0;
}
