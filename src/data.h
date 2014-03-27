#ifndef DATA_H
#define DATA_H

typedef struct Coord {
	int x;
	int y;
} coord;

typedef struct Node {
	char name[3];
	struct Coord coords;
	int checkpoint;
	int mark;
	int past;			/* 0 means no action necessary, 1 means vertically approached, 2 means horizontally */
	struct Node *up;
	struct Node *right;
	struct Node *left;
	struct Node *down;
} node;

#endif /* DATA_H */
