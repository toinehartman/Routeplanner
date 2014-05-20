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

int main(int argc, char *argv[]) {
    field_t *field;
    route_t *route;

    field = field_create();

    route = route_find(field, (coord_t){1, 0}, (coord_t){4, 3});
    field_print(field);
    route_print(route);

    route_destroy(route);
    field_destroy(field);

    return 0;
}