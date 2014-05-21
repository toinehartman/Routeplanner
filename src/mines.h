/**
 * @file mines.h
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Mines implementation.
 */

#ifndef MINES_H
#define MINES_H

#include <stdint.h>

#include "field.h"

typedef struct {
    node_t *black, *yellow;
} mine_t;

typedef struct {
    mine_t **mines;
    size_t num_mines;
    char *filename;
} mineset_t;

/**
 * Create a mineset.
 *
 * @param filename The filename of the mines file.
 * @return The mineset. Must be destroyed with mineset_destroy().
 */
mineset_t *mineset_create(const char *filename);

/**
 * Destroy a mineset.
 *
 * @param mineset The mineset to destroy.
 */
void mineset_destroy(mineset_t *mineset);

/**
 * Read from the mine file and expand the mineset.
 *
 * @param mineset The mineset.
 * @param field The field.
 * @return true on succes, false when unable to open file with read access.
 */
bool mineset_read(mineset_t *mineset, field_t *field);

/**
 * Write to the mines file.
 *
 * Populate the mines file with the data in the mineset. File is
 * overwritten, not appended.
 *
 * @param mineset The mineset to read from.
 * @param filename The filename of the mines file to write to.
 * @return true on succes, false when unable to open file with write access.
 */
bool mineset_write(mineset_t *mineset, const char *filename);

/**
 * Print all the mines in the mineset.
 *
 * @param mineset The mineset.
 */
void mineset_print(mineset_t *mineset);

/**
 * Add a mine to the mineset.
 *
 * @param mineset The mineset.
 * @param black One of the connected nodes.
 * @param yellow The other node.
 * @return The added mine.
 */
mine_t *mineset_add_mine(mineset_t *mineset, node_t *black, node_t *yellow);

/**
 * Apply a mineset.
 *
 * Adding the mines to the field by disconnecting nodes around the mine.
 *
 * @param mineset The mineset.
 * @param field The field.
 */
void mineset_apply(mineset_t *mineset, field_t *field);

/**
 * Apply a mine.
 *
 * Disconnect the nodes around the mine.
 *
 * @param mine The mine to apply.
 * @param field The field.
 */
void mine_apply(mine_t *mine, field_t *field);

#endif /* MINES_H */
