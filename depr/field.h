/*
 * field.h
 *
 *  Created on: Feb 19, 2014
 *      Author: Toine
 */

#ifndef FIELD_H
#define FIELD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define XDIM		5		/* define x-dimension */
#define YDIM		5		/* define y-dimension */
#define ARR_LEN(a)	sizeof(a) / sizeof(a[0])		/* used to determine the length of arrays */

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327		/* not really nessecary since it is defined in math.h */
#endif

node field[XDIM][YDIM]; /* initialize the field */
char *checkpoints[] = { "00", "10", "20", "30", "41", "42", "43", "34", "24", "14", "03", "02", "01" }; /* item i in this array is */

int roadValue = 1;		/* time-value of driving the length of a road */
int cornerValue = 2;	/* time value of making a 90-degree turn */

int mines[40][2][2];

coord return_coord(int x, int y) {
	coord c;
	c.x = x;
	c.y = y;
	return c;
}

void init_field() {
	int x, y;

	for (y = 0; y < YDIM; y++) {
		for (x = 0; x < XDIM; x++) {

			/* set name according to coord */
			snprintf(field[x][y].name, 3, "%d%d", x, y);

			/* set coords */
			field[x][y].coords = return_coord(x, y);

			/* assign checkpoint numbers */
			field[x][y].checkpoint = name_to_checkpoint(field[x][y]);

			/* init neighbours */
			field[x][y].up = NULL;
			field[x][y].down = NULL;
			field[x][y].left = NULL;
			field[x][y].right = NULL;
		}
	}
}

void link_nodes() {
	int i, j;
	coord node_coords; /* define coord */

	for (j = 0; j < YDIM; j++) {
		for (i = 0; i < XDIM; i++) {
			// memcpy(&node_coords, &field[i][j].coords, sizeof(node_coords));
			node_coords = field[i][j].coords;

			if (j <= 3)
				field[i][j].up = &field[i][j + 1];
			else
				field[i][j].up = NULL;

			if (j >= 1)
				field[i][j].down = &field[i][j - 1];
			else
				field[i][j].down = NULL;

			if (i <= 3)
				field[i][j].right = &field[i + 1][j];
			else
				field[i][j].right = NULL;

			if (i >= 1)
				field[i][j].left = &field[i - 1][j];
			else
				field[i][j].left = NULL;

			/*for (b = 0; b < YDIM; b++) {
			 	for (a = 0; a < XDIM; a++) {
					if (field[a][b].coords[0] == node_coords[0] && field[a][b].coords[1] - 1 == node_coords[1])
					 	field[i][j].up = &field[a][b];
					else if (field[a][b].coords[0] == node_coords[0] && field[a][b].coords[1] + 1 == node_coords[1])
					 	field[i][j].down = &field[a][b];
					else if (field[a][b].coords[0] - 1 == node_coords[0] && field[a][b].coords[1] == node_coords[1])
					 	field[i][j].right = &field[a][b];
					else if (field[a][b].coords[0] + 1 == node_coords[0] && field[a][b].coords[1] == node_coords[1])
						field[i][j].left = &field[a][b];
					 }
			 } */
		}
	}
}

