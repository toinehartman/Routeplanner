/**
 * @file field.h
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Declaration of the field_t structure and related functions.
 */

#ifndef FIELD_H
#define FIELD_H

#include <stdint.h>
#include <stdbool.h>

#include "types.h"

#define FIELD_SIZE_WIDTH    5
#define FIELD_SIZE_HEIGHT   5

typedef struct field_s field_t;

/**
 * @brief A node within a field.
 */
typedef struct node_s {
    /** Owning field. */
    field_t *field;

    /** Location of the node. */
    coord_t location;

    /**
     * Neighbours of the node.
     *
     * Use dir_t as index.
     */
    struct node_s *neighbours[4];

    /** Value used by the path finding algorithm */
    size_t lee_value;

    /** Point visited or not. */
    bool lee_visited;
} node_t;

/**
 * @brief Data for a field of nodes.
 */
struct field_s {
    /**
     * Nodes of the field.
     *
     * Coordinates can be used: nodes[x][y].
     */
    node_t nodes[FIELD_SIZE_WIDTH][FIELD_SIZE_HEIGHT];

    /** To find checkpoint x, use (x + 1) as array index. */
    node_t *checkpoints[12];
};

/**
 * Create a new field.
 *
 * @return The created field, or NULL on allocation failure.
 * Must be destroyed with field_destroy().
 */
field_t *field_create();

/**
 * Destroy specified field and free its memory.
 *
 * @param field The field.
 */
void field_destroy(field_t *field);

/**
 * Print specified field.
 *
 * @param field The field to print.
 */
void field_print(field_t *field);

/**
 * Get a node.
 *
 * Asserts the validity of the coordinate in regard of the
 * size of the field.
 *
 * @param field The field.
 * @param coord The coordinate of the node.
 * @return The node.
 */
node_t *field_get_node(field_t *field, coord_t coord);

/**
 * Get the checkpoint number of the given node.
 *
 * @param field The field.
 * @param node The node.
 * @return Checkpoint number between 0-11, or SIZE_MAX if no
 * checkpoint found at specified coordinate.
 */
size_t node_get_checkpoint(node_t *node);

/**
 * Get the direction of a checkpoint.
 *
 * Find the compass direction when driving out of the specified checkpoint.
 * @param node The node to which the checkpoint is connected.
 * @return The compass direction.
 */
compass_t node_get_checkpoint_direction(node_t *node);

/**
 * Sort the destination checkpoints.
 *
 * @param field The field.
 * @param start The start node.
 * @param checkpoints The destination checkpoints.
 * @param num_checkpoints The number of destination checkpoints.
 * @return The destinations sorted for the shortest total route.
 */
node_t **field_sort_checkpoints(field_t *field,
                                node_t *start,
                                node_t **checkpoints,
                                size_t *num_checkpoints);

void field_loop_destinations(field_t *field, node_t **destinations, size_t num_destinations);

#endif /* FIELD_H */
