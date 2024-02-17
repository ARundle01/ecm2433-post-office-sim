#ifndef __SERVICEPOINTS_H
#define __SERVICEPOINTS_H

/* ------------------------------ Includes ----------------------------------*/
#include <boolean.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <queue.h>

/* ------------------------------ Structure Definition ----------------------*/
/* this struct represents a single service point in a linked-list of
 * service points, a service point always contains the relevant details
 * of the customer.
 * When it is empty, all int variables are set to -1, as this avoids any 
 * potential numerical clashes (the times cannot be -1 and the customerId
 * is incremented from 0, for every customer) */
struct serve
{
    struct serve *prev;
    int customerId;
    int serviceTime;
    int timeWaiting;
    int timeServing;
    struct serve *next;
};
typedef struct serve SERVE;

/* ------------------------------ Function Prototypes -----------------------*/
static void addServe(SERVE **, SERVE ** tail);
extern void addNServes(SERVE **, SERVE **, int);
extern void emptyServe(SERVE *);
extern void emptyAllServes(SERVE **);
static BOOLEAN isEmptyServe(SERVE *);
extern int hasEmptyServe(SERVE **);
extern int lengthOfServe(SERVE **);
static void updateServe(SERVE *, int, int, int);
extern void updateFirstEmpty(SERVE **, NODE **, NODE **);
extern void fillEmptyServes(SERVE **, NODE **, NODE **);
extern void deleteLastServe(SERVE **, SERVE **);
extern void deleteAllServes(SERVE **, SERVE **);
extern void printServes(SERVE **);
static void incrementServing(SERVE *);
extern void incrementAllServes(SERVE **);
extern BOOLEAN isFulfilled(SERVE *);
extern void fulfillServes(SERVE **, int *, int *);

#endif
