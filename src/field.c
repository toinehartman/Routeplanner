/**
 * @file field.c
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Implementation of the field object.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "field.h"
#include "route.h"
#include "types.h"

field_t *field_create()
{
    field_t *field;

    field = calloc(sizeof(field_t), 1);
    if(field == NULL)
        return NULL;

#define SET_CHECKPOINT(n,x,y) field->checkpoints[n] = &field->nodes[x][y];

    SET_CHECKPOINT(0, 1, 0)
    SET_CHECKPOINT(1, 2, 0)
    SET_CHECKPOINT(2, 3, 0)
    SET_CHECKPOINT(3, 4, 1)
    SET_CHECKPOINT(4, 4, 2)
    SET_CHECKPOINT(5, 4, 3)
    SET_CHECKPOINT(6, 3, 4)
    SET_CHECKPOINT(7, 2, 4)
    SET_CHECKPOINT(8, 1, 4)
    SET_CHECKPOINT(9, 0, 3)
    SET_CHECKPOINT(10, 0, 2)
    SET_CHECKPOINT(11, 0, 1)

#undef SET_CHECKPOINT

    for(size_t y = 0; y < FIELD_SIZE_HEIGHT; ++y) {
        for(size_t x = 0; x < FIELD_SIZE_WIDTH; ++x) {
            node_t *node;

            node = &field->nodes[x][y];
            node->field = field;
            node->location = (coord_t){x, y};

            /* if node is not in upper row */
			if(y < FIELD_SIZE_HEIGHT - 1)
                node->neighbours[NORTH] = &field->nodes[x][y + 1];
			else
                node->neighbours[NORTH] = NULL;

			/* if node is not in lower row */
			if(y >= 1)
                node->neighbours[SOUTH] = &field->nodes[x][y - 1];
			else
                node->neighbours[SOUTH] = NULL;

			/* if node is not in rightmost column */
			if(x < FIELD_SIZE_WIDTH - 1)
                node->neighbours[EAST] = &field->nodes[x + 1][y];
			else
                node->neighbours[EAST] = NULL;

			/* if node is not in leftmost column */
			if(x >= 1)
                node->neighbours[WEST] = &field->nodes[x - 1][y];
			else
                node->neighbours[WEST] = NULL;
        }
    }

    return field;
}

void field_destroy(field_t *field)
{
    assert(field != NULL);

    free(field);
}

void field_print(field_t *field)
{
    assert(field != NULL);

    for(int y = FIELD_SIZE_HEIGHT - 1; y >= 0; --y) {
        for(size_t x = 0; x < FIELD_SIZE_WIDTH; ++x) {
            size_t checkpoint;
            node_t *node;

            node = &field->nodes[x][y];
            checkpoint = node_get_checkpoint(node);

            if(checkpoint != SIZE_MAX)
                printf("%3zu:(%zu, %zu)(%2zu)", checkpoint + 1, node->location.x, node->location.y, node->lee_value);
            else
                printf("    (%zu, %zu)(%2zu)", node->location.x, node->location.y, node->lee_value);

            printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

node_t *field_get_node(field_t *field, coord_t coord)
{
    assert(field != NULL);
    assert(coord.x >= 0 && coord.x < FIELD_SIZE_WIDTH);
    assert(coord.y >= 0 && coord.y < FIELD_SIZE_HEIGHT);

    return &(field->nodes[coord.x][coord.y]);
}

size_t node_get_checkpoint(node_t *node)
{
    assert(node != NULL);

    for(size_t i = 0; i < 12; ++i) {
        if(node->field->checkpoints[i] == node)
            return i;
    }

    return SIZE_MAX;
}
