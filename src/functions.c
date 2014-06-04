/**
 @file functions.c The functions file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Windows.h>
#include <stdbool.h>
#include <assert.h>

#include "zigbee.h"
#include "main.h"
#include "data.h"
#include "functions.h"

node field[XDIM][YDIM]; /* initialize the field */
char *checkpoints[] = { "00", "10", "20", "30", "41", "42", "43", "34", "24", "14", "03", "02", "01" }; /* item i in this array is */
char mine_file[] = "./mines.txt";

static coord current;
static coord route[40];
static int z = 0, curr_route_len;

void init_field()
{
    int x, y;

    for (y = 0; y < YDIM; y++)
    {
        for (x = 0; x < XDIM; x++)
        {

            /* set name according to coord */
            snprintf(field[x][y].name, 3, "%d%d", x, y);

            /* set coords */
            field[x][y].coords = (coord)
            {
                x ,y
            };

            /* assign checkpoint numbers */
            field[x][y].checkpoint = node_to_checkpoint(field[x][y]);

            /* init neighbours */
            field[x][y].north = NULL;
            field[x][y].south = NULL;
            field[x][y].west = NULL;
            field[x][y].east = NULL;
        }
    }

    current = field[1][0].coords;
}

void link_nodes()
{
    int i, j;

    for (j = 0; j < YDIM; j++)
    {
        for (i = 0; i < XDIM; i++)
        {

            /* if node is not in upper row */
            if (j <= 3)
                field[i][j].north = &field[i][j + 1];
            else
                field[i][j].north = NULL;

            /* if node is not in lower row */
            if (j >= 1)
                field[i][j].south = &field[i][j - 1];
            else
                field[i][j].south = NULL;

            /* if node is not in rightmost column */
            if (i <= 3)
                field[i][j].east = &field[i + 1][j];
            else
                field[i][j].east = NULL;

            /* if node is not in leftmost column */
            if (i >= 1)
                field[i][j].west = &field[i - 1][j];
            else
                field[i][j].west = NULL;
        }
    }
}

