#ifndef __SIMQ_H
#define __SIMQ_H

/*------------------------------ Includes ------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <queue.h> /* header file for all queue operations */
#include <fileManip.h> /* header file for all file manipulation */
#include <servicePoints.h> /* header file for all service point operation */
#include <boolean.h> /* header file for the BOOLEAN macros */

/*------------------------------ Function Prototypes ------------------------*/
void startSim(char [], char [], int);
void runSim(float, float, float, int, int, NODE **, NODE **, SERVE **, SERVE **, 
            int, int, char[], FILE **, int *, int *, int *, int *, int *);

#endif
