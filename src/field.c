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
            return i + 1;
    }

    return SIZE_MAX;
}

compass_t node_get_checkpoint_direction(node_t *node)
{
    coord_t coord;

    assert(node != NULL);

    coord = node->location;

    if(coord.y == 0)
        return NORTH;
    else if(coord.y == FIELD_SIZE_HEIGHT - 1)
        return SOUTH;
    else if(coord.x == 0)
        return EAST;
    else // if(coord.x == FIELD_SIZE_WIDTH - 1)
        return WEST;
}

node_t **field_sort_checkpoints(field_t *field,
                                node_t *start,
                                node_t **destinations,
                                size_t *num_destinations)
{
    assert(field != NULL);
    assert(start != NULL);
    assert(destinations != NULL);

    destinations = (node_t **)realloc(destinations, ++*num_destinations * sizeof(node_t *));
    for(size_t i = *num_destinations - 1; i >= 1; --i) {
        destinations[i] = calloc(1, sizeof(node_t *));
        destinations[i] = destinations[i - 1];
    }

    destinations[0] = start;

    for(size_t i = 0; i < *num_destinations; ++i) {
        for(size_t j = i + 1; j < *num_destinations; ++j) {
            for(size_t k = j + 1; k < *num_destinations; ++k) {
                assert(destinations[i] != NULL || ! printf("i = %zu\n", i));
                assert(destinations[j] != NULL || ! printf("j = %zu\n", j));
                assert(destinations[k] != NULL || ! printf("k = %zu\n", k));

                if (lee_get_lenght(field, destinations[i]->location, destinations[k]->location) < lee_get_lenght(field, destinations[i]->location, destinations[j]->location)) {
                    node_t *tmp;

                    tmp = destinations[j];
                    destinations[j] = destinations[k];
                    destinations[k] = tmp;
                }
            }
        }
    }

    return destinations;
}

void field_loop_destinations(field_t *field, node_t **destinations, size_t num_destinations) {
	for (size_t i = 0; i < num_destinations - 1; ++i) {
        route_t *route;
        coord_t from, to;

        from = destinations[i]->location;
        to = destinations [i + 1]->location;

        route = route_find(field, from, to);
        route_print(route);

		if (i < num_destinations - 2) printf("\n\n");

        route_destroy(route);
	}
}
