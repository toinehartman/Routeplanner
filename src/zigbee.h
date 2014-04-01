#ifndef ZIGBEE_H
#define ZIGBEE_H

#include <Windows.h>

#define COMPORT "COM5"
#define BAUDRATE CBR_9600

void initSio(HANDLE hSerial);
int readByte(HANDLE hSerial, char *buffRead);
int writeByte(HANDLE hSerial, char *buffWrite);

#endif /* ZIGBEE_H */
