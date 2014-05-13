#ifndef MAIN_H
#define MAIN_H

#include "data.h"
#include "functions.h"

#define START_CP	1
#ifdef DEBUG
	#define printfv printf
#else
	#define printfv
#endif

extern int cp_num;
extern int cp;
extern int verbose;

int main(int argc, char* argv[]);

#endif /* MAIN_H */
