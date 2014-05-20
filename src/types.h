/**
 * @file types.h
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Declaration of dir_t enumeration and coord_t structure.
 */

#ifndef TYPES_H
#define TYPES_H

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} dir_t;

typedef struct coord_s {
    size_t x, y;
} coord_t;

#endif /* TYPES_H */