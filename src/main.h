/**
 @file main.h The main header file.
 */

#ifndef MAIN_H
#define MAIN_H

#include "data.h"
#include "functions.h"

#define START_CP	1

#ifdef DEBUG
# define VERBOSE 1
#else
# define VERBOSE 0
#endif

extern int cp_num;
extern int cp;

int main(int argc, char* argv[]);

#endif /* MAIN_H */