void print_field() {
	int i, j;
	for (j = YDIM - 1; j >= 0; j--) {
		for (i = 0; i < XDIM; i++) {
			printf("%s:(%d, %d)", field[i][j].name, field[i][j].coords.x,
					field[i][j].coords.y);
			if (i < XDIM - 1)
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

void find_neighbours(int x, int y) {
	printf("N:\t%s {%d}\n", field[x][y].name, field[x][y].checkpoint);

	if (field[x][y].up && field[x][y].up->checkpoint != 0)
		printf("U:\t%s {%d}\n", field[x][y].up->name,
				field[x][y].up->checkpoint);
	else if (field[x][y].up->name == 0)
		printf("U:\t-\n");
	else
		printf("U:\t%s\n", field[x][y].up->name);

	if (field[x][y].down && field[x][y].down->checkpoint != 0)
		printf("D:\t%s {%d}\n", field[x][y].down->name,
				field[x][y].down->checkpoint);
	else if (field[x][y].down->name == 0)
		printf("D:\t-\n");
	else
		printf("D:\t%s\n", field[x][y].down->name);

	if (field[x][y].right && field[x][y].right->checkpoint != 0)
		printf("R:\t%s {%d}\n", field[x][y].right->name,
				field[x][y].right->checkpoint);
	else if (field[x][y].right->name == 0)
		printf("R:\t-\n");
	else
		printf("R:\t%s\n", field[x][y].right->name);

	if (field[x][y].left && field[x][y].left->checkpoint != 0)
		printf("L:\t%s {%d}\n", field[x][y].left->name,
				field[x][y].left->checkpoint);
	else if (field[x][y].left->name == 0)
		printf("L:\t-\n");
	else
		printf("L:\t%s\n", field[x][y].left->name);

	printf("\n");
}

void checkpoint_to_coord(int checkpoint, int coords[]) {
	char name[] = "00";
	strcpy(name, checkpoints[checkpoint]);
	coords[0] = name[0] - '0';
	coords[1] = name[1] - '0';
}

int name_to_checkpoint(node n) {
	int i, a, b;
	for (i = 1; i <= 12; i++) {
		a = atoi(checkpoints[i]);
		b = atoi(n.name);
		if (a == b)
			return i;
	}
	return 0;
}

void print_route(int x1, int y1, int x2, int y2) {
	int x_now = x1, y_now = y1;

	while (x_now != x2 || y_now != y2) {
		printf("[%d, %d], ", x_now, y_now);

		if (x_now > x2)
			x_now -= 1;
		else if (x_now < x2)
			x_now += 1;
		else if (y_now > y2)
			y_now -= 1;
		else if (y_now < y2)
			y_now += 1;
		else
			printf("ERROR: next node not found! ");
	}

	printf("[%d, %d]", x_now, y_now);
}

int factorial(int n) {
	if (n <= 1)
		return 1;
	else
		return n * factorial(n - 1);
}

int num_of_routes(int x1, int y1, int x2, int y2) {
	int dx = abs(x1 - x2), dy = abs(y1 - y2);
	return factorial(dx + dy) / (factorial(dx) * factorial(dy));
}


vector rotate_vector(vector ori, double pi_factor) {
	vector new;
	new.x = (int) ori.x * cos(-pi_factor * M_PI)
			- (int) ori.y * sin(-pi_factor * M_PI);
	new.y = (int) ori.x * sin(-pi_factor * M_PI)
			+ (int) ori.y * cos(-pi_factor * M_PI);
	return new;
}

void clear_marks(void) {
	int x, y;

	for (y = 0; y < YDIM; y++)
		for (x = 0; x < XDIM; x++) {
			field[x][y].mark = 0;
			field[x][y].past = 0;
		}
}

void get_route(int start, int end) {
	int x, y, i, finished, f[] = { 0, 0 }, t[] = { 0, 0 }, j, min_mark = 0, z = 0;
	coord min_coords, from, to, current;
	coord route[40];

	checkpoint_to_coord(start, f);
	checkpoint_to_coord(end, t);

	from.x = f[0];
	from.y = f[1];
	to.x = t[0];
	to.y = t[1];

	printf("Start\t%d: (%d, %d)\n", start, from.x, from.y);
	printf("End\t%d: (%d, %d)\n\n", end, to.x, to.y);

	field[from.x][from.y].mark = 1;
	i = 1;
	finished = 0;

	while (!finished) {
		finished = 1;
		for (y = 0; y < YDIM; y++)
			for (x = 0; x < XDIM; x++)
				if (field[x][y].mark == i && !field[x][y].past) {
					finished = 0;
					if (field[x][y].up && !field[x][y].up->mark)
						field[x][y].up->mark = i + 1;
					if (field[x][y].down && !field[x][y].down->mark)
						field[x][y].down->mark = i + 1;
					if (field[x][y].right && !field[x][y].right->mark)
						field[x][y].right->mark = i + 1;
					if (field[x][y].left && !field[x][y].left->mark)
						field[x][y].left->mark = i + 1;
					field[x][y].past = 1;
				}
		i++;
	}

/*	for (y = YDIM - 1; y >= 0; y--) {
		for (x = 0; x < XDIM; x++) {
			printf("%d", field[x][y].mark);
			if (x < XDIM - 1)
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
*/

	current.x = to.x;
	current.y = to.y;

	for (j = 0; j < 40; j++) {
		route[j].x = -1;
		route[j].y = -1;
	}
	/*
	printf("Route initialized...\n");
	*/
	while (current.x != from.x || current.y != from.y) {
		/*
		printf("(%d, %d) ▷ ", current.x, current.y);
		*/

		route[z] = current;
		z++;

		node *neighbours[] = { field[current.x][current.y].up,
				field[current.x][current.y].down,
				field[current.x][current.y].right,
				field[current.x][current.y].left };
		for (j = 0; j <= 3; j++)
			if (neighbours[j]
					&& (!min_mark || neighbours[j]->mark < min_mark)) {
				min_mark = neighbours[j]->mark;
				min_coords.x = neighbours[j]->coords.x;
				min_coords.y = neighbours[j]->coords.y;
			}
		current = min_coords;
		// printf("   min_mark: %d\n", min_mark);
	}
	/*
	printf("(%d, %d)\n", current.x, current.y);
	*/

	route[z] = current;

	for (j = 39; j >= 0; j--) {
		if (route[j].x != -1) {
			printf("(%d, %d)", route[j].x, route[j].y);
			if (j) printf(" ▷ ");
			else printf("\n");
		}
	}

}

void init_mines() {
	int i, c = 0, x1, y1, x2, y2;

	static const char *file_name = "mines.txt";
	FILE *fp;
	int ch, lines;
	char *line = NULL;
	size_t len = 0;

	fp = fopen(file_name, "r");

	if (!fp) {
		printf("Error while opening %s.\n", file_name);
		exit(EXIT_FAILURE);
	}

	int m[c][2][2];
	for (i = 0; i < 40; i++) {
		if (getline(&line, &len, fp) == -1) break;
		m[i][0][0] = atoi(&line[0]);
		m[i][0][1] = atoi(&line[1]);
		m[i][1][0] = atoi(&line[3]);
		m[i][1][1] = atoi(&line[4]);
	}

	for (i = 0; i < ARR_LEN(mines); i++)
		if (mines[i][0][0])
			c++;
	/*
	printf("%d non-empty items in 'mines'.\n", c);
	*/

	for (i = 0; i < c; i++)
		memcpy(m[i], mines[i], sizeof(int[2][2]));
	/*
	printf("Coords of mines read.\n");
	*/

	for (i = 0; i < ARR_LEN(m); i++) {
		x1 = m[i][0][0];
		y1 = m[i][0][1];
		x2 = m[i][1][0];
		y2 = m[i][1][1];

		printf("mine: (%d, %d) - (%d, %d)\n", x1, y1, x2, y2);
		if (x1 < x2 && y1 == y2) {
			field[x1][y1].right = NULL;
			field[x2][y2].left = NULL;
		}
		if (x1 > x2 && y1 == y2) {
			field[x1][y1].left = NULL;
			field[x2][y2].right = NULL;
		}
		if (x1 == x2 && y1 < y2) {
			field[x1][y1].up = NULL;
			field[x2][y2].down = NULL;
		}
		if (x1 == x2 && y1 > y2) {
			field[x1][y1].down = NULL;
			field[x2][y2].up = NULL;
		}
	}
}

#endif /* FIELD_H */