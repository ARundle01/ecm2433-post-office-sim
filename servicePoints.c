#include <servicePoints.h>

/* adds a new Service point to the list of service points */
void addServe(SERVE ** head, SERVE ** tail)
{
    SERVE *newServe;
    SERVE *lastServe = *tail;
    
    /* attempt to allocate memory to the service point */
    if ((newServe = (SERVE *)malloc(sizeof(SERVE)) ) == NULL) 
    {
        printf("Memory Allocation Error when creating new Serve point\n");
        fprintf(stderr, "Error %d: %s\n", errno, strerror(errno));
        exit(-1);
    }
    else
    {
        /* initialize all to -1 to denote it as empty */
        newServe->customerId = -1;
        newServe->serviceTime = -1;
        newServe->timeWaiting = -1;
        newServe->timeServing = -1;

        newServe->next = NULL;
    }
    
    if (*head == NULL) /* if head is empty, assign the serve to head */
    {
        newServe->prev = NULL;
        *head = newServe;
        *tail = newServe;
    }
    else /* otherwise place it at the end of the list */
    {
        lastServe->next = newServe;
        newServe->prev = lastServe;
        *tail = newServe;
    }
}


/* adds an N amount of service points */
void addNServes(SERVE ** head, SERVE ** tail, int N)
{
    int serveNum;
    for (serveNum = 0; serveNum < N; serveNum++)
    {
        addServe(head, tail);
    }
}


/* empties a given service point (replaces variables with -1) */
void emptyServe(SERVE * emptyServe)
{
    emptyServe->customerId = -1;
    emptyServe->serviceTime = -1;
    emptyServe->timeWaiting = -1;
    emptyServe->timeServing = -1;
}


/* empties all service points */
void emptyAllServes(SERVE ** head)
{
    SERVE *lastServe;
    SERVE *currentServe = *head;
    while (currentServe != NULL) /* iterate over the list */
    {
        emptyServe(currentServe); /* empty the serve */
        lastServe = currentServe; /* move to next serve */
        currentServe = currentServe->next;
    }
} 


/* predicate to show if a service point is empty */
BOOLEAN isEmptyServe(SERVE * servePoint)
{
    int customerId = servePoint->customerId;
    int serviceTime = servePoint->serviceTime;
    int timeServing = servePoint->timeServing;
    int timeWaiting = servePoint->timeWaiting;

    /* if all variables are -1, it is empty */
    if (customerId  == -1 && 
        serviceTime == -1 && 
        timeServing == -1 && 
        timeWaiting == -1)
        return TRUE;
    else
        return FALSE;
}


/* returns an int, the number of empty serves the list has */
int hasEmptyServe(SERVE ** head)
{
    SERVE *lastServe;
    SERVE *currentServe = *head;
    int emptyNum = 0;
    while(currentServe != NULL) /* iterate over the list */
    {
        if (isEmptyServe(currentServe) == TRUE) /* if it is empty */
            emptyNum++; /* increment the number of empty serves */
        
        lastServe = currentServe; /* move to next serve */
        currentServe = currentServe->next;
    }
    return emptyNum;
}


/* returns the int length of the list of serves, including empty serves */
int lengthOfServe(SERVE ** head)
{
    SERVE *lastServe;
    SERVE *currentServe = *head;
    int len = 0;
    while(currentServe != NULL) /* iterate over the list */
    {
        len++; /* increment length */
        lastServe = currentServe; /* move to next serve */
        currentServe = currentServe->next;
    }
    return len;
}


/* updates a given serve using the given variables */
void updateServe(SERVE * updatedServe, int customerId, int serviceTime, 
                 int timeWaiting)
{
        updatedServe->customerId = customerId; /* update the customerId */
        updatedServe->serviceTime = serviceTime; /* update the serviceTime */
        updatedServe->timeServing = 0; /* set the timeServing to 0 */
        updatedServe->timeWaiting = timeWaiting; /* update the timeWaiting */
}


/* finds the first empty serve and updates it using the head node of the 
 * queue */
void updateFirstEmpty(SERVE ** serveHead, NODE ** nodeHead, 
                      NODE ** nodeTail)
{
    if (*nodeHead != NULL) /* if the head is niot null 
                              (aka the queue is not emptyi) */
    {
        NODE *nodeToUse = *nodeHead;
        SERVE *lastServe;
        SERVE *currentServe = *serveHead;
        while(currentServe != NULL) /* iterate over the list of serves */
        {
            if (isEmptyServe(currentServe) == TRUE) /* if the serve is empty */
            {
                /* update the empty serve with the given details */
                int customerId = nodeToUse->customerId;
                int serviceTime = nodeToUse->serviceTime;
                int timeWaiting = nodeToUse->timeWaiting;
                updateServe(currentServe, customerId, serviceTime, timeWaiting);
                /* delete the node in the queue, as they are being served now */
                deleteNode(nodeHead, nodeTail, nodeToUse);
                break;
            }
            else
            {
                /* otherwise, move to the next node */
                lastServe = currentServe;
                currentServe = currentServe->next;
            }
        }
    }
}


