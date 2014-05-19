/**
 @file data.h
 @brief Header file to describe necessary data structures.
 */

#ifndef DATA_H
#define DATA_H

/**
 @struct Coord
 @brief A coordinate struct.

 A structure to represent the coordinates of a point.
 */
typedef struct Coord {
	int x; /**< The x-coordinate of the point. */
	int y; /**< The y-coordinate of the point. */
} coord;

/**
 @struct Node
 @brief Structure representing a node.
 
 A structure representing a node (junction) on the map.
 */
typedef struct Node {
	char name[3]; /**< The name of the node in the form "<x><y>", e.g. "03". */
	coord coords; /**< A coord to represent the coordinates of the node. */
	int checkpoint; /**< The number of the checkpoint the node is connected to. Is -1 when not connected. */
	int mark; /**< Mark used by Lee algorithm. */
	int past; /**< Used by Lee algorithm: 0 means not yet visited, 1 means already visited. */
	struct Node *north; /**< Pointer to the northern neighbour node. */
    struct Node *south; /**< Pointer to the southern neighbour node. */
    struct Node *east; /**< Pointer to the eastern neighbour node. */
    struct Node *west; /**< Pointer to the western neighbour node. */
} node;

#endif /* DATA_H */
