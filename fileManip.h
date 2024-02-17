#ifndef __FILEMANIP_H
#define __FILEMANIP_H

/*------------------------------ Includes ------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <boolean.h>

/*------------------------------ Function Prototypes -------------------------*/
void clearFile(char []);
void writeToFile(char [], char []);
BOOLEAN checkLastThree(char []);
void getVariableFromFile(char [], int *, int *, int *, 
                         float *, float *, float *);

#endif
