/* 	Projectgroep D2
	Luc, Wesley en Toine
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "data.h"
#include "functions.h"

int main(int argc, char* argv[]) {
	int fixed_order = 0;

	int i, j, *cp, cp_num, a;
	cp_num = argc - 1;

	for (i = 0; i < argc; i++) {
		if (strncmp(argv[i], "--fixed", 7))
			fixed_order = 1;
	}

	cp = (int*) calloc(cp_num, sizeof(int));
	// printf("cp created, %d elements of %ld bytes\n", cp_num, sizeof(int));

	if (cp_num >= 1) { /* als checkpoints in argument zijn meegegeven, deze gebruiken */
		printf("Number of target checkpoints: %d\n", cp_num);

		for (i = 0; i < cp_num; i++) {
			a = atoi(argv[i + 1]);

			if (a >= 1 && a <= 12)
				cp[i] = atoi(argv[i + 1]);
			else {
				printf("%d is not a number of an existing checkpoint!!\n", a);
				return 1;
			}
		}
	} 
	else { /* anders ERROR */
		printf("At least 1 argument required!\n");
		return 1;
	}

	printf("\n");

	init_field();
	link_nodes();
	print_field();

	if (fixed_order != 0) {
		short_sort(cp, cp_num);

		printf("Sorted!\n");
		for (j = 0; j < cp_num; j++)
			printf("%d ", cp[j]);
		printf("\n");
	}
	else {
		for (i = 0; i < cp_num; i++)
			printf("%d ", cp[i]);
		printf("\n");
	}

	return 0;
}
