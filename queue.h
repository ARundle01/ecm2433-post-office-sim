#ifndef __QUEUE_H
#define __QUEUE_H

/*------------------------------ Includes ------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <boolean.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

/*------------------------------ Structure Definition ------------------------*/
/* represents a single node in the doubly linked-list */
struct node
{
    struct node *prev;
    int customerId;
    int toleranceLim;
    int serviceTime;
    int timeWaiting;
    int timeServing;
    int joinedAt;
    struct node *next;
};
typedef struct node NODE;

/*------------------------------ Function Prototypes -------------------------*/
extern void appendNode(NODE **, NODE **, int, int, int, int);
extern void deleteNode(NODE **, NODE **, NODE *);
extern void printList(NODE *);
extern int lengthOfQueue(NODE **, NODE **);
static void incrementWaiting(NODE *);
extern void incrementAllWaiting(NODE **);
static BOOLEAN isBored(NODE *, int);
extern void removeAllBored(NODE **, NODE **, int *, int);
extern void addNewCustomerNode(NODE **, NODE **, gsl_rng **, int *, int, float,
                               float, int, int);

#endif
