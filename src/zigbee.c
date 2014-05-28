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
    }

    dcbSerialParams.BaudRate = BAUDRATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if(!SetCommState(hSerial, &dcbSerialParams)){
     /*   //error setting serial port state*/
        printf("Error setting state \n");
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if(!SetCommTimeouts(hSerial, &timeouts)){
/*    //error occureed. Inform user*/
        printf("Error setting timeout state \n");
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
    }
    //printf("Byte read from read buffer is: %c \n", buffRead[0]);
    return(0);

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
        printf("error writing byte to output buffer \n");
    }
    printf("Byte written to write buffer is: %c \n", buffWrite[0]);

    return(0);
}

int mains() {
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

    if(hSerial == INVALID_HANDLE_VALUE){
        if(GetLastError()== ERROR_FILE_NOT_FOUND){
            /*//serial port does not exist. Inform user.*/
            printf(" serial port does not exist \n");
        }
        /*//some other error occurred. Inform user.*/
        printf(" some other error occured. Inform user.\n");
    }
/*
    //----------------------------------------------------------
    // Initialize the parameters of the COM port
    //----------------------------------------------------------
*/
    initSio(hSerial);

    while ( 1 ) {
        gets(byteBuffer);

        if (byteBuffer[0] == 'q') /*// end the loop by typing 'q'*/
            break;

        readByte(hSerial, byteBuffer);
        writeByte(hSerial, byteBuffer);

    }

    printf("ZIGBEE IO DONE!\n");
    return 0;

    CloseHandle(hSerial);
}
