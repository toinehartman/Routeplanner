#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <math.h>

#include "main.h"
#include "data.h"
#include "functions.h"

node field[XDIM][YDIM]; /* initialize the field */
char *checkpoints[] = { "00", "10", "20", "30", "41", "42", "43", "34", "24", "14", "03", "02", "01" }; /* item i in this array is */

static const int roadValue = 1;		/* time-value of driving the length of a road */
static const int cornerValue = 2;	/* time value of making a 90-degree turn */
static coord current;

/* returns coord struct from given coordinates */
coord return_coord(int x, int y) {
	coord c;
	c.x = x;
	c.y = y;
	return c;
}

/* initializes all nodes */
void init_field() {
	int x, y;

	/* iterate over all possible x and y coordinates */
	for (y = 0; y < YDIM; y++) {
		for (x = 0; x < XDIM; x++) {

			/* set name according to coord */
			snprintf(field[x][y].name, 3, "%d%d", x, y);

			/* set coords */
			field[x][y].coords = return_coord(x, y);

			/* assign checkpoint numbers */
			field[x][y].checkpoint = node_to_checkpoint(field[x][y]);

			/* init neighbours */
			field[x][y].up = NULL;
			field[x][y].down = NULL;
			field[x][y].left = NULL;
			field[x][y].right = NULL;
		}
	}

	current = field[1][0].coords;
}

/* links node structures to their neighbours (if neighbours exist */
void link_nodes() {
	int i, j;

	for (j = 0; j < YDIM; j++) {
		for (i = 0; i < XDIM; i++) {

			/* if node is not in upper row */
			if (j <= 3) field[i][j].up = &field[i][j + 1];
			else field[i][j].up = NULL;

			/* if node is not in lower row */
			if (j >= 1) field[i][j].down = &field[i][j - 1];
			else field[i][j].down = NULL;

			/* if node is not in rightmost column */
			if (i <= 3) field[i][j].right = &field[i + 1][j];
			else field[i][j].right = NULL;

			/* if node is not in leftmost column */
			if (i >= 1) field[i][j].left = &field[i - 1][j];
			else field[i][j].left = NULL;
		}
	}
}