void print_field()
{
    int i, j;

    for (j = YDIM - 1; j >= 0; j--)
    {
        for (i = 0; i < XDIM; i++)
        {

            /* check is node is connected to checkpoint, other value than -1 means it is */
            if (node_to_checkpoint(field[i][j]) != -1)
                printf("%3d:(%d, %d)", node_to_checkpoint(field[i][j]), field[i][j].coords.x, field[i][j].coords.y);
            /* if no checkpoint present, just print coords */
            else printf("    (%d, %d)", field[i][j].coords.x, field[i][j].coords.y);

            if (i < XDIM - 1) printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

void find_neighbours(coord n)
{
    printf("N:\t%s {%d}\n", field[n.x][n.y].name, field[n.x][n.y].checkpoint);

    if (field[n.x][n.y].north && field[n.x][n.y].north->checkpoint != 0)
        printf("U:\t%s {%d}\n", field[n.x][n.y].north->name,
               field[n.x][n.y].north->checkpoint);
    else if (field[n.x][n.y].north->name == 0) printf("U:\t-\n");
    else printf("U:\t%s\n", field[n.x][n.y].north->name);

    if (field[n.x][n.y].south && field[n.x][n.y].south->checkpoint != 0)
        printf("D:\t%s {%d}\n", field[n.x][n.y].south->name,
               field[n.x][n.y].south->checkpoint);
    else if (field[n.x][n.y].south->name == 0) printf("D:\t-\n");
    else printf("D:\t%s\n", field[n.x][n.y].south->name);

    if (field[n.x][n.y].east && field[n.x][n.y].east->checkpoint != 0)
        printf("R:\t%s {%d}\n", field[n.x][n.y].east->name,
               field[n.x][n.y].east->checkpoint);
    else if (field[n.x][n.y].east->name == 0) printf("R:\t-\n");
    else printf("R:\t%s\n", field[n.x][n.y].east->name);

    if (field[n.x][n.y].west && field[n.x][n.y].west->checkpoint != 0)
        printf("L:\t%s {%d}\n", field[n.x][n.y].west->name,
               field[n.x][n.y].west->checkpoint);
    else if (field[n.x][n.y].west->name == 0) printf("L:\t-\n");
    else printf("L:\t%s\n", field[n.x][n.y].west->name);
}

coord checkpoint_to_coord(int checkpoint)
{
    char name[] = "00";
    coord coords;

    if (checkpoint > 12 && checkpoint < 1)
        return (coord)
    {
        -1, -1
    };

    strcpy(name, checkpoints[checkpoint]);

    /* string/char to int */
    coords.x = name[0] - '0';
    coords.y = name[1] - '0';

    return coords;
}

int node_to_checkpoint(node n)
{
    int i, a, b;
    for (i = 1; i <= 12; i++)
    {
        a = atoi(checkpoints[i]);
        b = atoi(n.name);
        if (a == b)
            return i;
    }
    return -1;
}

void clear_marks()
{
    int x, y;

    for (y = 0; y < YDIM; y++)
        for (x = 0; x < XDIM; x++)
        {
            field[x][y].mark = 0;
            field[x][y].past = 0;
        }
}

void find_shortest_route(coord from, coord to)
{
    int start_cp, end_cp, j, min_mark = 0, changed_neighbours = 0;
    coord min_coords;

    current = to;
    z = 0;

    route_marks(from, to);
    curr_route_len = route_len(from, to);

    start_cp = node_to_checkpoint(field[from.x][from.y]);
    end_cp = node_to_checkpoint(field[to.x][to.y]);

    printf("Start\t%d: (%d, %d)\n", start_cp, from.x, from.y);
    printf("End\t\t%d: (%d, %d)\n", end_cp, to.x, to.y);
    printf("Length of shortest route: %d\n\n", curr_route_len);

    for (j = 0; j < curr_route_len; j++)
    {
        route[j] = (coord)
        {
            -1, -1
        };
    }

    while (current.x != from.x || current.y != from.y)
    {
        route[z] = current;
        z++;

        node *neighbours[] = {	field[current.x][current.y].north,
            field[current.x][current.y].south,
            field[current.x][current.y].east,
            field[current.x][current.y].west
        };

        for (j = 0; j <= 3; j++)
        {
            if (neighbours[j] && (!min_mark || neighbours[j]->mark < min_mark))
            {
                min_mark = neighbours[j]->mark;
                min_coords = neighbours[j]->coords;

                ++changed_neighbours;
            }
        }
        if (changed_neighbours == 0 || (z > 1 && current.x == to.x && current.y == to.y))
        {
            printf("Can't find route.\n");
            exit(1);
        }
        current = min_coords;
    }
}

void iterate_route(HANDLE hSerial, char *byteBuffer, int init_dir, coord to)
{
    int j;
    int first = -1, prev_dir, next_dir;
    int end_cp;

    char ret_value;

    route[z] = current;

    for (j = curr_route_len; j > 0; j--)
    {
        if (route[j].x != -1 && route[j].y != -1)
        {
            if (first == -1) first = j;

            if (j == first)
            {
                prev_dir = init_dir;
                next_dir = drive_direction(prev_dir, route[j], route[j - 1]);

                //                printf("starting prev_dir: %c\n", compass_int(prev_dir));
                //                printf("starting curr_dir: %c\n", (char)next_dir);

                ret_value = zigbee_write(hSerial, byteBuffer, next_dir, START);

                printf("(%d, %d) -> (%d, %d)[%c]", route[j].x, route[j].y, route[j - 1].x, route[j - 1].y, next_dir);
                if (VERBOSE) printf("(j: %d)[prev: %c][curr: %c]", j, compass_int(init_dir), compass_int(compass_direction(route[j], route[j - 1])));
                printf("\nRET_VALUE: %c\n", ret_value);


            }
            else if (j <= curr_route_len - 1)
            {
                prev_dir = compass_direction(route[j + 1], route[j]);
                next_dir = drive_direction(prev_dir, route[j], route[j - 1]);

                //                printf("starting prev_dir: %c\n", compass_int(prev_dir));
                //                printf("starting next_dir: %c\n", (char)next_dir);

                ret_value = zigbee_write(hSerial, byteBuffer, next_dir, REAL_C);


                printf("(%d, %d) -> (%d, %d)[%c]", route[j].x, route[j].y, route[j - 1].x, route[j - 1].y, next_dir);
                if (VERBOSE) printf("(j: %d)[prev: %c][curr: %c]", j, compass_int(prev_dir), compass_int(compass_direction(route[j], route[j - 1])));
                printf("\nRET_VALUE: %c\n", ret_value);


            }
            else   /* vreemd, kan eigenlijk niet gebeuren */
            {
                fputs("[ERROR] in route-array!", stderr);
                fprintf(stderr, "(j: %d)", j);
                fputs("\n", stderr);
                exit(EXIT_FAILURE);
            }
        } else
            fprintf(stderr, "[ERROR] route[%d].x = %d, route[%d].y = %d\n", j, route[j].x, j , route[j].y);

        printf("MINE? (y/n):\n");

        ret_value = zigbee_write(hSerial, byteBuffer, 'V', FAKE_C);

        if (ret_value == 'M')
        {
            save_mine_to_file(route[j], route[j - 1]);
            add_mine_to_field(route[j], route[j - 1]);
            read_mines();

            printf("Mine saved, calculating new route...\n\n");
            find_shortest_route(route[j], to);
            iterate_route(hSerial, byteBuffer, (init_dir + 2) % 4, to);
            return;
        }
        else if (ret_value != 'C')
            fprintf(stderr, "Answer: %c\n", ret_value);

        //        printf("prev_dir: %c\n", compass_int(prev_dir));
        //        printf("next_dir: %c\n", next_dir);
    }

    prev_dir = compass_direction(route[j + 1], route[j]);
    end_cp = node_to_checkpoint(field[route[j].x][route[j].y]);

    next_dir = drive_to_cp_direction(prev_dir, (cp_direction(end_cp) + 2) % 4);


    // At checkpoint
    ret_value = zigbee_write(hSerial, byteBuffer, next_dir, CHECKPOINT);
    printf("(%d, %d) -> (%d)  [%c]\nRET_VALUE: %c\n", route[j].x, route[j].y, end_cp, next_dir, ret_value);
}

void route_marks(coord from, coord to)
{
    int finished, i, x, y;

    /* clear everything first */
    clear_marks();

    i = 1;
    field[from.x][from.y].mark = i;
    finished = 0;

    while (!finished)
    {
        finished = 1;
        for (y = 0; y < YDIM; y++)
            for (x = 0; x < XDIM; x++)
                if (field[x][y].mark == i && !field[x][y].past)
                {
                    finished = 0;
                    if (field[x][y].north && !field[x][y].north->mark)
                        field[x][y].north->mark = i + 1;
                    if (field[x][y].south && !field[x][y].south->mark)
                        field[x][y].south->mark = i + 1;
                    if (field[x][y].east && !field[x][y].east->mark)
                        field[x][y].east->mark = i + 1;
                    if (field[x][y].west && !field[x][y].west->mark)
                        field[x][y].west->mark = i + 1;
                    field[x][y].past = 1;
                }
        i++;
    }
}

void print_route_marks()
{
    int x, y;

    for (y = YDIM - 1; y >= 0; y--)
    {
        for (x = 0; x < XDIM; x++)
        {
            printf("%d", field[x][y].mark);
            if (x < XDIM - 1) printf(" ");
        }
        printf("\n");
    }
    printf("\n");
}

/* get the lenght of a certain route without actually going */
int route_len(coord a, coord b)
{
    route_marks(a, b);
    return field[b.x][b.y].mark - field[a.x][a.y].mark;
}

void short_sort(int *check, int num)
{
    int i, j, k, tmp;

    for (i = 0; i < num; i++)
        for (j = i + 1; j < num; j++)
            for (k = j + 1; k < num; k++)
                if (route_len(checkpoint_to_coord(check[i]), checkpoint_to_coord(check[k])) < route_len(checkpoint_to_coord(check[i]), checkpoint_to_coord(check[j])))
                {
                    tmp = check[j];
                    check[j] = check[k];
                    check[k] = tmp;
                }
}

void route_sequence(HANDLE hSerial, char *byteBuffer, int *checks, int checks_num)
{
    int i;

    for (i = 0; i < checks_num - 1; i++)
    {
        coord to = checkpoint_to_coord(checks[i + 1]);
        find_shortest_route(checkpoint_to_coord(checks[i]), to);
        iterate_route(hSerial, byteBuffer, cp_direction(checks[i]), checkpoint_to_coord(checks[i + 1]));
        if (i < checks_num - 2) printf("\n\n");
    }
}

int read_mines()
{
    FILE *mine_f;
    mine_f = fopen(mine_file, "r");
    char c;
    int i = 0;
    coord a, b;

    if (mine_f == NULL)
    {
        if (VERBOSE) printf("Mine file not found!\nContinuing without known mines...\n");
        return 1;
    }
    else
    {
        if (VERBOSE) printf("MINES:\n----------------\n");
        while ((c = fgetc(mine_f)) != EOF)
        {
            i++;
            if (c != ' ' && c != '\n')
            {
                switch(i)
                {
                    case 1:
                        a.x = c - '0';
                        break;
                    case 2:
                        a.y = c - '0';
                        break;
                    case 4:
                        b.x = c - '0';
                        break;
                    case 5:
                        b.y = c - '0';
                        break;
                    default:
                        printf("ERROR: i = %d", i);
                }
            }
            else if (c == '\n')
            {
                if (VERBOSE) printf("(%d, %d) -- (%d, %d)\n", a.x, a.y, b.x, b.y);
                add_mine_to_field(a, b);
                i = 0;
            }
        }
        if (VERBOSE) printf("----------------\n\n");
    }
    fclose(mine_f);
    return 0;
}

void save_mine_to_file(coord a, coord b)
{
    FILE *mine_f;
    mine_f = fopen(mine_file, "a");

    fprintf(mine_f, "%d%d %d%d\n", a.x, a.y, b.x, b.y);
    fclose(mine_f);
}

void add_mine_to_field(coord a, coord b)
{
    if (a.x == b.x)
    {
        if (a.y < b.y)
        {
            field[a.x][a.y].north = NULL;
            field[b.x][b.y].south = NULL;
        }
        else if (a.y > b.y)
        {
            field[a.x][a.y].south = NULL;
            field[b.x][b.y].north = NULL;
        }
    }
    else if (a.y == b.y)
    {
        if (a.x < b.x)
        {
            field[a.x][a.y].east = NULL;
            field[b.x][b.y].west = NULL;
        }
        else if (a.x > b.x)
        {
            field[a.x][a.y].west = NULL;
            field[b.x][b.y].east = NULL;
        }
    }
}

char drive_direction(int init_dir, coord now, coord to)
{
    int new_dir = compass_direction(now, to);
    return drive_to_cp_direction(init_dir, new_dir);
}

char drive_to_cp_direction(int prev_dir, int new_dir)
{
    /* 	's': straight
     'b': back
     'r': right
     'l': left */

    if (prev_dir == new_dir)
        return 'V';
    else if (prev_dir % 2 == new_dir % 2)
        return 'O';
    else if ((prev_dir - new_dir) % 4 == 3 || (prev_dir - new_dir) % 4 == -1) return 'R';
    else if ((prev_dir - new_dir) % 4 == 1 || (prev_dir - new_dir) % 4 == -3) return 'L';
    else
    {
        if (VERBOSE) printf("prev: %d, new: %d\n", prev_dir, new_dir);
        if (VERBOSE) printf("(prev_dir - new_dir) %% 4 = %d\n", (prev_dir - new_dir) % 4);
        return 'X';
    }
}

int compass_direction(coord from, coord to)
{
    /* returns '0' (north), '1' (east), '2' (south), '3' (west) */
    /* if this returns -1, something went terribly wrong */

    if (from.x == to.x)
        return from.y < to.y ? 0 : 2;
    else if (from.y == to.y)
        return from.x < to.x ? 1 : 3;
    return -1;
}

char compass_int(int comp)
{
    switch(comp)
    {
        case 0:
            return 'N';

        case 1:
            return 'E';

        case 2:
            return 'S';

        case 3:
            return 'W';

        default:
            return '!';
    }
}

int cp_direction(int cp)
{
    if (cp < 4) return 0;
    else if (cp < 7) return 3;
    else if (cp < 10) return 2;
    else if (cp < 13) return 1;
    return -1;
}

char zigbee_write(HANDLE hSerial, char *byteBuffer, char command, state route_state)
{
    char previous = '1';
    bool send_next_command = true;

    while(1)
    {
//        byteBuffer[0] = ' ';
        assert(readByte(hSerial, byteBuffer) == 0);

        if(byteBuffer[0] == 'A' && (previous == 'C' || route_state == START)) {
            byteBuffer[0] = command;
            writeByte(hSerial, byteBuffer);
            printf("C -> A || START, sending %c\n", byteBuffer[0]);
            return 'X';
        } else if(byteBuffer[0] == 'A') {
//            printf("Still on the line :)\n");
        } else if(byteBuffer[0] == 'C' && previous != 'C') {
            writeByte(hSerial, byteBuffer);
            printf("Confirmation C sent to robot.\n");
        } else {
            M_SLEEP(100);
            byteBuffer[0] = command;
            writeByte(hSerial, byteBuffer);
            printf("Command '%c' sent.\n", command);
        }

        previous = byteBuffer[0];

//        if(byteBuffer[0] != previous && (byteBuffer[0] != 'R' || byteBuffer[0] != 'L' || byteBuffer[0] != 'V' || byteBuffer[0] != 'O')) {
//
//            M_SLEEP(10);
//            writeByte(hSerial, byteBuffer);
//
//            assert(readByte(hSerial, byteBuffer) == 0);
//
//            if(byteBuffer[0] == 'C') {
//
//                if(fake_crossing) {
//                    byteBuffer[0] = 'V';
//                    printf("NO MINE!!\n");
//                } else
//                    byteBuffer[0] = command;
//
//                assert(writeByte(hSerial, byteBuffer) == 0);
//                assert(readByte(hSerial, byteBuffer) == 0);
//
//                previous = byteBuffer[0];
//
//                return 'C';
//            } else if(byteBuffer[0] == 'W' && checkpoint) {
//
//                byteBuffer[0] = 'O';
//
//                assert(writeByte(hSerial, byteBuffer) == 0);
//                assert(readByte(hSerial, byteBuffer) == 0);
//
//                previous = byteBuffer[0];
//                
//                return 'X';
//            } else if(byteBuffer[0] == 'M') {
//                
//                byteBuffer[0] = 'O';
//                printf("MINE!\n");
//                
//                assert(writeByte(hSerial, byteBuffer) == 0);
//                assert(readByte(hSerial, byteBuffer) == 0);
//                
//                previous = byteBuffer[0];
//                
//                return 'M';
//            } else {
//                
//                assert(readByte(hSerial, byteBuffer) == 0);
//                
//                if(previous != byteBuffer[0])
//                    printf("Found %c, doing nothing\n", byteBuffer[0]);
//                previous = byteBuffer[0];
//            }
//        }
    }
}
