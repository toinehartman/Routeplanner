/**
 * @file main.c
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Application entry point.
 */

#include <stdio.h>

#include "macros.h"
#include "field.h"
#include "route.h"
#include "mines.h"

int main(int argc, char *argv[]) {
    field_t *field;
    route_t *route;
    mineset_t *mineset;
    const char *mine_file = "./mines.txt";

    field = field_create();
    mineset = mineset_create(mine_file);

    if(mineset_read(mineset, field) == false)
        printf("mineset_read == false\n");
    mineset_apply(mineset, field);
    mineset_print(mineset);

//    mineset_add_mine(mineset,
//                     field_get_node(field, (coord_t){1, 1}),
//                     field_get_node(field, (coord_t){1, 2}));

    route = route_find(field, (coord_t){1, 0}, (coord_t){4, 3});
    field_print(field);
    route_print(route);

    mineset_write(mineset, mine_file);

    route_destroy(route);
    mineset_destroy(mineset);
    field_destroy(field);

    return 0;
}