/* continuously fills empty serves until all empty serves are full or
 * until the queue is empty */
void fillEmptyServes(SERVE ** serveHead, NODE ** nodeHead, NODE ** nodeTail)
{
    while (hasEmptyServe(serveHead) > 0) /* continue as long as there are empty
                                            serves left */
    {
        if (lengthOfQueue(nodeHead, nodeTail) > 0) /* if the queue isn't empty */
            updateFirstEmpty(serveHead, nodeHead, nodeTail); /* update the serve */
        else
            break;
    }
}


/* deletes the tail of the list of servesi, freeing the allocated memory */
void deleteLastServe(SERVE ** head, SERVE ** tail)
{
    SERVE *currentServe = *tail;
    SERVE *prevServe = currentServe->prev;
    if (lengthOfServe(head) > 1) /* if there is more than one serve left */
    {
        prevServe->next = NULL; /* set the previous serve's next serve to NULL */
        *tail = prevServe; /* set the tail to the previous serve */
        currentServe->prev = NULL; /* set the previous serve of current to NULL */
        free(currentServe); /* free the memory of the current serve */
    }
    else /* if there is only one serve left */
    {
        *head = NULL; /* set head and tail to NULL */
        *tail = NULL;
        free(currentServe); /* free the memory */
    }
}


/* deletes all serves, starting from the tail and working backwards */
void deleteAllServes(SERVE ** head, SERVE ** tail)
{
    while (lengthOfServe(head) >= 0) /* while the queue is not empty */
    {
        deleteLastServe(head, tail); /* delete the serve */
        if (lengthOfServe(head) == 0) /* if the queue is empty, break */
            break;
    } 
}


/* prints all serves in the list, starting from the head */
void printServes(SERVE ** head)
{
    SERVE *currentServe = *head;
    SERVE *lastServe;
    
    while (currentServe != NULL) /* iterate over the list */
    {
        int customerId = currentServe->customerId;
        int serviceTime = currentServe->serviceTime;
        int timeWaiting = currentServe->timeWaiting;
        int timeServing = currentServe->timeServing;
        /* print the serve details */
        printf("customerId: %d\nserviceTime: %d\ntimeWaiting: %d\ntimeServing: "
               "%d\n", customerId, serviceTime, timeWaiting, timeServing);
        
        lastServe = currentServe; /* move to next serve */
        currentServe = currentServe->next;
    }
}


/* increments the time that a serve has been served for */
void incrementServing(SERVE * servePoint)
{
    servePoint->timeServing += 1;
}


/* increments the time serving of all serves in the list */
void incrementAllServes(SERVE ** head)
{
    SERVE *currentServe = *head;
    SERVE *lastServe;
    
    while(currentServe != NULL) /* iterate over list */
    {
        if (isEmptyServe(currentServe) == FALSE) /* if it is not empty */
        {
            incrementServing(currentServe); /* increment serve time */
        }
        
        lastServe = currentServe; /* move to next serve */
        currentServe = currentServe->next;
    }
}


/* predicate that tells if a serve has finished being served, aka the service
 * time and the time being served are equal */
BOOLEAN isFulfilled(SERVE * currentServe)
{
    if (isEmptyServe(currentServe) == FALSE) /* if the serve is not empty */
    {
        int serviceTime = currentServe->serviceTime;
        int timeServing = currentServe->timeServing;
    
        if (serviceTime == timeServing) /* if the times are both equal */
            return TRUE; /* it has been fulfilled */
        else 
            return FALSE; /* it has not been fulfilled */
    }
    else /* if the serve is empty */
        return FALSE; /* it cannot be fulfilled */
}


/* fulfills all non-empty serves in the list */
void fulfillServes(SERVE ** head, int * numFulfill, int * totalWait)
{
    /* if there is at least one non-empty serve */
    if (hasEmptyServe(head) < lengthOfServe(head))
    {
        SERVE *currentServe = *head;
        SERVE *lastServe;
        
        while (currentServe != NULL) /* iterate over the list */
        {
            if (isFulfilled(currentServe) == TRUE) /* if it is fulfilled */
            {
                (*numFulfill)++; /* iterate tracker of fulfillments */
                /* add wait time onto total wait for whole sim */
                (*totalWait) += currentServe->timeWaiting;
                emptyServe(currentServe); /* empty the serve point */
            }
            
            lastServe = currentServe; /* mvoe to next serve */
            currentServe = currentServe->next;
        }
    }
}

