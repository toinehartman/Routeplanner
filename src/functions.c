/*
 * field.h
 *
 *  Created on: Feb 19, 2014
 *      Author: Toine
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "data.h"
#include "functions.h"

node field[XDIM][YDIM]; /* initialize the field */
char *checkpoints[] = { "00", "10", "20", "30", "41", "42", "43", "34", "24", "14", "03", "02", "01" }; /* item i in this array is */

static int roadValue = 1;		/* time-value of driving the length of a road */
static int cornerValue = 2;	/* time value of making a 90-degree turn */


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
}

/* links node structures to their neighbours (if neighbours exist */
void link_nodes() {
	int i, j;

	for (j = 0; j < YDIM; j++) {
		for (i = 0; i < XDIM; i++) {

			/* if node is not in upper row */
			if (j <= 3)
				field[i][j].up = &field[i][j + 1];
			else
				field[i][j].up = NULL;

			/* if node is not in lower row */
			if (j >= 1)
				field[i][j].down = &field[i][j - 1];
			else
				field[i][j].down = NULL;

			/* if node is not in rightmost column */
			if (i <= 3)
				field[i][j].right = &field[i + 1][j];
			else
				field[i][j].right = NULL;

			/* if node is not in leftmost column */
			if (i >= 1)
				field[i][j].left = &field[i - 1][j];
			else
				field[i][j].left = NULL;
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
			if (node_to_checkpoint(field[i][j]) != 0)
				printf("%3d:(%d, %d)", node_to_checkpoint(field[i][j]), field[i][j].coords.x, field[i][j].coords.y);
			/* if no checkpoint present, just print coords */
			else printf("    (%d, %d)", field[i][j].coords.x, field[i][j].coords.y);

			if (i < XDIM - 1) printf(" ");
		}
		printf("\n");
	}
	printf("\n");
}


/* print all the neighbours of a specific node, function not necessary but used for testing */
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

/* 'convert' checkpoint number to int array with coords */
void checkpoint_to_coord(int checkpoint, int coords[]) {
	char name[] = "00";
	strcpy(name, checkpoints[checkpoint]);

	/* string to int */
	coords[0] = name[0] - '0';
	coords[1] = name[1] - '0';
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
	return 0;
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

	clear_marks();
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

	for (y = YDIM - 1; y >= 0; y--) {
		for (x = 0; x < XDIM; x++) {
			printf("%d", field[x][y].mark);
			if (x < XDIM - 1)
				printf(" ");
		}
		printf("\n");
	}
	printf("\n");
	

	current = to;

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
