/**
 * @file main.c
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Application entry point.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>

#include "macros.h"
#include "field.h"
#include "route.h"
#include "mines.h"

node_t **parse_arguments(field_t *field, int argc, char *argv[], size_t *len);
void print_destinations(field_t *field, node_t **destinations, size_t *num_destinations);
void print_checkpoints(field_t *field);

int main(int argc, char *argv[])
{
    field_t *field;
//    route_t *route;
    mineset_t *mineset;
    node_t **destinations;
    size_t num_destinations = 0;
    const char *mine_file = "./mines.txt";

    mine_t *mine;

    field = field_create();
    mineset = mineset_create(mine_file);

    if(mineset_read(mineset, field) == false)
        printf("Mines couldn't be read from file.\n");
    mineset_apply(mineset, field);

    destinations = parse_arguments(field, argc, argv, &num_destinations);
    print_destinations(field, destinations, &num_destinations);

    destinations = field_sort_checkpoints(field,
                                          field_get_node(field, (coord_t){1, 0}),
                                          destinations,
                                          &num_destinations);
    print_destinations(field, destinations, &num_destinations);

//    print_checkpoints(field);

    assert(destinations != NULL);
    assert(destinations[0] != NULL);

    mine = mineset_add_mine(mineset,
                     field_get_node(field, (coord_t){1, 1}),
                     field_get_node(field, (coord_t){1, 2}));
    mine_apply(mine, field);
    mineset_print(mineset);

//    route = route_find(field, (coord_t){1, 0}, (coord_t){4, 3});
    field_print(field);
//    route_print(route);

    mineset_write(mineset, mine_file);

    field_loop_destinations(field, destinations, num_destinations);

//    route_destroy(route);
    mineset_destroy(mineset);
    field_destroy(field);

    return 0;
}

node_t **parse_arguments(field_t *field, int argc, char *argv[], size_t *len)
{
    node_t **checkpoints;

    checkpoints = calloc(sizeof(node_t *), argc - 1);
    *len = argc - 1;

    for(size_t i = 1; i < argc; ++i) {
        int cp;

        cp = atoi(argv[i]) - 1;
        checkpoints[i - 1] = field->checkpoints[cp];
    }

    return checkpoints;
}

void print_destinations(field_t *field, node_t **destinations, size_t *num_destinations) {
    assert(field != NULL);
    assert(destinations != NULL);

    for(size_t i = 0; i < *num_destinations; ++i)
        printf("%zu ", node_get_checkpoint(destinations[i]));
    printf("\n");
}

void print_checkpoints(field_t *field) {
    for(size_t x = 0; x < FIELD_SIZE_WIDTH; ++x) {
        for(size_t y = 0; y < FIELD_SIZE_HEIGHT; ++y) {
            node_t *node;
            size_t cp;

            node = field_get_node(field, (coord_t){x, y});
            cp = node_get_checkpoint(node);

            if(cp != SIZE_MAX)
                printf("(%zu, %zu): %zu\n", node->location.x, node->location.y, cp);
        }
    }
}