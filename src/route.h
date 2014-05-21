/**
 * @file route.h
 * @author Toine Hartman <tjbhartman@gmail.com
 *
 *
 */

#ifndef ROUTE_H
#define ROUTE_H

#include <stdint.h>

#include "field.h"

/**
 * Route structure
 */
typedef struct {
    /** Route steps from start to end. */
    node_t **steps;

    /** Number of steps. */
    size_t num_steps;

    /** Node where the route starts. */
    node_t *start;

    /** Destination node. */
    node_t *destination;
} route_t;

/**
 * Create a route_t and allocate memory for it.
 *
 * @return The created route, or NULL on allocation failure.
 * Must be destroyed with route_destroy().
 */
route_t *route_create();

/**
 * Destroy specified route_t and free the allocated memory.
 *
 * @param route The route.
 */
void route_destroy(route_t *route);

/**
 * Find a shortest route.
 *
 * @param field The field.
 * @param start The start coordinate.
 * @param destination The destination coordinate.
 * @return A shortest route. Must be destroyed with route_destroy().
 */
route_t *route_find(field_t *field,
                    coord_t start,
                    coord_t destination);

/**
 * Prints the route.
 *
 * @param route The route to print.
 */
void route_print(route_t *route);

/**
 *
 */
drive_t node_drive_direction(compass_t initial_dir, compass_t new_dir);

/**
 *
 */
compass_t node_compass_direction(node_t *current, node_t *next);

/**
 *
 */
char *print_drive_direction(drive_t drive_dir);

#endif /* ROUTE_H */
