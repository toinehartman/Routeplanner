#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string.h>

#include "zigbee.h"

/*
//--------------------------------------------------------------
// Function: initSio
// Description: intializes the parameters as Baudrate, Bytesize,
//           Stopbits, Parity and Timeoutparameters of
//           the COM port
//--------------------------------------------------------------
*/
int n;
void initSio(HANDLE hSerial) {

    COMMTIMEOUTS timeouts ={0};
    DCB dcbSerialParams = {0};

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
       /* //error getting state*/
        printf("Error getting state \n");
        exit(EXIT_FAILURE);
    }

    dcbSerialParams.BaudRate = BAUDRATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if(!SetCommState(hSerial, &dcbSerialParams)){
     /*   //error setting serial port state*/
        printf("Error setting state \n");
        exit(EXIT_FAILURE);
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if(!SetCommTimeouts(hSerial, &timeouts)){
/*    //error occureed. Inform user*/
        printf("Error setting timeout state \n");
        exit(EXIT_FAILURE);
    }
}
/*
//--------------------------------------------------------------
// Function: readByte
// Description: reads a single byte from the COM port into
//              buffer buffRead
//--------------------------------------------------------------
*/
int readByte(HANDLE hSerial, char *buffRead) {

    DWORD dwBytesRead = 0;


    if (!ReadFile(hSerial, buffRead, 1, &dwBytesRead, NULL))
    {
        printf("Error reading byte from input buffer \n");
        return -1;
    }
//    printf("Byte read from read buffer is: %c \n", buffRead[0]);
    return 0;

}
/*
//--------------------------------------------------------------
// Function: writeByte
// Description: writes a single byte stored in buffRead to
//              the COM port
//--------------------------------------------------------------
*/
int writeByte(HANDLE hSerial, char *buffWrite) {

    DWORD dwBytesWritten = 0;

    if (!WriteFile(hSerial, buffWrite, 1, &dwBytesWritten, NULL))
    {
        printf("Error writing byte to output buffer \n");
        return -1;
    }
//    printf("Byte written to write buffer is: %c \n", buffWrite[0]);

    return 0;
}