/* prints all nodes and their coords */
void print_field() {
	int i, j;

	/* iterate over all coords from top to bottom, left to right */
	for (j = YDIM - 1; j >= 0; j--) {
		for (i = 0; i < XDIM; i++) {

			/* check is node is connected to checkpoint, other value than zero means it is */
			if (node_to_checkpoint(field[i][j]) != -1)
				printf("%3d:(%d, %d)", node_to_checkpoint(field[i][j]), field[i][j].coords.x, field[i][j].coords.y);
			/* if no checkpoint present, just print coords */
			else printf("    (%d, %d)", field[i][j].coords.x, field[i][j].coords.y);

			if (i < XDIM - 1) printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}


/* print all the neighbours of a specific node, function not necessarn.y but used for testing */
void find_neighbours(coord n) {
	printf("N:\t%s {%d}\n", field[n.x][n.y].name, field[n.x][n.y].checkpoint);

	if (field[n.x][n.y].up && field[n.x][n.y].up->checkpoint != 0)
		printf("U:\t%s {%d}\n", field[n.x][n.y].up->name,
				field[n.x][n.y].up->checkpoint);
	else if (field[n.x][n.y].up->name == 0) printf("U:\t-\n");
	else printf("U:\t%s\n", field[n.x][n.y].up->name);

	if (field[n.x][n.y].down && field[n.x][n.y].down->checkpoint != 0)
		printf("D:\t%s {%d}\n", field[n.x][n.y].down->name,
				field[n.x][n.y].down->checkpoint);
	else if (field[n.x][n.y].down->name == 0) printf("D:\t-\n");
	else printf("D:\t%s\n", field[n.x][n.y].down->name);

	if (field[n.x][n.y].right && field[n.x][n.y].right->checkpoint != 0)
		printf("R:\t%s {%d}\n", field[n.x][n.y].right->name,
				field[n.x][n.y].right->checkpoint);
	else if (field[n.x][n.y].right->name == 0) printf("R:\t-\n");
	else printf("R:\t%s\n", field[n.x][n.y].right->name);

	if (field[n.x][n.y].left && field[n.x][n.y].left->checkpoint != 0)
		printf("L:\t%s {%d}\n", field[n.x][n.y].left->name,
				field[n.x][n.y].left->checkpoint);
	else if (field[n.x][n.y].left->name == 0) printf("L:\t-\n");
	else printf("L:\t%s\n", field[n.x][n.y].left->name);
}

/* 'convert' checkpoint number to int array with coords */
coord checkpoint_to_coord(int checkpoint) {
	char name[] = "00";
	coord coords;
	strcpy(name, checkpoints[checkpoint]);

	/* string/char to int */
	coords.x = name[0] - '0';
	coords.y = name[1] - '0';

	return coords;
}

/* convert node to checkpoint */
int node_to_checkpoint(node n) {
	int i, a, b;
	for (i = 1; i <= 12; i++) {
		a = atoi(checkpoints[i]);
		b = atoi(n.name);
		if (a == b)
			return i;
	}
	return -1;
}

/* reset all Lee's marks to zero and mark all nodes as not visited */
void clear_marks() {
	int x, y;

	for (y = 0; y < YDIM; y++)
		for (x = 0; x < XDIM; x++) {
			field[x][y].mark = 0;
			field[x][y].past = 0;
		}
}

/* find the shortest route from coord 'from' to coord 'to' */
void get_route(coord from, coord to) {
	int start_cp, end_cp, j, min_mark = 0, z = 0, len;

	coord min_coords;
	coord route[40];

	route_marks(from, to);
	len = route_len(from, to);

	start_cp = node_to_checkpoint(field[from.x][from.y]);
	end_cp = node_to_checkpoint(field[to.x][to.y]);

	printf("Start\t%d: (%d, %d)\n", start_cp, from.x, from.y);
	printf("End\t%d: (%d, %d)\n", end_cp, to.x, to.y);
	printf("Length of shortest route: %d\n", route_len(from, to));
	// printf("\n");

	current = to;

	for (j = 0; j < len; j++) {
		route[j].x = -1;
		route[j].y = -1;
	}
	
	while (current.x != from.x || current.y != from.y) {
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
	}

	route[z] = current;

	for (j = len; j >= 0; j--) {
		if (route[j].x != -1) {
			printf("(%d, %d)", route[j].x, route[j].y);
			if (j) printf(" ▷ ");
			else printf("\n");
		}
	}

}

/* mark all points according to Lee */
void route_marks(coord from, coord to) {
	int finished, i, x, y;

	/* clear ecerything first */
	clear_marks();

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
}

/* print all of Lee's marks */
void print_route_marks() {
	int x, y;
	
	for (y = YDIM - 1; y >= 0; y--) {
		for (x = 0; x < XDIM; x++) {
			printf("%d", field[x][y].mark);
			if (x < XDIM - 1) printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}

/* get the lenght of a certain route without actually going */
int route_len(coord a, coord b) {
	route_marks(a, b);
	return field[b.x][b.y].mark - field[a.x][a.y].mark;
}

/* sort all destinations checkpoints for shortest total distance */
void short_sort(int *check, int num) {
	int i, j, k, tmp;

	for (i = 0; i < num; i++)
		for (j = i + 1; j < num; j++)
			for (k = j + 1; k < num; k++)
				if (route_len(checkpoint_to_coord(check[i]), checkpoint_to_coord(check[k])) < route_len(checkpoint_to_coord(check[i]), checkpoint_to_coord(check[j]))) {
					tmp = check[j];
					check[j] = check[k];
					check[k] = tmp;
				}
}

void route_sequence(int *checks, int checks_num) {
	int i;

	for (i = 0; i < checks_num - 1; i++) {
		get_route(checkpoint_to_coord(checks[i]), checkpoint_to_coord(checks[i + 1]));
		printf("\n\n");
	}
}

void read_mines(int verbose) {
	FILE *mine_f;
	mine_f = fopen("./mines.txt", "r");
	char c;
	int i = 0;
	coord a, b;

	if (mine_f == NULL) {
		if (verbose) printf("Mine file not found!\n");
		if (verbose) printf("Continuing without mines...\n");
	}
	else {
		if (verbose) printf("MINES:\n----------------\n");
		while ((c = fgetc(mine_f)) != EOF) {
			i++;
			if (c != ' ' && c != '\n') {
				switch(i) {
					case 1:
					a.x = c - '0';
					break;

					case 2:
					a.y = c - '0';
					break;

					case 4:
					b.x = c - '0';
					break;

					case 5:
					b.y = c - '0';
					break;

					default:
					printf("ERROR: i = %d", i);
				}
			}
			else if (c == '\n') {
				if (verbose) printf("(%d, %d) -- (%d, %d)\n", a.x, a.y, b.x, b.y);
				place_mine(a, b);
				i = 0;
			}
		}
		if (verbose) printf("----------------\n\n");
	}
}

void place_mine(coord a, coord b) {
	if (a.x == b.x) {
		if (a.y < b.y) {
			field[a.x][a.y].up = NULL;
			field[b.x][b.y].down = NULL;
		}
		else if (a.y > b.y) {
			field[a.x][a.y].down = NULL;
			field[b.x][b.y].up = NULL;
		}
	}
	else if (a.y == b.y) {
		if (a.x < b.x) {
			field[a.x][a.y].right = NULL;
			field[b.x][b.y].left = NULL;
		}
		else if (a.x > b.x) {
			field[a.x][a.y].left = NULL;
			field[b.x][b.y].right = NULL;
		}
	}
}
