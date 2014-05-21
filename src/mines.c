/**
 * @file mines.c
 * @author Toine Hartman <tjbhartman@gmail.com>
 *
 * Mines implementation.
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "mines.h"
#include "field.h"
#include "types.h"

mineset_t *mineset_create(const char *filename)
{
    mineset_t *mineset;

    if(filename == NULL)
        return NULL;

    mineset = calloc(sizeof(mineset_t), 1);
    if(mineset == NULL)
        return NULL;

    mineset->mines = NULL;
    mineset->num_mines = 0;
    mineset->filename = strndup(filename, 255);

    return mineset;
}

void mineset_destroy(mineset_t *mineset)
{
    free(mineset->filename);

    for(size_t i = 0; i < mineset->num_mines; ++i)
        free(mineset->mines[i]);

    free(mineset->mines);
    free(mineset);
}

bool mineset_read(mineset_t *mineset, field_t *field)
{
    FILE *mine_file;
    coord_t black, yellow;

    mine_file = fopen(mineset->filename, "r");
    if(mine_file == NULL)
        return false;

    while(fscanf(mine_file, "%1zu %1zu %1zu %1zu",
                 &black.x, &black.y, &yellow.x, &yellow.y) != EOF) {
        mineset_add_mine(mineset,
                         field_get_node(field, black),
                         field_get_node(field, yellow));
    }

    fclose(mine_file);

    return true;
}

bool mineset_write(mineset_t *mineset, const char *filename)
{
    FILE *mine_file;

    mine_file = fopen(filename, "w");
    if(mine_file == NULL)
        return false;

    for(size_t i = 0; i < mineset->num_mines; ++i) {
        mine_t *mine;
        mine = mineset->mines[i];

        fprintf(mine_file, "%zu %zu %zu %zu\n",
                mine->black->location.x,
                mine->black->location.y,
                mine->yellow->location.x,
                mine->yellow->location.y);
    }

    return true;
}

void mineset_print(mineset_t *mineset)
{
    for(size_t i = 0; i < mineset->num_mines; ++i) {
        mine_t *mine;

        mine = mineset->mines[i];

        printf("Mine: (%zu, %zu) -- (%zu, %zu)\n",
               mine->black->location.x,
               mine->black->location.y,
               mine->yellow->location.x,
               mine->yellow->location.y);
    }
    printf("\n");
}

mine_t *mineset_add_mine(mineset_t *mineset, node_t *black, node_t *yellow)
{
    mine_t *mine;

    assert(black != NULL);
    assert(yellow != NULL);
    assert(mineset != NULL);

    mine = calloc(sizeof(mine_t), 1);
    assert(mine != NULL);

    mine->black = black;
    mine->yellow = yellow;

    for(size_t i = 0; i < mineset->num_mines; ++i) {
        mine_t *existing_mine;

        existing_mine = mineset->mines[i];
        if((existing_mine->black == black && existing_mine->yellow == yellow)
           || (existing_mine->black == yellow && existing_mine->yellow == black)) {
               free(mine);
               return existing_mine;
           } // Mine already exists in mineset, don't add it.
    }


    ++mineset->num_mines;
    mineset->mines = realloc(mineset->mines, mineset->num_mines * sizeof(mine_t *));
    mineset->mines[mineset->num_mines - 1] = mine;

    return mine;
}

void mineset_apply(mineset_t *mineset, field_t *field)
{
    assert(mineset != NULL);
    assert(field != NULL);

    for(size_t i = 0; i < mineset->num_mines; ++i) {
        mine_apply(mineset->mines[i], field);
    }
}

void mine_apply(mine_t *mine, field_t *field)
{
    coord_t black, yellow;

    assert(mine != NULL);
    assert(field != NULL);

    black = mine->black->location;
    yellow = mine->yellow->location;

    if (black.x == yellow.x) {
		if (black.y < yellow.y) {
			mine->black->neighbours[NORTH] = NULL;
            mine->yellow->neighbours[SOUTH] = NULL;
		} else if (black.y > yellow.y) {
            mine->black->neighbours[SOUTH] = NULL;
            mine->yellow->neighbours[NORTH] = NULL;
		}
	} else if (black.y == yellow.y) {
		if (black.x < yellow.x) {
            mine->black->neighbours[EAST] = NULL;
            mine->yellow->neighbours[WEST] = NULL;
		} else if (black.x > yellow.x) {
            mine->black->neighbours[WEST] = NULL;
            mine->yellow->neighbours[EAST] = NULL;
		}
	}
}
