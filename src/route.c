/**
 * @file route.c
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Implementation of the route object.
 */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "route.h"
#include "field.h"
#include "types.h"

route_t *route_create()
{
    route_t *route;

    route = calloc(sizeof(route_t), 1);
    if(route == NULL)
        return NULL;

    return route;
}

void route_destroy(route_t *route)
{
    assert(route != NULL);

    free(route->steps);
    free(route);
}

void lee_mark_cells(field_t *field, coord_t start, coord_t destination)
{
    size_t lee_value;
    bool finished;

    assert(field != NULL);

    // Erase all Lee values
    for(size_t y = 0; y < FIELD_SIZE_HEIGHT; ++y) {
        for(size_t x = 0; x < FIELD_SIZE_WIDTH; ++x) {
            node_t *node;

            node = field_get_node(field, (coord_t){x, y});
            node->lee_value = 0;
            node->lee_visited = false;
        }
    }

    // Fill the field with Lee values
    lee_value = 1;
    field_get_node(field, start)->lee_value = lee_value;
    finished = false;

    while(!finished) {
        finished = true;
        for(size_t y = 0; y < FIELD_SIZE_HEIGHT; ++y) {
            for(size_t x = 0; x < FIELD_SIZE_WIDTH; ++x) {
                node_t *node;

                node = field_get_node(field, (coord_t){x, y});

                if (node->lee_value == lee_value && !node->lee_visited) {
                    node_t **neighbours;

                    finished = false;
                    neighbours = node->neighbours;

                    if(neighbours[NORTH]
                       && (neighbours[NORTH]->lee_value == 0
                           || neighbours[NORTH]->lee_value > lee_value))
                        neighbours[NORTH]->lee_value = lee_value + 1;

                    if(neighbours[SOUTH]
                       && (neighbours[SOUTH]->lee_value == 0
                           || neighbours[SOUTH]->lee_value > lee_value))
                        neighbours[SOUTH]->lee_value = lee_value + 1;

                    if(neighbours[EAST]
                       && (neighbours[EAST]->lee_value == 0
                           || neighbours[EAST]->lee_value > lee_value))
                        neighbours[EAST]->lee_value = lee_value + 1;

                    if(neighbours[WEST]
                       && (neighbours[WEST]->lee_value == 0
                           || neighbours[WEST]->lee_value > lee_value))
                        neighbours[WEST]->lee_value = lee_value + 1;

                    node->lee_visited = true;
                }
            }
        }
        ++lee_value;
    }
}

void lee_backtrack(field_t *field,
                   coord_t start,
                   coord_t destination,
                   route_t *route)
{
    node_t **steps, *current_node;
    size_t num_steps = 1;

    assert(field != NULL);
    assert(route != NULL);

    steps = calloc(sizeof(node_t *), num_steps);
    assert(steps != NULL);

    route->start = field_get_node(field, start);
    route->destination = field_get_node(field, destination);

    current_node = route->destination;

    while(current_node != route->start) {
        size_t min_dist = SIZE_MAX;
        node_t *min_node = NULL;

        steps[num_steps - 1] = current_node;

        for(compass_t dir = NORTH; dir <= WEST; ++dir) {
            node_t *node;

            node = current_node->neighbours[dir];

            if(node && min_dist > node->lee_value) {
                min_dist = node->lee_value;
                min_node = node;
            }
        }

        if(min_node == NULL) {
            assert(!"Can't find route!");
        }

        current_node = min_node;

        ++num_steps;
        steps = realloc(steps, num_steps * sizeof(node_t *));
        assert(steps != NULL);
    }

    steps[num_steps - 1] = current_node;

    route->num_steps = num_steps;
    route->steps = steps;
}

size_t lee_get_lenght(field_t *field, coord_t start, coord_t destination)
{
    assert(field != NULL);

    lee_mark_cells(field, start, destination);

    return field_get_node(field, destination)->lee_value - field_get_node(field, start)->lee_value;
}

route_t *route_find(field_t *field,
                    coord_t start,
                    coord_t destination)
{
    route_t *route;

    assert(field != NULL);

    route = route_create();

    // Mark all cells
    lee_mark_cells(field, start, destination);

    // Backtract over the lee values
    lee_backtrack(field, start, destination, route);

    return route;
}

void route_print(route_t *route)
{
    compass_t previous_dir;
    node_t *node = NULL, *next = NULL;


    assert(route != NULL);
    assert(route->num_steps >= 1);

    previous_dir = node_get_checkpoint_direction(route->steps[route->num_steps - 1]);

    printf("<route>{start: (%zu, %zu), destination: (%zu, %zu)}[\n",
           route->start->location.x,
           route->start->location.y,
           route->destination->location.x,
           route->destination->location.y);

    for(long i = route->num_steps - 1; i > 0; --i) {
        compass_t new_dir;

        node = route->steps[i];
        next = route->steps[i - 1];
        new_dir = node_compass_direction(node, next);

        printf("  (%zu, %zu)[%s]\n", node->location.x, node->location.y, print_drive_direction(node_drive_direction(previous_dir, new_dir)));

        previous_dir = new_dir;
    }
    node = next;

    printf("  (%zu, %zu)[%s]\n",
           node->location.x,
           node->location.y,
           print_drive_direction(node_drive_direction(previous_dir,
                                                      (node_get_checkpoint_direction(node) + 2) % 4)));
    printf("]\n");
}

drive_t node_drive_direction(compass_t initial_dir, compass_t new_dir)
{
    if(initial_dir == new_dir)
        return FORWARD;
    else if(initial_dir % 2 == new_dir % 2)
        return BACKWARD;
    else if((initial_dir - new_dir) % 4 == 3 || (initial_dir - new_dir) % 4 == -1)
        return RIGHT;
    else // if((initial_dir - new_dir) % 4 == 1 || (initial_dir - new_dir) % 4 == -3)
        return LEFT;
}

compass_t node_compass_direction(node_t *current, node_t *next)
{
    assert(current != NULL);
    assert(next != NULL);
    
    if(current->location.x == next->location.x)
        return current->location.y < next->location.y ? NORTH : SOUTH;
    else // if(current->location.y == next->location.y)
        return current->location.x < next->location.x ? EAST : WEST;
}

char *print_drive_direction(drive_t drive_dir)
{
    switch(drive_dir) {
        case FORWARD: return "FORWARD";
        case BACKWARD: return "BACKWARD";
        case RIGHT: return "RIGHT";
        case LEFT: return "LEFT";
        default: return "UNKNOWN";
    }
}