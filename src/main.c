/**
 @file main.c The main file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "data.h"
#include "functions.h"
#include "zigbee.h"

int main(int argc, char* argv[])
{
    int i, j, *cp, cp_num, a;

    /* ZigBee initialisations */
    HANDLE hSerial;

    char byteBuffer[BUFSIZ+1];

    /*
        //----------------------------------------------------------
        // Open COMPORT for reading and writing
        //----------------------------------------------------------
        */
    hSerial = CreateFile(COMPORT,
                         GENERIC_READ | GENERIC_WRITE,
                         0,
                         0,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         0
                        );

    if(hSerial == INVALID_HANDLE_VALUE)
    {
        if(GetLastError()== ERROR_FILE_NOT_FOUND)
        {
            /* //serial port does not exist. Inform user.*/
            printf(" serial port does not exist \n");
        }
        /*    //some other error occurred. Inform user.*/
        printf(" some other error occured. Inform user.\n");
    }
    /*
        //----------------------------------------------------------
        // Initialize the parameters of the COM port
        //----------------------------------------------------------
    */
    initSio(hSerial);

    cp_num = argc - 1;
    cp = (int*) calloc(cp_num, sizeof(int));

    if (cp_num >= 1)   /* als checkpoints in argument zijn meegegeven, deze gebruiken */
    {
        for (i = 0; i < cp_num; i++)
        {
            a = atoi(argv[i + 1]);

            if (a >= 1 && a <= 12)
                cp[i] = a;
            else
            {
                printf("%d is not a number of an existing checkpoint!!\n", a);
                exit(EXIT_FAILURE);
            }
        }

        if (cp[0] != START_CP)
        {
            cp_num++;
            if ((cp = (int*) realloc(cp, (cp_num) * sizeof(int))) == NULL)
            {
                printf("ERROR: Unable to allocate enough memory for all destinations!\n");
                exit(EXIT_FAILURE);
            }

            for (i = cp_num - 1; i > 0; i--)
                cp[i] = cp[i - 1];
            cp[0] = START_CP;
        }

        printf("-----------------------------------\n");
        printf("Number of target checkpoints: %d\n", cp_num - 1);

        /* print checkpoints to go to */
        printf("Start at %d, go to: ", cp[0]);
        for (i = 1; i < cp_num; i++)
            printf("%d ", cp[i]);
        printf("\n-----------------------------------\n\n");
    }
    else   /* anders ERROR */
    {
        printf("At least 1 argument required!\n");
        exit(EXIT_FAILURE);
    }

    printf("\n");

    init_field();
    link_nodes();
    print_field();

    short_sort(cp, cp_num);

    printf("-------\nSorted!\n");
    for (j = 0; j < cp_num; j++) printf("%d ", cp[j]);
    printf("\n-------\n\n");

    if (read_mines() == 1) printf("No mine file!\n");

    route_sequence(hSerial, byteBuffer, cp, cp_num);
    free(cp);

    CloseHandle(hSerial);

    return 0;
}
